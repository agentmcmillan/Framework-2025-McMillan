#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>

// ---- Pins (adjust to your board) ----
#define PIXEL_PIN       4
#define IR_RX_PIN       27
#define MEOW            23
#define PURR            24

#define BTN1 5
#define BTN2 6
#define BTN3 7

#define WIDTH  15
#define HEIGHT 7
#define NUM_LEDS (WIDTH*HEIGHT)
#define COLOR_ORDER GRB
#define CHIPSET WS2812

static int16_t nsX = WIDTH;   // scroller X


struct BadgeConfig {
  String   name;           // display name
  uint16_t id;             // 0..1023
  uint32_t colorRGB;       // 0xRRGGBB
  uint8_t  brightness;     // 0..128 (mapped to 0..255)
  uint32_t unlockedMask;   // 32 charms
  uint32_t score;          // total hits received
  uint32_t sentCount;      // total fires sent
  uint8_t  userCharmId;    // 0..31, 0xFF = none

};

bool loadConfig(BadgeConfig &out);
bool saveConfig(const BadgeConfig &in);

bool loadHits(JsonDocument &doc);      // map "attackerId" -> count
bool saveHits(const JsonDocument &doc);


