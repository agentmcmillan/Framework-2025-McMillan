// ======================= main.cpp (RP2040 badge) =======================
#include <Arduino.h>
#include <FastLED.h>
#include <FastLED_NeoMatrix.h>
#include <Adafruit_GFX.h>
#include <IRremote.hpp>
#include <LittleFS.h>
#include <ArduinoJson.h>

// project headers you already have
#include "config.hpp"          // BadgeConfig CFG; loadConfig/saveConfig
#include "serial_cmds.hpp"     // void handleSerial();
#include "protocol.h"          // just the op codes if you keep them
#include "charms565.hpp"
#include "scene_nyan.hpp"
//#include "scenes.hpp"

// ---------------- Pins / Matrix ----------------
#define PIXEL_PIN   4
#define IR_RX_PIN   27
#define IR_TX_PIN   3
#define MEOW        23
#define PURR        24

#define WIDTH   15
#define HEIGHT   7
#define NUM_LEDS (WIDTH*HEIGHT)

#define CHIPSET     WS2812
#define COLOR_ORDER GRB

// Use the same struct the generator writes:
//#ifndef SCN_W
#define SCN_W 15
#define SCN_H 7
#define SCN_PIX (SCN_W*SCN_H)

// spacing between text and first icon, and between icons
#define TEXT_ICON_GAP  3
#define ICON_SPACING   1


//#endif

static uint32_t btnLastMs = 0;
static bool prevB1 = true, prevB2 = true, prevB3 = true;

static const char BUILD_DATE[] = __DATE__;
static const char BUILD_TIME[] = __TIME__;

CRGB leds[NUM_LEDS];

// --- Fast blit: RGB565 (PROGMEM) -> FastLED CRGB in-place ---
static inline void blit565ToLeds(const uint16_t *frame565) {
  for (uint16_t i = 0; i < SCN_PIX; ++i) {
    uint16_t p = pgm_read_word(&frame565[i]);     // read from flash
    // unpack 5/6/5 to 8/8/8
    uint8_t r = ((p >> 11) & 0x1F) * 255 / 31;
    uint8_t g = ((p >> 5)  & 0x3F) * 255 / 63;
    uint8_t b = ( p        & 0x1F) * 255 / 31;
    leds[i] = CRGB(r, g, b);
  }
}



FastLED_NeoMatrix* matrix = new FastLED_NeoMatrix(
  leds, WIDTH, HEIGHT,
  NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE
);

// ---------------- Config / state ----------------
BadgeConfig CFG;

// ================== Scoring state + helpers ==================
struct ScoreState {
  uint32_t score = 0;          // running total
  uint32_t hits_total = 0;     // any FIRE seen
  uint32_t fires_total = 0;    // local button-2 fires
  uint32_t sessions_woke = 0;  // WAKE events seen
  uint32_t scenes_triggered = 0;

  uint32_t rolls_5 = 0, rolls_10 = 0, rolls_20 = 0, rolls_40 = 0; // audit
} g_score;

// persistent meta for throttled saves
static bool     g_scoreDirty = false;
static uint32_t g_scoreLastSave = 0;

// Simple, stable RNG roll in [1..sides]
static uint16_t scoreRoll(uint8_t sides) {
  if (sides < 1) sides = 1;
  // randomSeed should be set in setup(); this is inclusive
  return (uint16_t)random(1, (long)sides + 1);
}

static void scoreAddInternal(uint32_t points, const char* why) {
  g_score.score += points;
  g_scoreDirty = true;
  if (why) Serial.printf("[SCORE] +%lu (%s) → %lu\n", (unsigned long)points, why, (unsigned long)g_score.score);
}

// Award “roll X”: add a random 1..X
static void scoreAwardRoll(uint8_t sides, const char* why) {
  uint16_t add = scoreRoll(sides);
  switch (sides) {
    case 5:  g_score.rolls_5++;  break;
    case 10: g_score.rolls_10++; break;
    case 20: g_score.rolls_20++; break;
    case 40: g_score.rolls_40++; break;
  }
  scoreAddInternal(add, why);
}

// Public helpers (used by serial_cmds.cpp too)
uint32_t scoreGet()            { return g_score.score; }
void     scoreSet(uint32_t v)  { g_score.score = v; g_scoreDirty = true; Serial.printf("[SCORE] SET → %lu\n", (unsigned long)v); }
void     scoreAdd(int32_t d)   { if (d >= 0) scoreAddInternal((uint32_t)d, "serial add"); else {
                                   uint32_t sub = (uint32_t)(-d);
                                   g_score.score = (g_score.score > sub) ? (g_score.score - sub) : 0;
                                   g_scoreDirty = true;
                                   Serial.printf("[SCORE] ADD %ld → %lu\n", (long)d, (unsigned long)g_score.score);
                                 } }

// Optional: persist score counters (keep raw stats for long-term download)
static bool scoreSave() {
  File f = LittleFS.open("/score.json", "w");
  if (!f) return false;
  StaticJsonDocument<384> doc;
  doc["score"]            = g_score.score;
  doc["hits_total"]       = g_score.hits_total;
  doc["fires_total"]      = g_score.fires_total;
  doc["sessions_woke"]    = g_score.sessions_woke;
  doc["scenes_triggered"] = g_score.scenes_triggered;
  doc["r5"]  = g_score.rolls_5;
  doc["r10"] = g_score.rolls_10;
  doc["r20"] = g_score.rolls_20;
  doc["r40"] = g_score.rolls_40;
  serializeJson(doc, f);
  f.close();
  g_scoreDirty = false;
  g_scoreLastSave = millis();
  return true;
}
static void scoreLoad() {
  File f = LittleFS.open("/score.json", "r");
  if (!f) return;
  StaticJsonDocument<384> doc;
  if (deserializeJson(doc, f) == DeserializationError::Ok) {
    g_score.score            = doc["score"]           | 0u;
    g_score.hits_total       = doc["hits_total"]      | 0u;
    g_score.fires_total      = doc["fires_total"]     | 0u;
    g_score.sessions_woke    = doc["sessions_woke"]   | 0u;
    g_score.scenes_triggered = doc["scenes_triggered"]| 0u;
    g_score.rolls_5  = doc["r5"]  | 0u;
    g_score.rolls_10 = doc["r10"] | 0u;
    g_score.rolls_20 = doc["r20"] | 0u;
    g_score.rolls_40 = doc["r40"] | 0u;
  }
  f.close();
}

// Call this in loop() to avoid flash spam
static void scoreMaybeAutoSave() {
  if (!g_scoreDirty) return;
  if (millis() - g_scoreLastSave >= 3000) (void)scoreSave();
}


// --- SCORE response throttle (default 30s) ---
#ifndef SCORE_RESP_MIN_MS
#define SCORE_RESP_MIN_MS 30000UL   // change via -D SCORE_RESP_MIN_MS=5000, etc.
#endif

// Optional: small jitter to avoid clumping after the throttle window
#ifndef SCORE_RESP_JITTER_MS
#define SCORE_RESP_JITTER_MS 15UL
#endif

static uint32_t g_lastScoreRespMs = 0;

static inline bool scoreThrottleOk() {
  if (SCORE_RESP_MIN_MS == 0) return true;                 // disabled
  uint32_t now = millis();
  // unsigned subtraction is rollover-safe
  if ((now - g_lastScoreRespMs) < SCORE_RESP_MIN_MS) return false;
  g_lastScoreRespMs = now;
  return true;
}


// exact 0..128 -> 0..255
uint8_t map128to255(uint8_t v){ return (uint16_t(v)*255u)/128u; }
static inline uint16_t badgeColor565() {
  uint8_t r=(CFG.colorRGB>>16)&0xFF, g=(CFG.colorRGB>>8)&0xFF, b=CFG.colorRGB&0xFF;
  return matrix->Color(r,g,b);
}

// ---- Unique attacker set (512 bits) + persistence ----
static uint32_t g_attackerBits[16]; // 16 * 32 = 512 ids

static inline bool statsAddAttacker(uint16_t id) {
  if (id >= 512) return false;
  uint8_t w = id >> 5;                 // 0..15
  uint32_t m = 1u << (id & 31);
  bool was = (g_attackerBits[w] & m);
  g_attackerBits[w] |= m;
  if (!was) {
    // persist lazily; cheap binary dump
    File f = LittleFS.open("/attackers.bin", "w");
    if (f) { f.write((const uint8_t*)g_attackerBits, sizeof(g_attackerBits)); f.close(); }
  }
  return !was;
}

uint16_t statsUniqueCount() {
  uint16_t n = 0;
  for (int i=0;i<16;i++) n += __builtin_popcount(g_attackerBits[i]);
  return n;
}

static void statsLoadAttackers() {
  memset(g_attackerBits, 0, sizeof(g_attackerBits));
  File f = LittleFS.open("/attackers.bin", "r");
  if (f) { f.read((uint8_t*)g_attackerBits, sizeof(g_attackerBits)); f.close(); }
}

// ---------------- SIRC-20 helpers ----------------
// flip 32-bit
static inline uint32_t bitrev32(uint32_t x) {
  x = ((x >> 1) & 0x55555555u) | ((x & 0x55555555u) << 1);
  x = ((x >> 2) & 0x33333333u) | ((x & 0x33333333u) << 2);
  x = ((x >> 4) & 0x0F0F0F0Fu) | ((x & 0x0F0F0F0Fu) << 4);
  x = ((x >> 8) & 0x00FF00FFu) | ((x & 0x00FF00FFu) << 8);
  x = (x >> 16) | (x << 16);
  return x;
}
// Take IRremote's MSB-first raw32, return our LSB-first 20-bit word (right-aligned)
static inline uint32_t sirc20FlipToLSB(uint32_t raw32) {
  return bitrev32(raw32) >> (32 - 20);
}
// Unpack: [0..5]=op(6), [6..14]=attacker(9), [15..19]=hi5(5)
static inline void sirc20Unpack(uint32_t wLSB, uint8_t &op, uint16_t &att, uint8_t &hi5) {
  op  =  wLSB        & 0x3F;
  att = (wLSB >> 6)  & 0x1FF;
  hi5 = (wLSB >> 15) & 0x1F;
}
// For value-style ops (14-bit)
static inline uint16_t sirc20Value14(uint32_t wLSB) {
  uint16_t low9 = (wLSB >> 6) & 0x1FF;
  uint16_t hi5  = (wLSB >> 15) & 0x1F;
  return (hi5 << 9) | low9;
}


// Pack our 20-bit Sony word (LSB-first on the wire):
// [0..5]=op, [6..14]=attacker(9b), [15..19]=charm(5b)
static inline uint32_t sirc20Pack(uint8_t op, uint16_t attacker, uint8_t charm) {
  return (uint32_t(op & 0x3F))
       | (uint32_t(attacker & 0x1FF) << 6)
       | (uint32_t(charm & 0x1F) << 15);
}

static void sendSirc20(uint32_t word, const char* tag = nullptr) {
#ifdef IR_TX_PIN
  if (tag) Serial.printf("[IRTX] %s SIRC20=0x%05lX\n", tag, (unsigned long)word);
  else     Serial.printf("[IRTX] SIRC20=0x%05lX\n", (unsigned long)word);
  // 20-bit Sony (library sends LSB-first as needed)
  IrSender.sendSony(word, 20);
#else
  (void)word; (void)tag;
  Serial.println("[IRTX] (no IR_TX_PIN) Fire pressed — anim only");
#endif
}

// Convenience: fire with our own CFG.id and charm (0..31; 0 = no unlock)
static void sendFireBadge(uint8_t charm = 0) {
        IrReceiver.disableIRIn(); 

  uint32_t w = sirc20Pack(/*op=*/0x00, /*attacker=*/CFG.id, /*charm=*/(CFG.userCharmId & 31));
  sendSirc20(w, "FIRE");
  delay(30);
        IrReceiver.enableIRIn(); 

}

// Respond to a SCORE_REQUEST by sending 3 frames: SCORE0/1/2

static void sendScoreTriplet(uint16_t myId, uint16_t score15) {
  // split 15-bit score into 3×5b chunks
  uint8_t p0 =  score15        & 0x1F; // [4:0]
  uint8_t p1 = (score15 >> 5)  & 0x1F; // [9:5]
  uint8_t p2 = (score15 >> 10) & 0x1F; // [14:10]

  // Add small randomized/backoff delay to reduce collisions across badges
  // (also bias by ID to spread even more)
  uint16_t jitter = 10 + (myId & 0x07) * 5 + (uint16_t)random(0, 15);
  delay(jitter);

  const uint16_t interPartMs  = 14;  // gap between 0→1 and 1→2
  const uint16_t interGroupMs = 28;  // gap between repeat groups
  const uint8_t  groups       = 2;   // send triplet twice

  for (uint8_t g = 0; g < groups; ++g) {
    IrSender.sendSony(sirc20Pack(0x07, myId, p0), 20); // SCORE0
    delay(interPartMs);
    IrSender.sendSony(sirc20Pack(0x08, myId, p1), 20); // SCORE1
    delay(interPartMs);
    IrSender.sendSony(sirc20Pack(0x09, myId, p2), 20); // SCORE2
    delay(interGroupMs);
  }
}

// ---------------- Simple message system ----------------
static const uint8_t CHAR_W = 6;     // 5x7 font + 1px gap
static const uint16_t SCROLL_MS = 80;

static String buildIdleLine() {
  String s = CFG.name.length() ? CFG.name : "Badge";
  //s += "  ";
  //for (uint8_t i=0;i<32;i++) if (CFG.unlockedMask & (1UL<<i)) s += char('0'+(i%10));
  //s += "  ";
  return s;
}
static String resolveMessageText(uint8_t id) {
  if (id == 0) return buildIdleLine();
  switch (id) {
    case 1: return "Fuse CATS: Michael Kohler, Andy Babin, and Ryan Middlemiss 2025";
    case 2: return "Share and enjoy";
    case 3: return "Pixera starts in Room 1 soon";
    default: return String("MSG ") + id;
  }
}

struct ScrollState {
  bool     active = false;
  bool     isIdle = true;        // true → idle (name) loop
  uint8_t  repeats = 0;          // for non-idle message
  uint16_t color = 0;            // 565 color for text
  String   text;
  int16_t  x = WIDTH;            // cursor X
  uint16_t w = 0;                // text pixel width
  uint16_t contentW = 0;   // <-- total width (text + gap + charm if any)
  uint32_t lastStep = 0;         // pacing
} g_scroll;

// ---- Text effects mode ----
enum TextMode : uint8_t { TM_SCROLL = 0, TM_BOUNCE = 1, TM_RAINBOW = 2, TM_MAX };
static TextMode g_textMode = TM_SCROLL;

// Per-mode state (bounce + rainbow)
static int16_t boX = 0;
static int8_t  boDir = +1;
static uint8_t rbHueBase = 0;

static inline bool hasUserCharm() {
  return (CFG.userCharmId != 0xFF);  // no unlock check; always show if configured
}



void startScroll(bool idle, uint8_t msgId = 0, uint8_t reps = 0, uint16_t color = 0) {
  g_scroll.isIdle  = idle;
  g_scroll.repeats = idle ? 0 : max<uint8_t>(1, reps);
  g_scroll.color   = color ? color : badgeColor565();
  g_scroll.text    = idle ? buildIdleLine() : resolveMessageText(msgId);

  matrix->setFont(NULL);
  matrix->setTextSize(1);
  matrix->setTextWrap(false);

  // Text width (5x7 font + 1px gap each char -> you used CHAR_W = 6)
  g_scroll.w = g_scroll.text.length() * CHAR_W;

  // If idle and a user charm is set, add its width + a small gap
  const uint16_t ICON_W   = hasUserCharm() ? CHARM_W : 0;
  const uint16_t ICON_GAP = hasUserCharm() ? 3 : 0;

  g_scroll.contentW = g_scroll.w + ICON_GAP + ICON_W;

  g_scroll.x = WIDTH;           // start offscreen to the right
  g_scroll.lastStep = 0;
  g_scroll.active = true;
}

// Draw charm `id` at (x,y) using RGB565 from PROGMEM
static void drawCharm565(uint8_t id, int16_t x, int16_t y) {
  if (id >= CHARM_COUNT) return;
  const uint16_t* p = charms565[id];
  for (uint8_t yy = 0; yy < CHARM_H; ++yy) {
    for (uint8_t xx = 0; xx < CHARM_W; ++xx) {
      uint16_t c = pgm_read_word(p++);      // 16-bit RGB565
      matrix->drawPixel(x + xx, y + yy, c); // NeoMatrix takes 565 directly
    }
  }
}


static void renderScrollTick() {
  const uint32_t now = millis();
  if (now - g_scroll.lastStep < SCROLL_MS) return;
  g_scroll.lastStep = now;

  matrix->fillScreen(0);
  matrix->setFont(NULL);
  matrix->setTextSize(1);
  matrix->setTextWrap(false);

  switch (g_textMode) {
    case TM_SCROLL: {
      matrix->setTextColor(g_scroll.color);

      // Draw text at current x
      matrix->setCursor(g_scroll.x, 0);
      matrix->print(g_scroll.text);

      // If idle, append the user charm inline so it scrolls together
      if (g_scroll.isIdle && hasUserCharm()) {
        const int ICON_GAP = 3;
        const int iconX = g_scroll.x + (int)g_scroll.w + ICON_GAP;
        drawCharm565(CFG.userCharmId, iconX, 0);
      }

      FastLED.show();

      // Move left
      g_scroll.x--;

      // Wrap when all content (text + optional icon) has fully left the screen
      if (g_scroll.x < -(int)g_scroll.contentW) {
        if (!g_scroll.isIdle) {
          if (--g_scroll.repeats == 0) {
            startScroll(/*idle*/true);
            break;
          }
        }
        g_scroll.x = WIDTH; // restart same content
      }
    } break;

    case TM_BOUNCE: {
      const int16_t minX = (g_scroll.w > WIDTH) ? -((int16_t)g_scroll.w - WIDTH) : 0;
      const int16_t maxX = (g_scroll.w > WIDTH) ? 0 : (WIDTH - (int16_t)g_scroll.w);

      matrix->setTextColor(g_scroll.color);
      matrix->setCursor(boX, 0);
      matrix->print(g_scroll.text);
      FastLED.show();

      boX += boDir;
      if (boX <= minX || boX >= maxX) {
        boDir = -boDir;
        if (boX <= minX && !g_scroll.isIdle && --g_scroll.repeats == 0) {
          startScroll(true);
          return;
        }
      }
    } break;

    case TM_RAINBOW: {
      int16_t x = g_scroll.x;
      for (uint16_t i = 0; i < g_scroll.text.length(); ++i) {
        CHSV hsv(rbHueBase + i * 8, 255, 255);
        CRGB rgb; hsv2rgb_rainbow(hsv, rgb);
        matrix->setTextColor(matrix->Color(rgb.r, rgb.g, rgb.b));
        matrix->setCursor(x, 0);
        matrix->print(g_scroll.text[i]);
        x += CHAR_W;
      }
      FastLED.show();

      rbHueBase++;
      g_scroll.x--;
      if (g_scroll.x < -(int)g_scroll.w) {
        if (!g_scroll.isIdle && --g_scroll.repeats == 0) {
          startScroll(true);
          return;
        }
        g_scroll.x = WIDTH;
      }
    } break;
  }
}



// Force a message to start immediately (preempt)
static void onShowMessage(uint8_t msgId, uint8_t scrolls) {
  startScroll(/*idle*/false, msgId, scrolls, 0xffff);
  Serial.printf("[MSG] NOW id=%u scrolls=%u\n", msgId, scrolls);
}


/*
// Draw charm `id` at top-left (x,y). Assumes your matrix origin is top-left.
void drawCharm(uint8_t id, int16_t x, int16_t y) {
  if (id >= CHARM_COUNT) return;
  const uint16_t* p = charms565[id];
  for (uint8_t yy=0; yy<CHARM_H; ++yy) {
    for (uint8_t xx=0; xx<CHARM_W; ++xx) {
      uint8_t r = pgm_read_byte(p++),
              g = pgm_read_byte(p++),
              b = pgm_read_byte(p++);
      matrix->drawPixel(x + xx, y + yy, matrix->Color(r,g,b));
    }
  }
  FastLED.show();
}
*/




static inline void drawUserCharmOverlay() {
  if (hasUserCharm()) {
    // draw at left; icon is 9x7
    drawCharm565(CFG.userCharmId, /*x=*/0, /*y=*/0);
  }
}

//scene


// ---- Scene player ----
struct ScenePlayer {
  bool active = false;
  const uint16_t (*frames)[WIDTH*HEIGHT] = nullptr; // PROGMEM frames
  uint8_t frameCount = 0;
  uint8_t fps = 8;
  uint8_t idx = 0;
  uint32_t nextMs = 0;
} g_scene;

static inline uint8_t u5to8(uint8_t v){ return (v * 527 + 23) >> 6; }  // 5->8
static inline uint8_t u6to8(uint8_t v){ return (v * 259 + 33) >> 6; }  // 6->8

static inline void rgb565ToCRGB(uint16_t c, CRGB &out) {
  out.r = u5to8((c >> 11) & 0x1F);
  out.g = u6to8((c >>  5) & 0x3F);
  out.b = u5to8((c      ) & 0x1F);
}

// use the NeoMatrix mapping (don’t write leds[] directly)
static void drawSceneFrame(const uint16_t* frame565) {
  matrix->fillScreen(0);
  for (int y=0; y<HEIGHT; ++y){
    for (int x=0; x<WIDTH; ++x){
      uint16_t c = pgm_read_word(&frame565[y*WIDTH + x]);
      CRGB rgb; rgb565ToCRGB(c, rgb);
      matrix->drawPixel(x, y, matrix->Color(rgb.r, rgb.g, rgb.b));
    }
  }
  FastLED.show();
}

static void sceneStart(const uint16_t (*frames)[WIDTH*HEIGHT],
                       uint8_t count, uint8_t fps)
{
  g_scene.frames     = frames;
  g_scene.frameCount = count;
  g_scene.fps        = (fps == 0 ? 8 : fps);
  g_scene.idx        = 0;
  g_scene.nextMs     = 0;
  g_scene.active     = true;

  // pre-empt other visuals
  //g_scene.active   = false;
  // also pause/clear message scroll if you want:
  // activeIsIdle = true; qHead=qCount=0; // etc.
}

static void sceneTick() {
  if (!g_scene.active) return;
  uint32_t now = millis();
  if (now < g_scene.nextMs) return;

  drawSceneFrame(g_scene.frames[g_scene.idx]);
  g_scene.idx++;

  if (g_scene.idx >= g_scene.frameCount) {
    g_scene.active = false;           // play once then return to normal
    return;
  }
  g_scene.nextMs = now + (1000u / g_scene.fps);
}


// ---------------- Fire animation ----------------
struct FireAnim { bool active=false; uint32_t t0=0; uint16_t dur=420; } g_fire;

static void fireAnimStart() {
  g_fire.active = true; g_fire.t0 = millis();
  tone(MEOW, 1200, 80);
  tone(PURR, 160,  80);
}
static void fireAnimRender() {
  if (!g_fire.active) return;
  uint32_t t = millis() - g_fire.t0;
  if (t >= g_fire.dur) { g_fire.active = false; return; }

  float p = float(t)/float(g_fire.dur);

  matrix->fillScreen(0);
  for (int y=0; y<HEIGHT; ++y) {
    float band = p*1.4f - (float(y)/(HEIGHT-1));
    uint8_t heat = band <= 0.f ? 0 : (uint8_t)min(255.f, band*255.f);
    CRGB col = HeatColor(heat);
    for (int x=0; x<WIDTH; ++x) leds[y*WIDTH + x] = col;
  }
  FastLED.show();
}

//stats

// ---- Stats view state machine ----
struct StatsView {
  bool active = false;
  uint8_t phase = 0;          // 0 = scrolling line, 1 = charms slideshow
  uint32_t t0 = 0;

  // unlocked list
  uint8_t list[32];
  uint8_t count = 0;
  uint8_t idx = 0;

  // phase-0 scroller state
  String  line;               // e.g., "H:23  Charms:5"
  int16_t scX = 0;
  uint16_t scW = 0;
  uint32_t lastStep = 0;
  uint8_t  loops = 0;         // number of completed scroll loops
  uint16_t scTextW = 0;     // pixels of "S:...  Charms:N"
  uint16_t scIconsW = 0;    // pixels for all icons + inter-icon spacing
  uint16_t contentW = 0;    // scTextW + TEXT_ICON_GAP + scIconsW
} g_stats;

static void statsStart() {
  // build list of unlocked charms, clamp to CHARM_COUNT
  g_stats.count = 0;
  for (uint8_t i = 0; i < 32; i++) {
    if ((CFG.unlockedMask & (1UL << i)) && i < CHARM_COUNT) {
      g_stats.list[g_stats.count++] = i;
    }
  }

  // Build scrolling status line for phase 0
  const uint32_t score = g_score.score;           // <-- your stored score
  g_stats.line = String("Score: ") + score + "  Charms:" + g_stats.count;

  g_stats.phase = 0;
  g_stats.idx = 0;
  g_stats.t0 = millis();

  // init scroller geometry
  matrix->setFont(NULL);
  matrix->setTextSize(1);
  matrix->setTextWrap(false);

  g_stats.scTextW  = g_stats.line.length() * CHAR_W;

  // width of icons block (only if there are unlocked charms)
  if (g_stats.count) {
    g_stats.scIconsW = (g_stats.count * CHARM_W) + ((g_stats.count - 1) * ICON_SPACING);
    g_stats.contentW = g_stats.scTextW + TEXT_ICON_GAP + g_stats.scIconsW;
  } else {
    g_stats.scIconsW = 0;
    g_stats.contentW = g_stats.scTextW;
  }

  g_stats.scW = g_stats.contentW;   // scroll total = text + (gap + icons if any)
  g_stats.scX = WIDTH;              // start off right edge
  g_stats.lastStep = 0;
  g_stats.loops = 0;

  g_stats.active = true;
}

static void statsTick() {
  if (!g_stats.active) return;
  const uint32_t now = millis();

  if (g_stats.phase == 0) {
    if (now - g_stats.lastStep >= SCROLL_MS) {
      g_stats.lastStep = now;

      matrix->fillScreen(0);
      matrix->setFont(NULL);
      matrix->setTextSize(1);
      matrix->setTextWrap(false);
      matrix->setTextColor(matrix->Color(255, 255, 255));

      // draw the text
      matrix->setCursor(g_stats.scX, 0);
      matrix->print(g_stats.line);

      // draw the icons right after the text (if any)
      if (g_stats.count) {
        int16_t x = g_stats.scX + (int)g_stats.scTextW + TEXT_ICON_GAP;
        for (uint8_t i = 0; i < g_stats.count; ++i) {
          drawCharm565(g_stats.list[i], x, 0);          // uses CHARM_W/CHARM_H from charms565.hpp
          x += CHARM_W + ICON_SPACING;
        }
      }

      FastLED.show();

      // advance, then wrap when the whole strip has left
      g_stats.scX--;
      if (g_stats.scX < -(int)g_stats.contentW) {
        g_stats.scX = WIDTH;
        g_stats.loops++;
        if (g_stats.loops >= 2) {   // after 2 full passes, go to charms slideshow
          g_stats.phase = 1;
          g_stats.t0 = now;
        }
      }
    }
    return;
  }

  // Phase 1: cycle each unlocked charm (400ms each)
  if (g_stats.phase == 1) {
    if (g_stats.idx < g_stats.count) {
      if (now - g_stats.t0 >= 400) {
        matrix->fillScreen(0);
        drawCharm565(g_stats.list[g_stats.idx], 3, 0);
        FastLED.show();
        g_stats.idx++;
        g_stats.t0 = now;
      }
    } else {
      g_stats.active = false; // done; return to normal display
    }
  }
}

// Show all charms in a simple strip for verification
void testAllCharms() {
  matrix->fillScreen(0);
  int16_t x = 0;
  for (uint8_t i = 0; i < CHARM_COUNT; ++i) {
    drawCharm565(i, x, 0);
    x += (CHARM_W + 1);
    if (x + CHARM_W > WIDTH) {  // wrap line if needed
      FastLED.show(); delay(600);
      matrix->fillScreen(0); x = 0;
    }
  }
  FastLED.show();
}


static void handleButtons() {
  if (millis() - btnLastMs < 30) return; // debounce
  btnLastMs = millis();

  bool b1 = digitalRead(BTN1); // pullup: pressed == LOW
  bool b2 = digitalRead(BTN2);
  bool b3 = digitalRead(BTN3);

  if (!b1 && prevB1) {
    // BTN1: Stats view
    statsStart();
    Serial.println("SendStats");
  }

  if (!b2 && prevB2) {
    // BTN2: Fire (send + local FX)
    //fireAnimStart();
    sendFireBadge(/*charm=*/0);  // change charm if you want local testing
    g_score.fires_total++;

    Serial.println("Send Fire");
  }

  if (!b3 && prevB3) {
    g_textMode = static_cast<TextMode>((g_textMode + 1) % TM_MAX);
    // re-start current text under the new mode so you can see the change right away
    startScroll(g_scroll.isIdle, /*msgId*/0, g_scroll.repeats, g_scroll.color);
    Serial.printf("[UI] TextMode -> %u\n", (unsigned)g_textMode);
  }
    prevB1 = b1; prevB2 = b2; prevB3 = b3;
}


// ---------------- IR handling ----------------
void handleIR() {
  if (!IrReceiver.decode()) return;
  auto &d = IrReceiver.decodedIRData;

  if (d.numberOfBits == 20) {
    uint32_t wLSB = sirc20FlipToLSB(d.decodedRawData);
    uint8_t  op; uint16_t attacker; uint8_t hi5;
    sirc20Unpack(wLSB, op, attacker, hi5);

    switch (op) {
      case 0x00: { // FIRE
        uint8_t charm = hi5 & 0x1F;
        Serial.printf("[IR] FIRE attacker=%u charm=%u\n", attacker, charm);
        g_score.hits_total++;                 // any laser tag hit → Roll 5
        scoreAwardRoll(5, "hit");

          // Unique badge ID tiers (only if first time seen)
        bool firstTime = statsAddAttacker(attacker); // your existing unique-bitset
        if (firstTime) {
          if (attacker >= 300 && attacker <= 400) {
            scoreAwardRoll(40, "unique badge 300-400");
          } else if (attacker >= 255 && attacker <= 300) {
            scoreAwardRoll(20, "unique badge 255-300");
          } else if (attacker >= 1 && attacker <= 254) {
            scoreAwardRoll(10, "unique badge 1-254");
          }
        }

        // Charm unlock → if newly unlocked, Roll 20
        if (charm) {
          uint32_t bit = (1UL << (charm & 31));
          bool was = (CFG.unlockedMask & bit);
          if (!was) {
            CFG.unlockedMask |= bit;
            saveConfig(CFG);
            scoreAwardRoll(20, "unique charm");
          }
        }

        fireAnimStart();
      } break;

      case 0x01: { // SLEEP (value14: minutes 0..127)
        uint8_t minutes = sirc20Value14(wLSB) & 0x7F;
        Serial.printf("[IR] SLEEP %u\n", minutes);
        // TODO: sleep behavior if you want it here
      } break;

      case 0x02: { // WAKE
        Serial.println("[IR] WAKE");
        g_score.sessions_woke++;
        scoreAwardRoll(10, "session wake");

        // TODO: wake behavior
      } break;

      case 0x03: { // SET_BRIGHTNESS (0..128)
        uint8_t b = sirc20Value14(wLSB) & 0x7F;
        Serial.printf("[IR] BRIGHT %u\n", b);
        FastLED.setBrightness(map128to255(b));
        CFG.brightness = b; saveConfig(CFG);
        FastLED.show();
      } break;

      case 0x04: { // SHOW_MESSAGE (value14 = (id<<7)|scrolls)
        uint16_t v = sirc20Value14(wLSB);
        uint8_t scrolls   =  v & 0x7F;
        uint8_t messageId = (v >> 7) & 0x7F;
        if (scrolls == 0) scrolls = 1;
        Serial.printf("[IR] SHOW_MESSAGE id=%u scrolls=%u\n", messageId, scrolls);
        onShowMessage(messageId, scrolls); // preempt immediately
      } break;

      case 0x05: { // SPECIAL_SCENE
        uint16_t sid = sirc20Value14(wLSB) & 0x7F;
        g_score.scenes_triggered++;
        scoreAwardRoll(40, "special scene");
        Serial.printf("[IR] SCENE %u\n", sid);

        // pick the scene by ID
        switch (sid) {
          case SCENE_NYAN_ID:
            sceneStart(scene_nyan_frames, SCENE_NYAN_FRAMES, SCENE_NYAN_FPS);
            break;

          // add more scenes here:
          // case SCENE_RAIN_ID: sceneStart(scene_rain_frames, SCENE_RAIN_FRAMES, SCENE_RAIN_FPS); break;

          default:
            Serial.println("[SCENE] unknown id");
            break;
        }
      } break;

      case 0x06: {
       // respect throttle
      if (!scoreThrottleOk()) {
        #if 1
            Serial.println("[SCORE] throttled");
        #endif
            break;
          }

          // (optional) tiny jitter so many badges don't transmit at the exact same time
          if (SCORE_RESP_JITTER_MS) delay((uint32_t)random(0, SCORE_RESP_JITTER_MS + 1));
          #define SCORE_TX_DIV  1   // set to 10 if you want to transmit “score/10”
          uint32_t txScore = scoreGet() / SCORE_TX_DIV;
          if (txScore > 32767) txScore = 32767;  // clamp for 3 parts (5+5+5)
          
            //uint16_t score15 = CFG.score & 0x7FFF;   // whatever you compute
          sendScoreTriplet(CFG.id, txScore);          // your existing robust sender
        } break;

      case 0x07: {
        //uint8_t lo5 = hi5 & 0x1F; // payload5 area
        //Serial.printf("[IR] SCORE_RSP_LO id=%u lo5=%u\n", attacker, lo5);
      } break;

      case 0x08: {
        //uint8_t hi5 = hi5 & 0x1F;
        //Serial.printf("[IR] SCORE_RSP_HI id=%u hi5=%u\n", attacker, hi5);
      } break;

      case 0x09: {
        //uint8_t hi5 = hi5 & 0x1F;
        //Serial.printf("[IR] SCORE_RSP_HI id=%u hi5=%u\n", attacker, hi5);
      } break;


      default:
        Serial.printf("[IR] Unknown op=0x%02X (wLSB=0x%05lX)\n", op, (unsigned long)wLSB);
        break;
    }
  }
  IrReceiver.resume();
}

// ---------------- Arduino lifecycle ----------------
void setup() {
  delay(200);
  Serial.begin(115200);
  pinMode(MEOW, OUTPUT); pinMode(PURR, OUTPUT);
  pinMode(BTN1, INPUT_PULLUP);
  pinMode(BTN2, INPUT_PULLUP);
  pinMode(BTN3, INPUT_PULLUP);

  // Seed RNG from jitter
  randomSeed( (uint32_t)micros() ^ (uint32_t)analogRead(A0) ^ (uint32_t)millis() );

  #ifdef IR_TX_PIN
    IrSender.begin(IR_TX_PIN);
  #endif

  LittleFS.begin();
  loadConfig(CFG);
  scoreLoad();
  statsLoadAttackers();

  FastLED.addLeds<CHIPSET, PIXEL_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 1000);
  FastLED.setBrightness(map128to255(CFG.brightness));
  FastLED.clear(true); FastLED.show();

  IrReceiver.begin(IR_RX_PIN, DISABLE_LED_FEEDBACK); // disable LED feedback on RP2040 boards

  Serial.printf("[IR] RX on pin %d (SIRC-20)\n", IR_RX_PIN);

  delay(500);
  //testAllCharms();
  // start idle name scroll
  startScroll(/*idle*/true);
    Serial.printf("[BOOT] Build %s %s\n", BUILD_DATE, BUILD_TIME);

}

void loop() {  
  handleIR();
  handleButtons();


  if (g_scene.active) {
    sceneTick();
  } else if (g_fire.active) {
    fireAnimRender();
  } else if (g_stats.active) {
    statsTick();
  } else {
    renderScrollTick();
  }

  // fire animation takes over screen; otherwise scroll
  //if (g_fire.active) fireAnimRender();
  //else               renderScrollTick();

  handleSerial(); 
  scoreMaybeAutoSave();  // throttle persistent writes
}
// ======================================================================
