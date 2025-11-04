#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>


#ifndef FIRE_MIN_MS
#define FIRE_MIN_MS 5000UL   // 5 seconds between local FIREs
#endif
static uint32_t g_lastFireBtnMs = 0;   // last time we allowed a local FIRE


// ---- Sleep/Wake ----
#ifndef AUTO_SLEEP_MINUTES_DEFAULT
#define AUTO_SLEEP_MINUTES_DEFAULT 15   // auto-sleep after 30 min of inactivity
#endif




// ---------------- Pins / Matrix ----------------
#define PIXEL_PIN   4
#define IR_RX_PIN   27
#define IR_TX_PIN   3
#define MEOW        23
#define PURR        24

#define BTN1 5
#define BTN2 6
#define BTN3 7

#define WIDTH  15
#define HEIGHT 7
#define NUM_LEDS (WIDTH*HEIGHT)

#define COLOR_ORDER GRB
#define CHIPSET WS2812

// Use the same struct the generator writes:
//#ifndef SCN_W
#define SCN_W 15
#define SCN_H 7
#define SCN_PIX (SCN_W*SCN_H)

// spacing between text and first icon, and between icons
#define TEXT_ICON_GAP  3
#define ICON_SPACING   1


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
  bool     sleepDisabled; 

};

bool loadConfig(BadgeConfig &out);
bool saveConfig(const BadgeConfig &in);

bool loadHits(JsonDocument &doc);      // map "attackerId" -> count
bool saveHits(const JsonDocument &doc);


// ---- Sleep pulse config ----
#ifndef SLEEP_PULSE_MS
#define SLEEP_PULSE_MS 16UL          // update cadence (~60 Hz)
#endif
#ifndef SLEEP_PULSE_PERIOD_MS
#define SLEEP_PULSE_PERIOD_MS 4000UL // full breathe cycle
#endif
#ifndef SLEEP_PULSE_BRIGHT_MIN
#define SLEEP_PULSE_BRIGHT_MIN 1     // 0..255
#endif
#ifndef SLEEP_PULSE_BRIGHT_MAX
#define SLEEP_PULSE_BRIGHT_MAX 4    // keep tiny to save power
#endif
#ifndef SLEEP_PULSE_X
#define SLEEP_PULSE_X 0              // top-left corner (x,y)
#endif
#ifndef SLEEP_PULSE_Y
#define SLEEP_PULSE_Y 0
#endif

#ifndef SLEEP_BRIGHT_MIN_128
#define SLEEP_BRIGHT_MIN_128 1         // min brightness (0..128 scale)
#endif
#ifndef SLEEP_BRIGHT_MAX_128
#define SLEEP_BRIGHT_MAX_128 4       // max brightness (0..128 scale)
#endif

static uint32_t g_sleepBreathLast = 0;

static uint32_t g_sleepPulseLast = 0;



