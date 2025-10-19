#pragma once
#include <Arduino.h>
#include <IRremote.hpp>

#ifndef MICROS_PER_TICK
// IRremote usually defines this; fall back to 50us if not.
#define MICROS_PER_TICK 50
#endif

// Reverse 32 bits (used to flip on-wire LSB->MSB)
static inline uint32_t bitrev32(uint32_t x) {
  x = ((x >> 1) & 0x55555555UL) | ((x & 0x55555555UL) << 1);
  x = ((x >> 2) & 0x33333333UL) | ((x & 0x33333333UL) << 2);
  x = ((x >> 4) & 0x0F0F0F0FUL) | ((x & 0x0F0F0F0FUL) << 4);
  x = ((x >> 8) & 0x00FF00FFUL) | ((x & 0x00FF00FFUL) << 8);
  x = (x >> 16) | (x << 16);
  return x;
}

static inline bool nearPct(uint32_t v, uint32_t ref, uint8_t tolPct) {
  uint32_t lo = (uint64_t)ref * (100 - tolPct) / 100;
  uint32_t hi = (uint64_t)ref * (100 + tolPct) / 100;
  return v >= lo && v <= hi;
}

// Decode **64 bits** of NEC-like data from IRremote rawbuf (tolerant).
// Returns MSB-first word in 'word64'.
static inline bool decodeNec64FromRaw(uint64_t &word64) {
  auto *raw = IrReceiver.decodedIRData.rawDataPtr;
  if (!raw) return false;

  const auto n = raw->rawlen;
  if (n < 2 + 2*64 + 2) return false; // header + 64 bits + trailing

  uint32_t hdrMark  = (uint32_t)raw->rawbuf[1] * MICROS_PER_TICK;
  uint32_t hdrSpace = (uint32_t)raw->rawbuf[2] * MICROS_PER_TICK;
  if (!nearPct(hdrMark, 9000, 35) || !nearPct(hdrSpace, 4500, 40)) return false;

  uint64_t bitsLSB = 0;
  int idx = 3;
  for (int i = 0; i < 64; i++) {
    if (idx + 1 >= n) return false;
    uint32_t mark  = (uint32_t)raw->rawbuf[idx++] * MICROS_PER_TICK;
    uint32_t space = (uint32_t)raw->rawbuf[idx++] * MICROS_PER_TICK;
    if (!nearPct(mark, 560, 45)) return false;

    bool one  = nearPct(space, 1690, 40);
    bool zero = nearPct(space,  560, 45);
    if (!one && !zero) return false;

    if (one) bitsLSB |= (1ULL << i);  // NEC is LSB-first on the wire
  }

  // Convert to MSB-first 64b: reverse order of 64 bits via two 32b reversals.
  uint32_t lo = (uint32_t)(bitsLSB & 0xFFFFFFFFULL);
  uint32_t hi = (uint32_t)(bitsLSB >> 32);
  lo = bitrev32(lo);
  hi = bitrev32(hi);
  word64 = (uint64_t)lo << 32 | (uint64_t)hi;
  return true;
}
