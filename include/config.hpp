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

// --- SCORE response throttle (default 30s) ---
#ifndef SCORE_RESP_MIN_MS
#define SCORE_RESP_MIN_MS 30000UL   // change via -D SCORE_RESP_MIN_MS=5000, etc.
#endif

// Optional: small jitter to avoid clumping after the throttle window
#ifndef SCORE_RESP_JITTER_MS
#define SCORE_RESP_JITTER_MS 15UL
#endif



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

#ifndef SLEEP_BRIGHT_MIN_128
#define SLEEP_BRIGHT_MIN_128 1         // min brightness (0..128 scale)
#endif
#ifndef SLEEP_BRIGHT_MAX_128
#define SLEEP_BRIGHT_MAX_128 4       // max brightness (0..128 scale)
#endif

// ---- Choose which scene+frame to show while asleep ----
#ifndef SLEEP_SCENE_ID
#define SLEEP_SCENE_ID 3     // you said "ID 2" (e.g., fuse)
#endif

#ifndef SLEEP_FRAME_INDEX
#define SLEEP_FRAME_INDEX 2  // which frame from that scene
#endif


static uint16_t g_batt_mV   = 0;  // last computed battery millivolts
static uint16_t g_batt_raw  = 0;  // last averaged raw ADC counts (0..4095)
static uint32_t g_battLastMs = 0;

// --- Battery sense (GPIO26 / ADC0) -----------------------------------------
#ifndef BATTERY_ADC_PIN
#define BATTERY_ADC_PIN A0            // RP2040 ADC0 is GPIO26
#endif
#ifndef ADC_FULL_SCALE_VREF
#define ADC_FULL_SCALE_VREF 3.30f     // adjust if you calibrate later
#endif
#ifndef BATTERY_DIVIDER_K
#define BATTERY_DIVIDER_K 0.357f      // Vadc = K * Vbatt  (so Vbatt = Vadc / K)
#endif
#ifndef BATT_SAMPLES
#define BATT_SAMPLES 8
#endif
#ifndef BATT_UPDATE_MS
#define BATT_UPDATE_MS 2000UL
#endif
// Optional: set to 0 to disable warning for now (we don’t know the right value yet)
#ifndef LOW_BATT_MV
#define LOW_BATT_MV 0                 // e.g., 2300 for 2.3V, TBD
#endif

// ---- Low-battery GPIO (drives a transistor/LED/etc.) ----
#ifndef LOW_BATT_PIN
#define LOW_BATT_PIN 11          // GPIO11
#endif

// Thresholds in millivolts (tweak later when we know the real number)
#ifndef LOW_BATT_THRESH_MV
#define LOW_BATT_THRESH_MV 2200  // default ~2.4 V pack
#endif
#ifndef LOW_BATT_HYST_MV
#define LOW_BATT_HYST_MV 100     // 0.10 V hysteresis
#endif

#ifndef BATT_PRESENT_MIN_MV
#define BATT_PRESENT_MIN_MV 1200  // 0.1 V floor
#endif

// If your external circuit is active-low, set to 0
#ifndef LOW_BATT_ACTIVE_HIGH
#define LOW_BATT_ACTIVE_HIGH 1
#endif
#if LOW_BATT_ACTIVE_HIGH
  #define LOW_BATT_ON  HIGH
  #define LOW_BATT_OFF LOW
#else
  #define LOW_BATT_ON  LOW
  #define LOW_BATT_OFF HIGH
#endif

extern uint16_t g_batt_mV;   // current pack mV
static bool g_lowBatt = false; // debounced state

static uint32_t g_sleepBreathLast = 0;

static uint32_t g_sleepPulseLast = 0;



