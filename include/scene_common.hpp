#pragma once
#include <Arduino.h>
#include <FastLED.h>

#define SCN_W 15
#define SCN_H 7
#define SCN_PIX (SCN_W*SCN_H)

struct SceneRef {
  const uint16_t *frames565;  // pointer to first frame (RGB565, PROGMEM)
  uint16_t frameCount;        // number of frames
  uint16_t msPerFrame;        // playback speed
};

// Fast blit 565 -> FastLED CRGB
static inline void blit565ToLeds(const uint16_t *frame565, CRGB *leds) {
  for (uint16_t i = 0; i < SCN_PIX; ++i) {
    uint16_t p = pgm_read_word(&frame565[i]);
    uint8_t r = ((p >> 11) & 0x1F) * 255 / 31;
    uint8_t g = ((p >> 5)  & 0x3F) * 255 / 63;
    uint8_t b = ( p        & 0x1F) * 255 / 31;
    leds[i] = CRGB(r,g,b);
  }
}