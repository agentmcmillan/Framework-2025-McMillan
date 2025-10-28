#pragma once
#include <Arduino.h>

namespace CATSIR {

// 64-bit logical frame layout (MSB-first):
// [63:59]=op (5 bits) | [58:50]=src (9 bits, 0..511) | [49:18]=payload (32 bits) | [17:0]=0
// All commands below use the same 64-bit envelope.
// Transport over IR (NEC) is done as two 32-bit words: LO then HI.
enum Op : uint8_t {
  OP_FIRE = 0x00,
  OP_SLEEP = 0x01,
  OP_WAKE = 0x02,
  OP_SET_BRIGHT = 0x03,
  OP_SHOW_MESSAGE = 0x04,
  OP_SPECIAL_SCENE = 0x05,
  OP_SCORE_REQUEST = 0x06,
  OP_SCORE0  = 0x07,
  OP_SCORE1  = 0x08,
  OP_SCORE2  = 0x09,
  
};
// TX pacing for the triplet
static constexpr uint8_t  SCORE_RESP_REPEATS = 0;  // 0 = send once, 1 = send twice, etc.
static constexpr uint16_t SCORE_RESP_GAP_MS  = 8;  // gap between frames in a triplet

//static const uint8_t OP_SCORE_REQUEST = 0x06;
//static const uint8_t OP_SCORE_RSP_LO  = 0x07;
//static const uint8_t OP_SCORE_RSP_HI  = 0x08;

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
