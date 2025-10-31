# ESP32 Board Comparison Guide

Quick reference to help you choose the right ESP32 coprocessor for your Framework Badge.

## TL;DR - Which Board Should I Buy?

| Your Priority | Recommended Board | Why? |
|---------------|-------------------|------|
| **Best overall** | ESP32 ProS3 | 16MB Flash, 8MB PSRAM, STEMMA QT, dual LDOs |
| **WiFi 6** | XIAO ESP32-C6 | Latest WiFi standard, BLE 5.3 |
| **Smallest size** | XIAO ESP32-C6 or S3 | Only 21×17.5mm |
| **Budget** | XIAO ESP32-C6 or S3 | ~$10 vs $20 for ProS3 |
| **Easy connection** | ESP32 ProS3 | STEMMA QT plug-and-play |

---

## Detailed Comparison

### ESP32 ProS3 by Unexpected Maker

**Price:** ~$20 USD

**Pros:**
- ✅ **16MB Flash** - Store large web interfaces, OTA updates, logs
- ✅ **8MB PSRAM** - Better for graphics, buffering, complex apps
- ✅ **STEMMA QT connector** - Plug-and-play I2C (no soldering!)
- ✅ **Dual 700mA LDOs** - Ultra-stable power, auto-sleep on LDO2
- ✅ **Battery charging** - Built-in LiPo charger with PicoBlade
- ✅ **27 GPIOs** - More expansion options
- ✅ **RGB LED** - Built-in NeoPixel on GPIO48 for status
- ✅ **10µA deep sleep** - Best-in-class power efficiency
- ✅ **Great documentation** - Active Discord support
- ✅ **USB-C with ESD protection** - More robust

**Cons:**
- ❌ Larger size (Feather-like form factor)
- ❌ Higher cost
- ❌ WiFi 802.11b/g/n only (no WiFi 6)
- ❌ BLE 5.0 (not 5.3)

**Best For:**
- Production projects requiring reliability
- Complex web interfaces (LED designer, file uploads)
- Battery-powered applications (excellent deep sleep)
- Users who want plug-and-play (STEMMA QT)
- Projects needing PSRAM for graphics

**I2C Pins:** GPIO8 (SDA), GPIO9 (SCL) - STEMMA QT connector

**Setup Guide:** `ESP32_PROS3_SETUP.md`

---

### XIAO ESP32-C6 by Seeed Studio

**Price:** ~$10 USD

**Pros:**
- ✅ **WiFi 6 (802.11ax)** - Better in crowded environments (conferences!)
- ✅ **BLE 5.3** - Improved range and speed over 5.0
- ✅ **Tiny size** - Only 21×17.5mm, easy to hide
- ✅ **RISC-V architecture** - Open-source ISA
- ✅ **Low cost** - Half the price of ProS3
- ✅ **4MB Flash** - Enough for most badge applications
- ✅ **512KB SRAM** - Built-in (no external PSRAM)
- ✅ **Low power** - ~150µA deep sleep

**Cons:**
- ❌ No PSRAM (vs 8MB on ProS3)
- ❌ Smaller Flash (4MB vs 16MB)
- ❌ No STEMMA QT connector (requires soldering)
- ❌ Single LDO (vs dual on ProS3)
- ❌ No built-in battery charger
- ❌ Fewer GPIOs

**Best For:**
- Budget-conscious projects
- Conference badges (WiFi 6 advantage)
- Compact installations
- Learning RISC-V architecture
- Simple web server applications

**I2C Pins:** GPIO6 (SDA), GPIO7 (SCL)

**Setup Guide:** `ESP32_C6_SETUP.md`

---

### XIAO ESP32-S3 by Seeed Studio

**Price:** ~$10-15 USD

**Pros:**
- ✅ **8MB PSRAM** - Same as ProS3!
- ✅ **8MB Flash** - More than C6
- ✅ **Tiny size** - 21×17.5mm
- ✅ **Dual-core Xtensa** - Better multi-tasking than C6
- ✅ **Camera support** - S3 Sense variant has OV2640
- ✅ **Lower cost** than ProS3

**Cons:**
- ❌ WiFi 802.11b/g/n (no WiFi 6)
- ❌ BLE 5.0 (not 5.3)
- ❌ No STEMMA QT connector
- ❌ Single LDO
- ❌ No built-in battery charger
- ❌ Higher power consumption than C6

**Best For:**
- Projects needing PSRAM but on a budget
- Camera integration (with S3 Sense variant)
- Users familiar with Xtensa architecture
- Legacy projects (being phased out in favor of C6/ProS3)

**I2C Pins:** GPIO6 (SDA), GPIO7 (SCL)

**Setup Guide:** `ESP32_C6_SETUP.md` (same as C6)

---

## Feature Comparison Table

| Feature | ProS3 | XIAO C6 | XIAO S3 |
|---------|-------|---------|---------|
| **Architecture** | Xtensa dual-core | RISC-V single | Xtensa dual-core |
| **CPU Speed** | 240MHz | 160MHz | 240MHz |
| **Flash** | **16MB** ✓ | 4MB | 8MB |
| **PSRAM** | **8MB** ✓ | None | **8MB** ✓ |
| **WiFi** | 802.11b/g/n | **WiFi 6** ✓ | 802.11b/g/n |
| **Bluetooth** | BLE 5.0 | **BLE 5.3** ✓ | BLE 5.0 |
| **I2C Connector** | **STEMMA QT** ✓ | Pins only | Pins only |
| **I2C Pins** | GPIO8/9 | GPIO6/7 | GPIO6/7 |
| **GPIOs** | **27** ✓ | ~11 | ~11 |
| **Battery Charger** | **Yes** ✓ | No | No |
| **LDOs** | **Dual 700mA** ✓ | Single | Single |
| **RGB LED** | **Yes (GPIO48)** ✓ | No | No |
| **Deep Sleep** | **10µA** ✓ | 150µA | 150µA |
| **USB** | **USB-C + ESD** ✓ | USB-C | USB-C |
| **Size** | Large (Feather) | **Tiny** ✓ | **Tiny** ✓ |
| **Price** | $20 | **$10** ✓ | $10-15 |
| **Firmware Support** | ✅ Full | ✅ Full | ✅ Full |

---

## Real-World Scenarios

### Scenario 1: Conference Badge at Busy Event

**Best Choice:** XIAO ESP32-C6

**Why:** WiFi 6 (802.11ax) performs better in crowded 2.4GHz environments. At a tech conference with 500+ people, WiFi 6's improved spectral efficiency and OFDMA will give you better connection reliability.

### Scenario 2: Interactive Art Installation

**Best Choice:** ESP32 ProS3

**Why:** 16MB Flash allows storing complex web interfaces, animations, and media files. 8MB PSRAM enables smooth graphics. Dual LDOs provide stable operation over long periods. Battery charging for easy maintenance.

### Scenario 3: Badge-to-Badge Messaging Game

**Best Choice:** XIAO ESP32-C6

**Why:** BLE 5.3 has better range and throughput than 5.0, crucial for proximity detection and data sync. Compact size makes it easy to embed in custom badge designs.

### Scenario 4: Web-Controlled LED Matrix Designer

**Best Choice:** ESP32 ProS3

**Why:** 16MB Flash can store HTML/CSS/JS files without compression. 8MB PSRAM allows buffering full 15×7 LED patterns in memory. STEMMA QT makes prototyping quick.

### Scenario 5: Budget DIY Workshop (Teaching)

**Best Choice:** XIAO ESP32-C6

**Why:** Lower cost means you can buy more units for students. WiFi 6 is a good teaching topic. Tiny size is less intimidating for beginners. Good availability.

---

## Connection Comparison

### ProS3: STEMMA QT (Easiest!)

```
Badge SAO Port  →  STEMMA QT Adapter  →  ProS3 STEMMA QT Connector
     ↓                                           ↓
  [4 wires]                               [JST SH plug]
```

**Pros:** Plug-and-play, no soldering on ProS3, reversible
**Cons:** Need to make/buy SAO→STEMMA adapter cable

### XIAO C6/S3: Direct Wiring

```
Badge SAO Port  →  4 Jumper Wires  →  XIAO GPIO Pins
     ↓                                      ↓
  [Solder]                            [Solder]
```

**Pros:** Simple, direct connection
**Cons:** Requires soldering on both sides

---

## Firmware Compatibility

**All three boards are fully supported!** The same firmware files work on all boards:

- `ESP32_S3_Slave.ino` - Basic WiFi/BLE coprocessor
- `ESP32_S3_Enhanced.ino` - WiFi AP + Web LED designer

Just uncomment your board at the top of the file:

```cpp
// Uncomment ONE of these:
// #define BOARD_PROS3        // ESP32 ProS3 (recommended)
#define BOARD_XIAO_C6      // XIAO ESP32-C6 (default)
// #define BOARD_XIAO_S3      // XIAO ESP32-S3
```

Firmware automatically configures correct I2C pins!

---

## Recommendations by Use Case

| Use Case | 1st Choice | 2nd Choice | Why? |
|----------|------------|------------|------|
| **Conference badge** | XIAO C6 | ProS3 | WiFi 6 for crowded networks |
| **Art installation** | ProS3 | XIAO S3 | Flash/PSRAM for complex UI |
| **Prototype/Dev** | ProS3 | XIAO C6 | STEMMA QT for quick iteration |
| **Production (100+)** | XIAO C6 | XIAO S3 | Lower cost, good availability |
| **Battery-powered** | ProS3 | XIAO C6 | 10µA deep sleep, dual LDOs |
| **Learning/Education** | XIAO C6 | ProS3 | Lower cost, modern standards |
| **Maker Fair demo** | ProS3 | XIAO C6 | Reliability, battery charging |
| **Hackathon** | XIAO C6 | ProS3 | Fast soldering, compact |

---

## Where to Buy

### ESP32 ProS3
- **Adafruit:** https://www.adafruit.com/product/5401
- **Pimoroni:** https://shop.pimoroni.com/
- **Unexpected Maker:** https://unexpectedmaker.com/shop
- **Crowd Supply:** https://www.crowdsupply.com/unexpected-maker/pros3

### XIAO ESP32-C6
- **Seeed Studio:** https://www.seeedstudio.com/
- **Digi-Key:** Search "XIAO ESP32-C6"
- **Mouser:** Search "102110908"
- **Amazon:** Available from various sellers

### XIAO ESP32-S3
- **Seeed Studio:** https://www.seeedstudio.com/
- **Digi-Key:** Search "XIAO ESP32-S3"
- **Mouser:** Search "102110250"
- **Amazon:** Available from various sellers

---

## Migration Path

Already have a XIAO C6/S3 but want to upgrade to ProS3?

1. ✅ **Firmware is 100% compatible** - Just change one line!
2. ✅ Update I2C wiring from GPIO6/7 to GPIO8/9
3. ✅ Optional: Use STEMMA QT connector instead of SAO port
4. ✅ Upload same firmware, select `BOARD_PROS3`
5. ✅ Done! Enjoy 16MB Flash and 8MB PSRAM

---

## Still Unsure?

### Start with XIAO ESP32-C6 if:
- You want to minimize cost
- You're at a conference (WiFi 6 advantage)
- You want to learn RISC-V
- Size is critical

### Upgrade to ProS3 if:
- You need 16MB Flash for large web files
- You need 8MB PSRAM for graphics
- You want plug-and-play STEMMA QT
- You need rock-solid power (dual LDOs)
- You want best-in-class deep sleep (10µA)

---

**Bottom Line:** All three boards work great with the Framework Badge. Choose based on your specific needs, budget, and timeline!

For detailed setup instructions:
- ProS3: See `ESP32_PROS3_SETUP.md`
- XIAO C6/S3: See `ESP32_C6_SETUP.md`
- Wiring: See `WIRING.txt`
