#pragma once
#include <Arduino.h>

// SIRC-20 logical packing (20 bits total; LSB-first on the wire)
namespace SIRC20 {

// 6-bit ops
enum Op : uint8_t {
  FIRE            = 0x01, // layout A: [op:6 | attacker:9 | charm:5]
  UNLOCK_CHARM    = 0x02, // layout B: [op:6 | value14]
  SLEEP           = 0x03, // value = minutes (0..127)
  WAKE            = 0x04, // value = 0
  SET_BRIGHTNESS  = 0x05, // value = 0..128 (capped)
  SHOW_MESSAGE    = 0x06, // value = (messageId<<7) | scrolls
  SPECIAL_SCENE   = 0x07, // value = scene id
  DUMP_STATS      = 0x08  // reserved
};

// ---- Pack helpers (MSB order in the 20-bit integer; we still transmit LSB-first on the wire)

inline uint16_t packFire(uint16_t attacker /*0..511*/, uint8_t charm /*0..31*/) {
  attacker &= 0x1FF;
  charm    &= 0x1F;
  uint16_t word = (uint16_t(FIRE & 0x3F) << 14) | (uint16_t(attacker) << 5) | charm;
  return word; // 20-bit value in lower bits
}

inline uint16_t packKV(uint8_t op /*6-bit*/, uint16_t value14) {
  uint16_t v = value14 & 0x3FFF;
  uint16_t word = (uint16_t(op & 0x3F) << 14) | v;
  return word; // 20-bit value in lower bits
}

// ---- Unpack

inline uint8_t  getOp(uint16_t w20)      { return (w20 >> 14) & 0x3F; }
inline uint16_t getValue14(uint16_t w20) { return (w20 & 0x3FFF); }

inline void splitFire(uint16_t w20, uint16_t &attacker, uint8_t &charm) {
  attacker = (w20 >> 5) & 0x1FF;
  charm    = (w20 & 0x1F);
}

// Optional: helper to pretty print a 20-bit word
inline void debugPrintWord(Print &p, uint16_t w20) {
  p.printf("op=0x%02X w20=0x%05X\n", getOp(w20), w20 & 0xFFFFF);
}

} // namespace SIRC20
