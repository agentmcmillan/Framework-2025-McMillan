#include "uart_link.hpp"
#include <stdarg.h>

namespace UartLink {

static String s_in;
static CmdHandler s_cb = nullptr;

// Use Serial1 on RP2040 and remap pins BEFORE begin()
void init(unsigned long baud) {
    Serial1.setTX(12);     // UART0 TX
    Serial1.setRX(13);     // UART0 RX  (not 23)
  Serial1.begin(baud);

  // Tiny hello so host can tell we’re alive
  log("info", "rp2040-uart up");
}

void onCommand(CmdHandler cb) { s_cb = cb; }

// Internal: write a line safely
static inline void writeLine(const String& L) {
  Serial1.print(L); Serial1.print('\n');
}

// ---- Public senders (newline-delimited JSON) ----
void sendHeartbeat(uint32_t reqMs, uint16_t seenBadges, uint16_t queueLen) {
  // {"t":"hb","uptime":12345,"req_ms":900,"seen":42,"q":0}
  String j;
  j.reserve(96);
  j += F("{\"t\":\"hb\",\"uptime\":");
  j += millis();
  j += F(",\"req_ms\":");
  j += reqMs;
  j += F(",\"seen\":");
  j += seenBadges;
  j += F(",\"q\":");
  j += queueLen;
  j += '}';
  writeLine(j);
}

void sendScore(uint16_t badgeId, uint32_t score, uint8_t parts) {
  // {"t":"score","id":123,"score":4567,"parts":3,"ms":123456}
  String j;
  j.reserve(96);
  j += F("{\"t\":\"score\",\"id\":");
  j += badgeId;
  j += F(",\"score\":");
  j += score;
  j += F(",\"parts\":");
  j += parts;
  j += F(",\"ms\":");
  j += millis();
  j += '}';
  writeLine(j);
}

void sendPart(uint16_t badgeId, uint8_t partIdx, uint8_t bits, uint8_t haveMask) {
  // {"t":"part","id":1,"p":0,"bits":26,"have":5}
  String j;
  j.reserve(80);
  j += F("{\"t\":\"part\",\"id\":");
  j += badgeId;
  j += F(",\"p\":");
  j += partIdx;
  j += F(",\"bits\":");
  j += bits;
  j += F(",\"have\":");
  j += haveMask;
  j += '}';
  writeLine(j);
}

void log(const char* level, const char* msg) {
  // {"t":"log","lvl":"info","msg":"text"}
  String j;
  j.reserve(80);
  j += F("{\"t\":\"log\",\"lvl\":\"");
  j += level;
  j += F("\",\"msg\":\"");
  // escape quotes minimally
  for (const char* p = msg; *p; ++p) {
    if (*p == '\"') j += '\\';
    j += *p;
  }
  j += F("\"}");
  writeLine(j);
}

void logf(const char* level, const char* fmt, ...) {
  char buf[160];
  va_list ap;
  va_start(ap, fmt);
  vsnprintf(buf, sizeof(buf), fmt, ap);
  va_end(ap);
  log(level, buf);
}

// ---- RX line assembler (simple text lines, e.g. JSON or commands) ----
void poll() {
  while (Serial1.available()) {
    char c = (char)Serial1.read();
    if (c == '\r') continue;
    if (c != '\n') {
      if (s_in.length() < 512) s_in += c;
      continue;
    }
    // got a line
    String line = s_in; s_in = "";
    line.trim();
    if (!line.length()) continue;

    // Built-ins: PING -> PONG, VER -> id string
    if (line.equalsIgnoreCase("PING")) { writeLine(F("{\"t\":\"pong\"}")); continue; }
    if (line.equalsIgnoreCase("VER"))  { writeLine(F("{\"t\":\"ver\",\"who\":\"rp2040-score\"}")); continue; }

    // Upstream handler (ESP can send JSON like {"t":"set","req_ms":800})
    if (s_cb) s_cb(line);
  }
}

} // namespace UartLink