#pragma once
#include <Arduino.h>

namespace CATSIR64 {
  // Layout (MSB-first in the air):
  // [63:62] ver (2 bits)  -> fixed 1
  // [61:56] op  (6 bits)
  // [55:47] src (9 bits)  -> 0..511 badges
  // [46:0]  pay (47 bits) -> for FIRE we use lower 32b as charm mask
  static constexpr uint8_t  VER = 1;

  // Opcodes
  enum : uint8_t {
    FIRE        = 0x01,
    UNLOCK_CHARM= 0x02,
    SLEEP       = 0x03,
    WAKE        = 0x04,
    SET_BRIGHT  = 0x05,
    SHOW_MESSAGE= 0x06,
    SPECIAL_SCENE=0x07,
    DUMP_STATS  = 0x08
  };

  inline uint64_t pack64(uint8_t op, uint16_t src, uint64_t payload /*<=47b*/) {
    return (uint64_t(VER) & 0x3ULL) << 62 |
           (uint64_t(op)  & 0x3FULL) << 56 |
           (uint64_t(src) & 0x1FFULL) << 47 |
           (payload & 0x7FFFFFFFFFFFULL);
  }

  inline bool unpack64(uint64_t w, uint8_t &op, uint16_t &src, uint64_t &payload) {
    uint8_t ver = (w >> 62) & 0x3;
    if (ver != VER) return false;
    op       = (w >> 56) & 0x3F;
    src      = (w >> 47) & 0x1FF;
    payload  =  w & 0x7FFFFFFFFFFFULL;
    return true;
  }
}
