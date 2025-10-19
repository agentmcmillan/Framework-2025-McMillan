#include <Arduino.h>
#include <FastLED.h>
#include <FastLED_NeoMatrix.h>
#include <Adafruit_GFX.h>
#include <IRremote.hpp>     // capture only
#include <LittleFS.h>
#include <ArduinoJson.h>

#include "protocol_sirc20.h"

// ===== Pins (your spec) =====
#define PIXEL_PIN 4
#define IR_RECEIVE_PIN 27
#define IR_TX 3
#define BUILTIN_LED 11
#define MEOW 23
#define PURR 24

#define BTN1 5
#define BTN2 6
#define BTN3 7

// ===== Matrix =====
#define WIDTH 15
#define HEIGHT 7
#define NUM_LEDS (WIDTH*HEIGHT)
#define COLOR_ORDER GRB
#define CHIPSET WS2812

CRGB leds[NUM_LEDS];
FastLED_NeoMatrix *matrix = new FastLED_NeoMatrix(
  leds, WIDTH, HEIGHT,
  NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE
);

// ===== State / Config (very small stub for now) =====
struct BadgeConfig {
  char     name[24] = "Badge";
  uint16_t id       = 0;          // 0..511
  uint8_t  brightness = 40;       // 0..128
  uint32_t colorRGB   = 0x00FF00; // default green
  uint32_t unlockedMask = 0;      // 32 charms
  uint32_t score = 0;
} CFG;

static void saveConfig() {
  StaticJsonDocument<256> doc;
  doc["name"] = CFG.name;
  doc["id"]   = CFG.id;
  doc["b"]    = CFG.brightness;
  doc["c"]    = CFG.colorRGB;
  doc["um"]   = CFG.unlockedMask;
  doc["sc"]   = CFG.score;
  File f = LittleFS.open("/cfg.json", "w");
  if (f) serializeJson(doc, f);
  f.close();
}
static void loadConfig() {
  File f = LittleFS.open("/cfg.json", "r");
  if (!f) return;
  StaticJsonDocument<256> doc;
  DeserializationError e = deserializeJson(doc, f);
  f.close();
  if (e) return;
  strlcpy(CFG.name, doc["name"] | "Badge", sizeof(CFG.name));
  CFG.id = doc["id"] | 0;
  CFG.brightness = doc["b"] | 40;
  CFG.colorRGB = doc["c"] | 0x00FF00;
  CFG.unlockedMask = doc["um"] | 0;
  CFG.score = doc["sc"] | 0;
}

// ===== Simple UI helpers =====
static inline uint16_t color565(uint32_t rgb) {
  uint8_t r=(rgb>>16)&0xFF, g=(rgb>>8)&0xFF, b=rgb&0xFF;
  return matrix->Color(r,g,b);
}
static void drawLineOnce(const String &s, uint16_t c) {
  matrix->setFont(NULL);
  matrix->setTextSize(1);
  matrix->setTextWrap(false);
  int16_t x1,y1; uint16_t w,h;
  matrix->getTextBounds(s.c_str(),0,0,&x1,&y1,&w,&h);
  for (int16_t x=WIDTH; x>-(int)w; --x){
    matrix->fillScreen(0);
    matrix->setTextColor(c);
    matrix->setCursor(x,0);
    matrix->print(s);
    FastLED.show();
    delay(60);
  }
}

// ===== Game events =====
static void recordHit(uint16_t attacker, uint8_t charm) {
  CFG.score++;
  saveConfig();
  // FX: quick flash + tones
  fill_solid(leds, NUM_LEDS, CRGB::Red); FastLED.show();
  tone(MEOW, 1200, 80); tone(PURR, 140, 100);
  delay(120);
  FastLED.clear(true);

  Serial.printf("[HIT] from=%u charm=%u score=%lu\n", attacker, charm, (unsigned long)CFG.score);
}

static void unlockCharm(uint8_t c) {
  if (c < 32) {
    if ((CFG.unlockedMask & (1UL<<c)) == 0) {
      CFG.unlockedMask |= (1UL<<c);
      saveConfig();
      Serial.printf("[UNLOCK] charm %u (mask=0x%08lX)\n", c, (unsigned long)CFG.unlockedMask);
    }
  }
}

static void applyBrightness(uint8_t b128) {
  if (b128 > 128) b128 = 128;
  uint8_t b255 = (uint16_t(b128) * 255u) / 128u;
  FastLED.setBrightness(b255);
  FastLED.show();
  CFG.brightness = b128;
  saveConfig();
}

// ===== SIRC-20 tolerant raw decode =====
// IRremote gives us raw->rawbuf[] in ticks. Sony uses ~600us unit.
// Start: 2400us mark + 600us space
// Bit: 600us mark + space(0=600us, 1=1200us), LSB-first.
static inline bool near(uint32_t v, uint32_t ref, uint8_t pct) {
  uint32_t lo = (ref * (100 - pct)) / 100, hi = (ref * (100 + pct)) / 100;
  return v >= lo && v <= hi;
}

// ===== SIRC-20 tolerant raw decode (RP2040-safe) =====
// raw->rawbuf is IRRawbufType* (byte ticks) on this core.
static inline bool near_us(uint32_t v, uint32_t ref, uint8_t pct) {
  uint32_t lo = (ref * (100 - pct)) / 100, hi = (ref * (100 + pct)) / 100;
  return v >= lo && v <= hi;
}

static bool sirc20Decode(uint16_t &outLSBfirst) {
  if (!IrReceiver.decode()) return false;
  auto *raw = IrReceiver.decodedIRData.rawDataPtr;
  Serial.println("msg");
  if (!raw || raw->rawlen < (2 + 2 * 20)) { IrReceiver.resume(); return false; }

  // NOTE: IRRawbufType is uint8_t on RP2040 build of IRremote
  const IRRawbufType *buf = raw->rawbuf;
  const uint16_t n = raw->rawlen;

  // Header: buf[1] mark, buf[2] space (buf[0] is the leading gap)
  uint32_t hdrMark  = (uint32_t)buf[1] * MICROS_PER_TICK;
  uint32_t hdrSpace = (uint32_t)buf[2] * MICROS_PER_TICK;
  if (!near_us(hdrMark, 2400, 30) || !near_us(hdrSpace, 600, 35)) {
    IrReceiver.resume(); 
    return false;
  }

  // Read 20 bits, LSB-first per Sony
  uint16_t bits = 0;
  int idx = 3;
  for (uint8_t i = 0; i < 20; i++) {
    if (idx + 1 >= n) { IrReceiver.resume(); return false; }

    uint32_t mark  = (uint32_t)buf[idx++] * MICROS_PER_TICK;
    uint32_t space = (uint32_t)buf[idx++] * MICROS_PER_TICK;

    if (!near_us(mark, 600, 40)) { IrReceiver.resume(); return false; }

    bool isOne  = near_us(space, 1200, 40);
    bool isZero = near_us(space,  600, 40);
    if (!isOne && !isZero) { IrReceiver.resume(); return false; }

    if (isOne) bits |= (1u << i); // LSB-first
  }

  IrReceiver.resume();
  outLSBfirst = bits; // 20-bit SIRC word (LSB-first already)
  return true;
}


// ===== IR handling =====
static void handleIR() {
  uint16_t wLSB;
  if (!sirc20Decode(wLSB)) return;

  uint8_t  op      = SIRC20::getOp(wLSB);
  uint16_t value14 = SIRC20::getValue14(wLSB);

  switch(op) {
    case SIRC20::FIRE: {
      uint16_t attacker; uint8_t charm;
      SIRC20::splitFire(wLSB, attacker, charm);
      // 1) record hit
      recordHit(attacker, charm);
      // 2) optional unlock (charm != 0)
      if (charm != 0) unlockCharm(charm);
      Serial.printf("[IR] FIRE attacker=%u charm=%u word=0x%05X\n", attacker, charm, wLSB);
    } break;

    case SIRC20::UNLOCK_CHARM: {
      uint8_t charm = value14 & 0x1F;
      if (charm != 0) unlockCharm(charm);
      Serial.printf("[IR] UNLOCK_CHARM %u\n", charm);
    } break;

    case SIRC20::SLEEP: {
      uint8_t minutes = value14 & 0x7F;
      Serial.printf("[IR] SLEEP %u\n", minutes);
      // TODO: implement your sleep mode visuals/timer
    } break;

    case SIRC20::WAKE:
      Serial.println("[IR] WAKE");
      // TODO: implement wake
      break;

    case SIRC20::SET_BRIGHTNESS: {
      uint8_t b = value14 & 0x7F;
      applyBrightness(b);
      Serial.printf("[IR] SET_BRIGHTNESS %u\n", b);
    } break;

    case SIRC20::SHOW_MESSAGE: {
      uint8_t scrolls   =  value14 & 0x7F;
      uint8_t messageId = (value14 >> 7) & 0x7F;
      Serial.printf("[IR] SHOW_MESSAGE id=%u scrolls=%u\n", messageId, scrolls);
      // TODO: queue scroll
    } break;

    case SIRC20::SPECIAL_SCENE: {
      uint8_t sid = value14 & 0x7F;
      Serial.printf("[IR] SPECIAL_SCENE %u\n", sid);
      // TODO: play scene
    } break;

    case SIRC20::DUMP_STATS:
      Serial.println("[IR] DUMP_STATS (TBD)");
      break;

    default:
      Serial.printf("[IR] Unknown op=0x%02X w=0x%05X\n", op, wLSB);
      break;
  }
}

void setup() {
  pinMode(BTN1, INPUT_PULLUP);
  pinMode(BTN2, INPUT_PULLUP);
  pinMode(BTN3, INPUT_PULLUP);
  pinMode(MEOW, OUTPUT);
  pinMode(PURR, OUTPUT);

  Serial.begin(115200);

  LittleFS.begin();
  loadConfig();

  FastLED.addLeds<CHIPSET, PIXEL_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 1000);
  FastLED.setBrightness((uint16_t(CFG.brightness)*255u)/128u);
  FastLED.clear(true);

  // Simple boot splash
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  for (int i=0; i<NUM_LEDS; i+=2) leds[i]=CRGB::Yellow;
  FastLED.show();
  delay(1000);
  FastLED.clear(true);

  // IR receive (we only need raw capture)
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
  Serial.printf("[IR] Receiver ready on pin %d\n", IR_RECEIVE_PIN);
}

void loop() {
  handleIR();

  // simple idle scroll of name + charms
  static uint32_t last=0;
  if (millis()-last > 20) {
    last = millis();
    static int16_t x = WIDTH;
    String s = String(CFG.name) + "  ";
    for (uint8_t i=0;i<32;i++) if (CFG.unlockedMask & (1UL<<i)) s += char('0'+(i%10));
    matrix->fillScreen(0);
    matrix->setFont(NULL);
    matrix->setTextSize(1);
    matrix->setTextWrap(false);
    matrix->setTextColor(color565(CFG.colorRGB));
    matrix->setCursor(x,0);
    matrix->print(s);
    FastLED.show();
    x--; int16_t w = s.length()*6;
    if (x < -w) x = WIDTH;
  }
}
