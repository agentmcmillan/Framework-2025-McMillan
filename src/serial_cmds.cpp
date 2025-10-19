#include <Arduino.h>
#include <LittleFS.h>
#include "serial_cmds.hpp"
#include "protocol.h"

// Small line buffer
static String inLine;



static uint32_t parseHex(const String &s){
  uint32_t v = 0;
  for (char c : s) {
    uint8_t d;
    if      (c>='0' && c<='9') d = c - '0';
    else if (c>='a' && c<='f') d = 10 + (c - 'a');
    else if (c>='A' && c<='F') d = 10 + (c - 'A');
    else break;
    v = (v<<4) | d;
  }
  return v;
}

static void printHelp(){
  Serial.println(F("Commands:"));
  Serial.println(F("  SET NAME <text>"));
  Serial.println(F("  SET ID <0..1023>"));
  Serial.println(F("  SET COLOR <RRGGBB>"));
  Serial.println(F("  SET BRIGHT <0..128>"));
  Serial.println(F("  UNLOCK <0..31>     / LOCK <0..31>"));
  Serial.println(F("  UNLOCK_MASK <hex28>"));
  Serial.println(F("  SAVE / LOAD / DUMP"));
  Serial.println(F("  SLEEP <minutes> / WAKE"));
  Serial.println(F("  MSG SHOW <id> <scrolls>   (stub)"));
  Serial.println(F("  HELP"));
}

void handleSerial(){
  while (Serial.available()){
    char c = Serial.read();
    if (c == '\r') continue;
    if (c != '\n') { inLine += c; if (inLine.length() > 256) inLine.remove(0, inLine.length()-256); continue; }

    // process a full line
    String s = inLine; inLine = "";
    s.trim();
    if (s.length() == 0) continue;

    // tokenize
    s.replace('\t',' ');
    while (s.indexOf("  ") >= 0) s.replace("  "," ");
    int sp = s.indexOf(' ');
    String cmd = (sp<0) ? s : s.substring(0, sp);
    String rest = (sp<0) ? "" : s.substring(sp+1);

    cmd.toUpperCase();

    if (cmd == "HELP" || cmd == "?") { printHelp(); continue; }

    if (cmd == "SET") {
      int sp2 = rest.indexOf(' ');
      if (sp2 < 0) { Serial.println(F("ERR: SET what? NAME/ID/COLOR/BRIGHT")); continue; }
      String field = rest.substring(0, sp2); field.toUpperCase();
      String val   = rest.substring(sp2+1);

      if (field == "NAME") {
        CFG.name = val; saveConfig(CFG);
        Serial.println(F("OK: name saved"));
      } else if (field == "ID") {
        uint16_t id = (uint16_t)val.toInt();
        if (id > 1023) id = 1023;
        CFG.id = id; saveConfig(CFG);
        Serial.println(F("OK: id saved"));
      } else if (field == "COLOR") {
        val.trim(); if (val.startsWith("#")) val.remove(0,1);
        CFG.colorRGB = parseHex(val);
        saveConfig(CFG);
        Serial.println(F("OK: color saved"));
      } else if (field == "BRIGHT") {
        int b = val.toInt(); if (b<0) b=0; if (b>128) b=128;
        CFG.brightness = (uint8_t)b;
        saveConfig(CFG);
        Serial.println(F("OK: brightness saved"));
      } else {
        Serial.println(F("ERR: unknown field"));
      }
      continue;
    }

    if (cmd == "UNLOCK" || cmd == "LOCK") {
      int idx = rest.toInt();
      if (idx < 0 || idx > 31) { Serial.println(F("ERR: index 0..31")); continue; }
      if (cmd == "UNLOCK") CFG.unlockedMask |=  (1UL << idx);
      else                 CFG.unlockedMask &= ~(1UL << idx);
      saveConfig(CFG);
      Serial.println(F("OK"));
      continue;
    }

    if (cmd == "UNLOCK_MASK") {
      rest.trim(); if (rest.startsWith("0x")||rest.startsWith("0X")) rest.remove(0,2);
      CFG.unlockedMask = parseHex(rest) & 0x0FFFFFFFul; // up to 28 bits used
      saveConfig(CFG);
      Serial.println(F("OK"));
      continue;
    }

    if (cmd == "SAVE") { saveConfig(CFG); saveHits(hitsDoc); Serial.println(F("OK: saved")); continue; }
    if (cmd == "LOAD") { loadConfig(CFG); loadHits(hitsDoc); Serial.println(F("OK: loaded")); continue; }

    if (cmd == "DUMP") {
      Serial.println(F("{"));
      Serial.printf("  \"name\":\"%s\",\n", CFG.name.c_str());
      Serial.printf("  \"id\":%u,\n", CFG.id);
      Serial.printf("  \"colorRGB\":%lu,\n", (unsigned long)CFG.colorRGB);
      Serial.printf("  \"brightness\":%u,\n", CFG.brightness);
      Serial.printf("  \"unlockedMask\":%lu,\n", (unsigned long)CFG.unlockedMask);
      Serial.printf("  \"score\":%lu,\n", (unsigned long)CFG.score);
      Serial.printf("  \"sentCount\":%lu\n", (unsigned long)CFG.sentCount);
      Serial.println(F("}"));
      continue;
    }

    if (cmd == "SLEEP") {
      uint8_t m = (uint8_t)rest.toInt();
      // signal to main loop via serial? not needed—IR path already exists. But allow local sleep:
      extern void enterSleepForMinutes(uint8_t minutes);
      enterSleepForMinutes(m);
      Serial.println(F("OK: sleeping"));
      continue;
    }

    if (cmd == "WAKE") {
      extern void wakeNow();
      wakeNow();
      Serial.println(F("OK: awake"));
      continue;
    }

    if (cmd == "MSG") {
      Serial.println(F("TODO: messages bank not implemented yet"));
      continue;
    }

    Serial.println(F("ERR: unknown command; type HELP"));
  }
}
