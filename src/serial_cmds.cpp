#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <FastLED.h>
#include "serial_cmds.hpp"
#include "config.hpp"

extern BadgeConfig CFG;

// ---- weak hooks so you don’t have to implement them if you don’t want to
__attribute__((weak)) void restartNameScroll() {}
__attribute__((weak)) bool msgqPush(uint8_t, uint8_t, uint16_t) { return false; }
__attribute__((weak)) uint16_t badgeColor565() { return 0xFFFF; }

// --- helpers ---
static inline uint8_t map128to255(uint8_t v){ return (uint16_t(v) * 255u) / 128u; }

// Accept "#RRGGBB" or "R,G,B"
static bool parseRgb(const String& s, uint32_t &rgbOut) {
  if (s.length() >= 7 && s[0] == '#') {
    long v = strtol(s.c_str() + 1, nullptr, 16);
    if (v >= 0 && v <= 0xFFFFFF) { rgbOut = (uint32_t)v; return true; }
    return false;
  }
  int c1 = s.indexOf(','), c2 = (c1 >= 0) ? s.indexOf(',', c1 + 1) : -1;
  if (c1 > 0 && c2 > c1) {
    int r = s.substring(0, c1).toInt();
    int g = s.substring(c1 + 1, c2).toInt();
    int b = s.substring(c2 + 1).toInt();
    r = constrain(r, 0, 255);
    g = constrain(g, 0, 255);
    b = constrain(b, 0, 255);
    rgbOut = (uint32_t(r) << 16) | (uint32_t(g) << 8) | uint32_t(b);
    return true;
  }
  return false;
}

static void printHelp() {
  Serial.println(F("YOU SHOULDN'T BE HERE"));
  Serial.println(F("-talk to us cats@fuse-tg.com"));
  Serial.println(F("Commands:"));
  Serial.println(F("  GET STATUS | NAME | ID | COLOR | BRIGHTNESS | UNLOCK_MASK | ALL"));
  Serial.println(F("  SET NAME=<text>"));
  Serial.println(F("  SET ID=<0..511>"));
  Serial.println(F("  SET COLOR=<#RRGGBB|R,G,B>"));
  Serial.println(F("  SET BRIGHTNESS=<0..128>   (alias: SET BRIGHT=)"));
  Serial.println(F("  UNLOCK CHARM=<0..31>      (aliases: SET UNLOCK <n>, UNLOCK <n>)"));
  Serial.println(F("  SET LOCK CHARM=<0..31>    (alias: LOCK <n>)"));
  Serial.println(F("  MSG ADD <id 0..127> <text…>"));
  Serial.println(F("  MSG DEL <id>"));
  Serial.println(F("  SLEEP <minutes 0..127> / WAKE"));
  Serial.println(F("  SAVE | LOAD | DUMP | PING | HELP"));
}

static void printAllText() {
  Serial.printf("NAME %s\n", CFG.name.c_str());
  Serial.printf("ID %u\n", CFG.id);
  Serial.printf("COLOR #%06lX\n", (unsigned long)CFG.colorRGB);
  Serial.printf("BRIGHTNESS %u\n", CFG.brightness);
  Serial.printf("UNLOCK_MASK 0x%08lX\n", (unsigned long)CFG.unlockedMask);
}

static void printStatusJson() {
  // One-line JSON for your page to parse
  StaticJsonDocument<256> doc;
  doc["name"]         = CFG.name;
  doc["id"]           = CFG.id;
  doc["colorRGB"]     = CFG.colorRGB;
  doc["brightness"]   = CFG.brightness;
  doc["unlockedMask"] = CFG.unlockedMask;
  serializeJson(doc, Serial);
  Serial.println();
}

// ========== Message storage (optional persistence) ==========
static const char* kMsgsPath = "/msgs.json";

// simple in-RAM table; if you want, call loadMsgs() once in setup()
static String g_msgText[128];

static void saveMsgs() {
  StaticJsonDocument<4096> doc; // enough for a handful of strings
  JsonObject root = doc.to<JsonObject>();
  for (int i=0;i<128;i++) if (g_msgText[i].length()) root[String(i)] = g_msgText[i];
  File f = LittleFS.open(kMsgsPath, "w");
  if (!f) return;
  serializeJson(doc, f);
  f.close();
}
static void loadMsgs() {
  if (!LittleFS.exists(kMsgsPath)) return;
  File f = LittleFS.open(kMsgsPath, "r");
  if (!f) return;
  StaticJsonDocument<4096> doc;
  DeserializationError e = deserializeJson(doc, f);
  f.close();
  if (e) return;
  for (JsonPair kv : doc.as<JsonObject>()) {
    int id = atoi(kv.key().c_str());
    if (id >= 0 && id < 128) g_msgText[id] = kv.value().as<const char*>();
  }
}

// ========== Parser ==========
static String s_inLine;

static void processLine(const String& rawLine) {
  String line = rawLine; line.trim();
  if (line.isEmpty()) return;

  // First token (uppercased)
  int sp = line.indexOf(' ');
  String w0 = (sp < 0) ? line : line.substring(0, sp);
  String rest = (sp < 0) ? String() : line.substring(sp + 1);
  w0.toUpperCase();

  // ---------- GET ----------
  if (w0 == "GET") {
    String fld = rest; fld.trim(); fld.toUpperCase();
    if (fld == "STATUS") { printStatusJson(); return; }
    if (fld == "NAME") { Serial.printf("NAME %s\n", CFG.name.c_str()); return; }
    if (fld == "ID") { Serial.printf("ID %u\n", CFG.id); return; }
    if (fld == "COLOR") { Serial.printf("COLOR #%06lX\n", (unsigned long)CFG.colorRGB); return; }
    if (fld == "BRIGHT" || fld == "BRIGHTNESS") { Serial.printf("BRIGHTNESS %u\n", CFG.brightness); return; }
    if (fld == "UNLOCK_MASK") { Serial.printf("UNLOCK_MASK 0x%08lX\n", (unsigned long)CFG.unlockedMask); return; }
    if (fld == "ALL") { printAllText(); return; }
    Serial.println(F("ERR Unknown GET field"));
    return;
  }

  // ---------- PING ----------
  if (w0 == "PING") { Serial.println(F("PONG")); return; }

  // ---------- SAVE / LOAD / DUMP / HELP ----------
  if (w0 == "SAVE") { saveConfig(CFG); saveMsgs(); Serial.println(F("OK")); return; }
  if (w0 == "LOAD") { loadConfig(CFG); loadMsgs(); Serial.println(F("OK")); return; }
  if (w0 == "DUMP") { printAllText(); return; }
  if (w0 == "HELP") { printHelp(); return; }

  // ---------- SLEEP / WAKE ----------
  if (w0 == "SLEEP") {
    int minutes = constrain(rest.toInt(), 0, 127);
    // If you want to act locally, do it here; for now just ACK.
    Serial.println(F("OK"));
    return;
  }
  if (w0 == "WAKE") { Serial.println(F("OK")); return; }

  // ---------- MSG ADD/DEL ----------
  if (w0 == "MSG") {
    int sp2 = rest.indexOf(' ');
    String sub = (sp2 < 0) ? rest : rest.substring(0, sp2);
    String args = (sp2 < 0) ? String() : rest.substring(sp2 + 1);
    sub.toUpperCase(); args.trim();

    if (sub == "ADD") {
      int sp3 = args.indexOf(' ');
      if (sp3 < 0) { Serial.println(F("ERR usage: MSG ADD <id> <text>")); return; }
      int id = constrain(args.substring(0, sp3).toInt(), 0, 127);
      String txt = args.substring(sp3 + 1);
      if (!txt.length()) { Serial.println(F("ERR empty text")); return; }
      g_msgText[id] = txt;
      saveMsgs();
      Serial.println(F("OK"));
      return;
    }
    if (sub == "DEL") {
      int id = constrain(args.toInt(), 0, 127);
      g_msgText[id] = "";
      saveMsgs();
      Serial.println(F("OK"));
      return;
    }
    Serial.println(F("ERR Unknown MSG cmd"));
    return;
  }

  // ---------- UNLOCK / LOCK (various spellings) ----------
  if (w0 == "UNLOCK") {
    String arg = rest; arg.trim();
    // Support "UNLOCK CHARM=<n>" and "UNLOCK <n>"
    int eq = arg.indexOf('=');
    if (eq >= 0) arg = arg.substring(eq + 1);
    int n = constrain(arg.toInt(), 0, 31);
    CFG.unlockedMask |= (1UL << n);
    saveConfig(CFG);
    Serial.println(F("OK"));
    return;
  }
  if (w0 == "LOCK") {
    String arg = rest; arg.trim();
    // Support "LOCK CHARM=<n>" and "LOCK <n>"
    int eq = arg.indexOf('=');
    if (eq >= 0) arg = arg.substring(eq + 1);
    int n = constrain(arg.toInt(), 0, 31);
    CFG.unlockedMask &= ~(1UL << n);
    saveConfig(CFG);
    Serial.println(F("OK"));
    return;
  }

  // ---------- SET ----------
  if (w0 == "SET") {
    // Accept both: "SET FIELD=value" and "SET FIELD value"
    String sub, val;

    // Split first token
    int sp2 = rest.indexOf(' ');
    if (sp2 < 0) { sub = rest; val = ""; }
    else { sub = rest.substring(0, sp2); val = rest.substring(sp2 + 1); }

    // If "FIELD=VALUE" in either sub or val, normalize:
    int eq = sub.indexOf('=');
    if (eq >= 0) {
      val = sub.substring(eq + 1) + (val.length() ? (" " + val) : "");
      sub = sub.substring(0, eq);
    } else {
      // maybe "FIELD" and then "=VALUE" at start of val
      if (val.startsWith("=")) val.remove(0,1);
    }
    sub.trim(); val.trim();
    sub.toUpperCase();

    if (sub == "NAME") {
      if (!val.length()) { Serial.println(F("ERR NAME needs text")); return; }
      CFG.name = val;
      saveConfig(CFG);
      restartNameScroll();
      Serial.println(F("OK"));
      return;
    }

    if (sub == "ID") {
      int id = constrain(val.toInt(), 0, 511); // protocol is 9-bit
      CFG.id = (uint16_t)id;
      saveConfig(CFG);
      Serial.println(F("OK"));
      return;
    }

    if (sub == "COLOR") {
      uint32_t rgb;
      if (!parseRgb(val, rgb)) { Serial.println(F("ERR COLOR format")); return; }
      CFG.colorRGB = rgb;
      saveConfig(CFG);
      Serial.println(F("OK"));
      return;
    }

    if (sub == "BRIGHT" || sub == "BRIGHTNESS") {
      int b = constrain(val.toInt(), 0, 128);
      CFG.brightness = (uint8_t)b;
      FastLED.setBrightness(map128to255(CFG.brightness));
      FastLED.show();
      saveConfig(CFG);
      Serial.println(F("OK"));
      return;
    }

    if (sub == "UNLOCK") {           // e.g. SET UNLOCK 7
      int n = constrain(val.toInt(), 0, 31);
      CFG.unlockedMask |= (1UL << n);
      saveConfig(CFG);
      Serial.println(F("OK"));
      return;
    }
    if (sub == "LOCK") {             // e.g. SET LOCK 7  or  SET LOCK CHARM=7
      // support "LOCK CHARM=7"
      int eq2 = val.indexOf('=');
      if (eq2 >= 0) val = val.substring(eq2 + 1);
      int n = constrain(val.toInt(), 0, 31);
      CFG.unlockedMask &= ~(1UL << n);
      saveConfig(CFG);
      Serial.println(F("OK"));
      return;
    }

    // Web page uses these exact forms:
    if (sub == "LOCK CHARM") {       // "SET LOCK CHARM=7" (handled above via generic LOCK, but keep alias)
      int eq3 = val.indexOf('=');
      if (eq3 >= 0) val = val.substring(eq3 + 1);
      int n = constrain(val.toInt(), 0, 31);
      CFG.unlockedMask &= ~(1UL << n);
      saveConfig(CFG);
      Serial.println(F("OK"));
      return;
    }

    Serial.println(F("ERR Unknown SET field"));
    return;
  }

  Serial.println(F("ERR Unknown command. Try HELP."));
}

// ========== Public: call from loop() ==========
void handleSerial() {
  while (Serial.available()) {
    char c = (char)Serial.read();
    if (c == '\r') continue;
    if (c == '\n') {
      String line = s_inLine; s_inLine = "";
      processLine(line);
    } else if (s_inLine.length() < 512) {
      s_inLine += c;
    }
  }
}