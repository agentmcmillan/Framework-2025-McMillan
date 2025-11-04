#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <FastLED.h>

#include "config.hpp"      // BadgeConfig CFG; saveConfig/loadConfig
#include "serial_cmds.hpp" // this header
#include "protocol.h"      // (op codes if you need them)

// --------- externs provided by main.cpp ----------
extern BadgeConfig CFG;
// IMPORTANT: ensure startScroll in main.cpp is NOT 'static'
extern void startScroll(bool idle, uint8_t msgId, uint8_t reps, uint16_t color);
// Brightness mapper from 0..128 -> 0..255
extern uint8_t map128to255(uint8_t v);

// ============== tunables ==============
// Increase max single-line command length (e.g., for large SET DUMP payloads)
#ifndef SERIAL_LINE_MAX
#define SERIAL_LINE_MAX 2048
#endif

// ============== small helpers ==============
static bool parseRgbTriplet(const String& s, uint32_t &rgbOut) {
  // Accept #RRGGBB or R,G,B
  if (s.length() >= 7 && s[0] == '#') {
    char *endp = nullptr;
    long v = strtol(s.c_str() + 1, &endp, 16);
    if (endp && v >= 0 && v <= 0xFFFFFF) { rgbOut = (uint32_t)v; return true; }
    return false;
  }
  int c1 = s.indexOf(','); if (c1 < 0) return false;
  int c2 = s.indexOf(',', c1 + 1); if (c2 < 0) return false;
  int r = s.substring(0, c1).toInt();
  int g = s.substring(c1 + 1, c2).toInt();
  int b = s.substring(c2 + 1).toInt();
  r = constrain(r, 0, 255); g = constrain(g, 0, 255); b = constrain(b, 0, 255);
  rgbOut = (uint32_t(r) << 16) | (uint32_t(g) << 8) | uint32_t(b);
  return true;
}

static String binToHex(const uint8_t* data, size_t len) {
  static const char* H = "0123456789ABCDEF";
  String out; out.reserve(len * 2);
  for (size_t i = 0; i < len; ++i) { out += H[(data[i] >> 4) & 0xF]; out += H[data[i] & 0xF]; }
  return out;
}
static int hexNib(char c) {
  if (c >= '0' && c <= '9') return c - '0';
  if (c >= 'a' && c <= 'f') return 10 + (c - 'a');
  if (c >= 'A' && c <= 'F') return 10 + (c - 'A');
  return -1;
}
static bool hexToBin(const String& s, uint8_t* out, size_t outLen) {
  if (s.length() != outLen * 2) return false;
  for (size_t i = 0; i < outLen; ++i) {
    int hi = hexNib(s[2*i]), lo = hexNib(s[2*i+1]);
    if (hi < 0 || lo < 0) return false;
    out[i] = uint8_t((hi << 4) | lo);
  }
  return true;
}

// ============== STATUS JSON (for WebSerial UI) ==============
static void printStatusJson() {
  JsonDocument doc;                   // v7 API
  doc["name"]         = CFG.name;
  doc["id"]           = CFG.id;
  doc["brightness"]   = CFG.brightness;
  doc["colorRGB"]     = CFG.colorRGB;
  doc["unlockedMask"] = CFG.unlockedMask;
  doc["userCharmId"]  = CFG.userCharmId;  // 0..31, or 255 for none
  doc["sleepDisabled"] = CFG.sleepDisabled;   // <-- NEW
  serializeJson(doc, Serial);
  Serial.println();
}

// ============== BACKUP (GET DUMP) ==============
static void printBadgeDumpJson() {
  // attackers map: 64 bytes stored in /attackers.bin
  uint8_t attackers[64] = {0};
  if (LittleFS.exists("/attackers.bin")) {
    File f = LittleFS.open("/attackers.bin", "r");
    if (f) { (void)f.read(attackers, sizeof(attackers)); f.close(); }
  }

  JsonDocument doc;  // dynamic in v7
  doc["version"] = 1;
  doc["fw"]      = "cats-badge";

  auto cfg = doc["cfg"].to<JsonObject>();
  cfg["name"]         = CFG.name;
  cfg["id"]           = CFG.id;
  cfg["brightness"]   = CFG.brightness;
  cfg["colorRGB"]     = CFG.colorRGB;
  cfg["unlockedMask"] = CFG.unlockedMask;
  cfg["userCharmId"]  = CFG.userCharmId;  // include in dump

  auto stats = doc["stats"].to<JsonObject>();
  stats["attackers_hex"]   = binToHex(attackers, sizeof(attackers));
  stats["score"]           = 0;  // placeholders for future growth
  stats["hitsTotal"]       = 0;
  stats["firePresses"]     = 0;
  stats["sessionsAttended"]= 0;

  serializeJson(doc, Serial);
  Serial.println();
}

// ============== RESTORE (SET DUMP=...) ==============
static bool applyBadgeDumpJson(const char* json, size_t len) {
  JsonDocument doc;
  DeserializationError e = deserializeJson(doc, json, len);
  if (e) { Serial.printf("ERR DUMP JSON parse: %s\n", e.c_str()); return false; }

  uint32_t ver = doc["version"] | 0;
  if (!ver) { Serial.println("ERR DUMP version missing/invalid"); return false; }

  if (doc["cfg"].is<JsonObject>()) {
    JsonObject c = doc["cfg"].as<JsonObject>();
    if (c["name"].is<const char*>())    CFG.name = c["name"].as<const char*>();
    if (c["id"].is<uint32_t>())         CFG.id   = (uint16_t)c["id"].as<uint32_t>();
    if (c["brightness"].is<uint32_t>()) CFG.brightness = (uint8_t)c["brightness"].as<uint32_t>();
    if (c["colorRGB"].is<uint32_t>())   CFG.colorRGB   = (uint32_t)c["colorRGB"].as<uint32_t>();
    if (c["unlockedMask"].is<uint32_t>()) CFG.unlockedMask = (uint32_t)c["unlockedMask"].as<uint32_t>();
    if (c["userCharmId"].is<uint32_t>())  CFG.userCharmId  = (uint8_t)c["userCharmId"].as<uint32_t>();
  }

  if (doc["stats"].is<JsonObject>()) {
    JsonObject s = doc["stats"].as<JsonObject>();
    if (s["attackers_hex"].is<const char*>()) {
      String hex = s["attackers_hex"].as<const char*>();
      uint8_t attackers[64];
      if (hexToBin(hex, attackers, sizeof(attackers))) {
        File f = LittleFS.open("/attackers.bin", "w");
        if (f) { (void)f.write(attackers, sizeof(attackers)); f.close(); }
      } else {
        Serial.println("WARN attackers_hex invalid");
      }
    }
  }

  saveConfig(CFG);
  FastLED.setBrightness(map128to255(CFG.brightness));
  FastLED.show();
  startScroll(/*idle*/true, /*msgId*/0, /*reps*/0, /*color*/0);
  return true;
}

// ============== Console parser ==============
// Larger input buffer with overflow protection
static String s_input;

static void cmd_HELP() {
  Serial.println(F("Commands:"));
  Serial.println(F("  GET STATUS"));
  Serial.println(F("  GET DUMP"));
  Serial.println(F("  SET DUMP={json}"));
  Serial.println(F("  SET NAME=<text>"));
  Serial.println(F("  SET ID=<0..1023>"));
  Serial.println(F("  SET COLOR=<#RRGGBB|R,G,B>"));
  Serial.println(F("  SET BRIGHTNESS=<0..128>"));
  Serial.println(F("  SET USERCHARM=<0..31|-1 to clear>"));
  Serial.println(F("  UNLOCK CHARM=<0..31>"));
  Serial.println(F("  SET LOCK CHARM=<0..31>"));
  Serial.println(F("  MSG ADD <id> <text>"));
  Serial.println(F("  MSG DEL <id>"));
  Serial.println(F("  PING"));
  Serial.println(F("  SET SLEEP_DISABLED=<0|1>"));
  Serial.println(F("  SET SLEEP=<ENABLE|DISABLE>"));
}

void handleSerial() {
  while (Serial.available()) {
    char c = (char)Serial.read();
    if (c == '\r') continue;

    if (c != '\n') {
      if ((int)s_input.length() < SERIAL_LINE_MAX) {
        s_input += c;
      } else {
        // Still read & ignore until EOL to resync
      }
      continue;
    }

    // got a line
    String line = s_input; s_input = "";
    line.trim(); if (!line.length()) continue;
    if ((int)line.length() >= SERIAL_LINE_MAX) {
      Serial.println("ERR Line too long");
      continue;
    }

    // split first token
    int sp = line.indexOf(' ');
    String cmd  = (sp >= 0) ? line.substring(0, sp) : line;
    String args = (sp >= 0) ? line.substring(sp + 1)  : String();
    cmd.toUpperCase();

    // ------------- PING -------------
    if (cmd == "PING") { Serial.println("PONG"); continue; }

    // ------------- HELP -------------
    if (cmd == "HELP") { cmd_HELP(); continue; }

    // ------------- GET -------------
    if (cmd == "GET") {
      args.trim(); args.toUpperCase();
      if (args == "STATUS") { printStatusJson(); }
      else if (args == "DUMP") { printBadgeDumpJson(); }
      else { Serial.println("ERR Unknown GET field"); }
      continue;
    }

    // ------------- UNLOCK CHARM=K -------------
    if (cmd == "UNLOCK") {
      String a = args; a.trim(); a.toUpperCase();
      if (a.startsWith("CHARM=")) {
        int k = a.substring(6).toInt();
        if (k >= 0 && k < 32) {
          CFG.unlockedMask |= (1UL << k);
          saveConfig(CFG);
          Serial.printf("OK UNLOCK CHARM %d\n", k);
        } else Serial.println("ERR CHARM index 0..31");
      } else Serial.println("ERR Unknown UNLOCK field");
      continue;
    }

    // ------------- MSG ADD / MSG DEL (stubs) -------------
    if (cmd == "MSG") {
      String a = args; a.trim();
      if (a.startsWith("ADD ")) {
        // format: MSG ADD <id> <text...>
        int sp1 = a.indexOf(' ', 4);
        if (sp1 > 4) {
          int id = a.substring(4, sp1).toInt();
          String text = a.substring(sp1 + 1);
          text.trim();
          // TODO: persist if desired
          Serial.printf("OK MSG ADD id=%d len=%d\n", id, (int)text.length());
        } else Serial.println("ERR MSG ADD syntax");
      } else if (a.startsWith("DEL ")) {
        int id = a.substring(4).toInt();
        // TODO: delete if persisted
        Serial.printf("OK MSG DEL id=%d\n", id);
      } else {
        Serial.println("ERR MSG syntax");
      }
      continue;
    }

    // ------------- SET ... -------------
    if (cmd == "SET") {
      String a = args; a.trim();

      // SET DUMP={json}
      if (a.startsWith("DUMP=")) {
        String js = a.substring(5); js.trim();
        if (!js.length()) Serial.println("ERR DUMP requires JSON");
        else Serial.println(applyBadgeDumpJson(js.c_str(), js.length()) ? "OK DUMP restored" : "ERR DUMP failed");
        continue;
      }

      // normalize for key tests
      String AU = a; AU.toUpperCase();

      // SET NAME=...
      if (AU.startsWith("NAME=")) {
        String v = a.substring(5); v.trim();
        CFG.name = v;
        saveConfig(CFG);
        startScroll(/*idle*/true, 0, 0, 0);   // restart idle scroller
        Serial.printf("OK NAME -> \"%s\"\n", CFG.name.c_str());
        continue;
      }

      // SET ID=...
      if (AU.startsWith("ID=")) {
        int id = a.substring(3).toInt();
        id = constrain(id, 0, 1023);
        CFG.id = (uint16_t)id;
        saveConfig(CFG);
        Serial.printf("OK ID -> %d\n", id);
        continue;
      }

      // SET COLOR=...
      if (AU.startsWith("COLOR=")) {
        String v = a.substring(6); v.trim();
        uint32_t rgb;
        if (parseRgbTriplet(v, rgb)) {
          CFG.colorRGB = rgb; saveConfig(CFG);
          Serial.printf("OK COLOR -> #%06lX\n", (unsigned long)rgb);
        } else Serial.println("ERR COLOR format (#RRGGBB or R,G,B)");
        continue;
      }

      // SET BRIGHTNESS=...
      if (AU.startsWith("BRIGHTNESS=")) {
        int b = a.substring(11).toInt();
        b = constrain(b, 0, 128);
        CFG.brightness = (uint8_t)b;
        FastLED.setBrightness(map128to255(CFG.brightness));
        FastLED.show();
        saveConfig(CFG);
        Serial.printf("OK BRIGHTNESS -> %d\n", b);
        continue;
      }

      // SET USERCHARM=<0..31> or -1 to clear
      if (AU.startsWith("USERCHARM=")) {
        int v = a.substring(10).toInt();
        if (v < 0) {
          CFG.userCharmId = 0xFF;  // none
          saveConfig(CFG);
          startScroll(/*idle*/true, 0, 0, 0);
          Serial.println("OK USERCHARM cleared");
        } else if (v >= 0 && v < 32) {
          CFG.userCharmId = (uint8_t)v;
          saveConfig(CFG);
          startScroll(/*idle*/true, 0, 0, 0);
          Serial.printf("OK USERCHARM -> %d\n", v);
        } else {
          Serial.println("ERR USERCHARM range 0..31 or -1");
        }
        continue;
      }
      if (AU.startsWith("SLEEP_DISABLED=")) {
          int v = a.substring(15).toInt();
          CFG.sleepDisabled = (v != 0);
          saveConfig(CFG);
          Serial.printf("OK SLEEP_DISABLED -> %d\n", (int)CFG.sleepDisabled);
          continue;
        }

        // Friendly alias: SET SLEEP=ENABLE|DISABLE
        if (AU.startsWith("SLEEP=")) {
          String v = a.substring(6); v.trim(); v.toUpperCase();
          if (v == "ENABLE") {
            CFG.sleepDisabled = false;
          } else if (v == "DISABLE") {
            CFG.sleepDisabled = true;
          } else {
            Serial.println("ERR SLEEP expects ENABLE or DISABLE");
            continue;
          }
          saveConfig(CFG);
          Serial.printf("OK SLEEP -> %s\n", CFG.sleepDisabled ? "DISABLED" : "ENABLED");
          continue;
        }
      // SET LOCK CHARM=K
      if (AU.startsWith("LOCK CHARM=")) {
        int k = a.substring(11).toInt();
        if (k >= 0 && k < 32) {
          CFG.unlockedMask &= ~(1UL << k);
          saveConfig(CFG);
          Serial.printf("OK LOCK CHARM %d\n", k);
        } else Serial.println("ERR CHARM index 0..31");
        continue;
      }

      Serial.println("ERR Unknown SET field");
      continue;
    }

    // ------------- SHOW (legacy) -------------
    if (cmd == "SHOW") { printStatusJson(); continue; }

    // ------------- default -------------
    Serial.println("ERR Unknown command. Type HELP.");
  }
}