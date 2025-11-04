#pragma once
#include <Arduino.h>

// --- Wire map (RP2040 <-> ESP32-S3) ---
// RP2040 TX -> GPIO12  --> ESP32-S3 RX (GPIO44)
// RP2040 RX -> GPIO23  <-- ESP32-S3 TX (GPIO43)
// GND to GND

namespace UartLink {

// call once in setup (115200 is good)
void init(unsigned long baud = 115200);

// call often in loop (handles RX line assembly and simple commands)
void poll();

// senders (newline-delimited JSON)
void sendHeartbeat(uint32_t reqMs, uint16_t seenBadges, uint16_t queueLen = 0);
void sendScore(uint16_t badgeId, uint32_t score, uint8_t parts = 3);
void sendPart(uint16_t badgeId, uint8_t partIdx, uint8_t bits, uint8_t haveMask);
void log(const char* level, const char* msg);           // level: "info","warn","err"
void logf(const char* level, const char* fmt, ...);     // printf-style

// optional: simple command hook you can implement in your main
// e.g. {"t":"set","req_ms":800}
typedef void (*CmdHandler)(const String& line);
void onCommand(CmdHandler cb);

} // namespace UartLink