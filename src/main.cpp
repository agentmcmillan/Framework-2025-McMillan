#include <Arduino.h>
#include <IRremote.hpp>   // IRremote v4.x

// ==== change this to your RX pin ====
#define IR_RX_PIN 27
// ---- helpers ----
static inline uint32_t bitrev32(uint32_t x) {
  x = ((x >> 1) & 0x55555555u) | ((x & 0x55555555u) << 1);
  x = ((x >> 2) & 0x33333333u) | ((x & 0x33333333u) << 2);
  x = ((x >> 4) & 0x0F0F0F0Fu) | ((x & 0x0F0F0F0Fu) << 4);
  x = ((x >> 8) & 0x00FF00FFu) | ((x & 0x00FF00FFu) << 8);
  x = (x >> 16) | (x << 16);
  return x;
}

static inline uint32_t sirc20FlipToLSB(uint32_t raw, uint8_t bits = 20) {
  // Flip the lower `bits` and return them right-aligned (LSB-first layout)
  uint32_t rev = bitrev32(raw);
  return rev >> (32 - bits);
}

// ---- unpack for our SIRC-20 layout ----
// LSB-first word: [0..5]=op, [6..14]=attacker (9b), [15..19]=charm/hi (5b)
static inline void sirc20Unpack(uint32_t wLSB, uint8_t &op, uint16_t &attacker, uint8_t &charmHi) {
  op       =  wLSB        & 0x3F;
  attacker = (wLSB >> 6)  & 0x1FF;
  charmHi  = (wLSB >> 15) & 0x1F;
}

// For value-style ops (SLEEP, BRIGHT, MESSAGE, SCENE):
static inline uint16_t sirc20Value14(uint32_t wLSB) {
  uint16_t low9 = (wLSB >> 6) & 0x1FF;
  uint16_t hi5  = (wLSB >> 15) & 0x1F;
  return (hi5 << 9) | low9;   // 14-bit value
}

// ---- handleIR() core ----
void handleIR() {
  if (!IrReceiver.decode()) return;

  auto &d = IrReceiver.decodedIRData;

  // We only care about 20-bit Sony frames for this protocol
  if (d.numberOfBits == 20 /* && (d.protocol == SONY or tolerant) */) {
    // Flip to our LSB-first packing
    uint32_t wLSB = sirc20FlipToLSB(d.decodedRawData, 20);

    uint8_t  op; 
    uint16_t attacker;
    uint8_t  hi5;
    sirc20Unpack(wLSB, op, attacker, hi5);

    switch (op) {
      case 0x00: { // FIRE
        uint8_t charm = hi5; // direct 0..31
        // DEBUG:
        Serial.printf("[IR] FIRE attacker=%u charm=%u (w=0x%05lX)\n",
                      attacker, charm, (unsigned long)wLSB);
        // Effects:
       // if (charm) { CFG.unlockedMask |= (1UL << (charm & 31)); saveConfig(CFG); }
        //recordHit(attacker, charm); // your hit handler
      } break;

      case 0x01: { // SLEEP
        uint16_t minutes = sirc20Value14(wLSB) & 0x7F;
        Serial.printf("[IR] SLEEP %u min\n", minutes);
        //enterSleepForMinutes(minutes);
      } break;

      case 0x02: { // WAKE
        Serial.println("[IR] WAKE");
       // wakeNow();
      } break;

      case 0x03: { // SET_BRIGHTNESS
        uint16_t b = sirc20Value14(wLSB) & 0x7F;
        Serial.printf("[IR] BRIGHT %u\n", b);
       // applyBrightness(b);  // map 0..128 to FastLED brightness
      } break;

      case 0x04: { // SHOW_MESSAGE
        uint16_t v = sirc20Value14(wLSB);
        uint8_t scrolls   =  v & 0x7F;
        uint8_t messageId = (v >> 7) & 0x7F;
        Serial.printf("[IR] MSG id=%u scrolls=%u\n", messageId, scrolls);
        //showPredefinedMessage(messageId, scrolls);
      } break;

      case 0x05: { // SPECIAL_SCENE
        uint16_t sid = sirc20Value14(wLSB) & 0x7F;
        Serial.printf("[IR] SCENE %u\n", sid);
       // playSpecialScene(sid);
      } break;

      default:
        Serial.printf("[IR] Unknown op=0x%02X rawLSB=0x%05lX\n",
                      op, (unsigned long)wLSB);
        break;
    }
  } else {
    // Not 20-bit Sony? Ignore or log briefly:
    // Serial.printf("[IR] proto=%u bits=%u raw=0x%08lX\n",
    //               (unsigned)d.protocol, (unsigned)d.numberOfBits,
    //               (unsigned long)d.decodedRawData);
  }

  IrReceiver.resume();
}


void setup() {
  Serial.begin(115200);
  delay(100);
  Serial.println("\n[IRRX] Raw sniffer starting…");

  // LED feedback is handy while testing; disable if the board’s LED conflicts.
  IrReceiver.begin(IR_RX_PIN, ENABLE_LED_FEEDBACK); 
  Serial.printf("[IRRX] Receiver on pin %d\n", IR_RX_PIN);
}

void loop() {
 handleIR();
}
