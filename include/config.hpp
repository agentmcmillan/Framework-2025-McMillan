#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>

struct BadgeConfig {
  String   name;           // display name
  uint16_t id;             // 0..1023
  uint32_t colorRGB;       // 0xRRGGBB
  uint8_t  brightness;     // 0..128 (mapped to 0..255)
  uint32_t unlockedMask;   // 32 charms
  uint32_t score;          // total hits received
  uint32_t sentCount;      // total fires sent
};

bool loadConfig(BadgeConfig &out);
bool saveConfig(const BadgeConfig &in);

bool loadHits(JsonDocument &doc);      // map "attackerId" -> count
bool saveHits(const JsonDocument &doc);
