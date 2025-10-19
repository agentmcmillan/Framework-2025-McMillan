#pragma once
#include <Arduino.h>

namespace CATSIR {

// 64-bit logical frame layout (MSB-first):
// [63:59]=op (5 bits) | [58:50]=src (9 bits, 0..511) | [49:18]=payload (32 bits) | [17:0]=0
// All commands below use the same 64-bit envelope.
// Transport over IR (NEC) is done as two 32-bit words: LO then HI.
enum Op : uint8_t {
  FIRE            = 0x10,  // payload = 32-bit charm mask
  UNLOCK_CHARM    = 0x11,  // payload[0..4] = charm id (0..31)
  SLEEP           = 0x12,  // payload[0..6] = minutes (0..127)
  WAKE            = 0x13,  // payload = 0
  SET_BRIGHTNESS  = 0x14,  // payload[0..6] = brightness (0..128 capped)
  SHOW_MESSAGE    = 0x15,  // payload = (messageId<<7) | scrolls
  SPECIAL_SCENE   = 0x16,  // payload[0..6] = scene id
  DUMP_STATS      = 0x17   // TBD
};

inline uint64_t pack64(uint8_t op, uint16_t src /*0..511*/, uint32_t payload32) {
  return ( (uint64_t(op & 0x1F) << 59)
         | (uint64_t(src & 0x1FF) << 50)
         | (uint64_t(payload32) << 18) );
}

// For transport over NEC (32-bit words), split a MSB-first 64-bit frame:
inline void split64to32(uint64_t frameMSB, uint32_t &lo, uint32_t &hi) {
  lo = uint32_t(frameMSB & 0xFFFFFFFFULL);
  hi = uint32_t((frameMSB >> 32) & 0xFFFFFFFFULL);
}

} // namespace CATSIR
