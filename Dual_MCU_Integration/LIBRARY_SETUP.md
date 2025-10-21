# Required Libraries Installation Guide

Step-by-step instructions for installing all required libraries for the enhanced dual-MCU system.

## Overview

This project requires libraries for both microcontrollers:
- **RP2040 (Framework Badge):** 3 libraries
- **ESP32-S3 (XIAO):** 1 library + built-in libraries

---

## RP2040 Libraries

### 1. Adafruit NeoPixel (LED Matrix Control)

**Installation via Library Manager:**

1. Open Arduino IDE
2. Go to **Tools → Manage Libraries...**
3. Search: `Adafruit NeoPixel`
4. Find: **Adafruit NeoPixel by Adafruit**
5. Click **Install** (version 1.10.0 or newer)
6. Click **Close**

**Manual Installation (Alternative):**
```bash
cd ~/Documents/Arduino/libraries/
git clone https://github.com/adafruit/Adafruit_NeoPixel.git
```

**Verification:**
```cpp
// Test sketch
#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel strip(105, 4, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show(); // If compiles, library is installed!
}
void loop() {}
```

---

### 2. Seeed_GFX (TFT Display Driver)

**Already in Repository!**

The Seeed_GFX library is already included in your repo:
```
Framework-2025-Badge/Seeed_GFX-master/
```

**Installation:**

**Option A: Symbolic Link (Recommended)**
```bash
cd ~/Documents/Arduino/libraries/
ln -s ~/Documents/Framework-2025-Badge/Seeed_GFX-master Seeed_GFX
```

**Option B: Copy**
```bash
cp -r ~/Documents/Framework-2025-Badge/Seeed_GFX-master ~/Documents/Arduino/libraries/Seeed_GFX
```

**Configuration:**

Copy the TFT configuration file:
```bash
cp ~/Documents/Framework-2025-Badge/Badge_LED_TFT_Demo/User_Setup.h \
   ~/Documents/Arduino/libraries/Seeed_GFX/User_Setup.h
```

**Verification:**
```cpp
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

void setup() {
  tft.init(); // If compiles, library is installed!
}
void loop() {}
```

---

### 3. QRCode (QR Code Generation) ⭐ NEW

**Installation via Library Manager:**

1. Open Arduino IDE
2. Go to **Tools → Manage Libraries...**
3. Search: `qrcode`
4. Find: **QRCode by Richard Moore**
5. Click **Install** (version 0.0.1 or newer)
6. Click **Close**

**Manual Installation (Alternative):**
```bash
cd ~/Documents/Arduino/libraries/
git clone https://github.com/ricmoo/QRCode.git
```

**Verification:**
```cpp
#include "qrcode.h"

void setup() {
  Serial.begin(115200);

  QRCode qrcode;
  uint8_t qrcodeData[qrcode_getBufferSize(3)];
  qrcode_initText(&qrcode, qrcodeData, 3, 0, "TEST");

  Serial.println("QR Code library working!");
  Serial.print("QR Size: ");
  Serial.println(qrcode.size); // Should print: 29
}
void loop() {}
```

**Library Details:**
- **Author:** Richard Moore
- **GitHub:** https://github.com/ricmoo/QRCode
- **License:** MIT
- **Size:** ~10KB
- **Features:** Generate QR codes up to version 40

---

## ESP32-S3 Libraries

### 1. ArduinoJson (JSON Parsing) ⭐ NEW

**Installation via Library Manager:**

1. Open Arduino IDE
2. Go to **Tools → Manage Libraries...**
3. Search: `ArduinoJson`
4. Find: **ArduinoJson by Benoit Blanchon**
5. Click **Install** (version 7.0.0 or newer)
6. Click **Close**

**Manual Installation (Alternative):**
```bash
cd ~/Documents/Arduino/libraries/
git clone https://github.com/bblanchon/ArduinoJson.git
```

**Verification:**
```cpp
#include <ArduinoJson.h>

void setup() {
  Serial.begin(115200);

  StaticJsonDocument<200> doc;
  doc["test"] = "working";

  String output;
  serializeJson(doc, output);

  Serial.println(output); // Should print: {"test":"working"}
}
void loop() {}
```

**Library Details:**
- **Author:** Benoit Blanchon
- **Version:** 7.x (latest)
- **GitHub:** https://github.com/bblanchon/ArduinoJson
- **Documentation:** https://arduinojson.org/
- **Size:** ~50KB

---

### 2. Built-in ESP32 Libraries

These are included with ESP32 board support (no installation needed):

**WiFi** - WiFi client and AP functionality
```cpp
#include <WiFi.h>
```

**WebServer** - HTTP server for web interface
```cpp
#include <WebServer.h>
```

**DNSServer** - DNS server for captive portal
```cpp
#include <DNSServer.h>
```

**Wire** - I2C communication
```cpp
#include <Wire.h>
```

**Verification:**
All these should compile without errors after ESP32 board support is installed.

---

## Complete Library Checklist

### Before Uploading RP2040 Firmware

Check these libraries are installed:

- [ ] **Adafruit_NeoPixel** (for LED matrix)
  - Test: `#include <Adafruit_NeoPixel.h>` compiles

- [ ] **Seeed_GFX / TFT_eSPI** (for TFT display)
  - Test: `#include <TFT_eSPI.h>` compiles
  - Test: `User_Setup.h` configured for RP2040 + GC9A01

- [ ] **QRCode** (for QR code generation)
  - Test: `#include "qrcode.h"` compiles
  - Test: Create simple QR code in setup()

### Before Uploading ESP32-S3 Firmware

Check these libraries are installed:

- [ ] **ArduinoJson** (for web API)
  - Test: `#include <ArduinoJson.h>` compiles

- [ ] **WiFi, WebServer, DNSServer** (built-in)
  - Test: All three includes compile

---

## Troubleshooting

### "QRCode.h: No such file or directory"

**Solution 1: Verify Installation Path**
```bash
# Check if library exists
ls ~/Documents/Arduino/libraries/QRCode/

# Should see: qrcode.h, qrcode.c, etc.
```

**Solution 2: Check Library Manager**
- Tools → Manage Libraries
- Search "qrcode"
- If installed, should show "INSTALLED"
- If not, click Install

**Solution 3: Restart Arduino IDE**
- Close Arduino IDE completely
- Reopen
- Try compiling again

---

### "ArduinoJson.h: No such file or directory"

**Solution 1: Check Version**
- Library Manager might show ArduinoJson 6.x
- Make sure to install version **7.x or newer**
- Uninstall old version first if needed

**Solution 2: Manual Installation**
```bash
cd ~/Documents/Arduino/libraries/
rm -rf ArduinoJson  # Remove old version
git clone https://github.com/bblanchon/ArduinoJson.git
cd ArduinoJson
git checkout 7.x  # Use latest 7.x branch
```

---

### "TFT_eSPI.h: No such file or directory"

**Solution: Verify Seeed_GFX Setup**
```bash
# Check if library exists
ls ~/Documents/Arduino/libraries/Seeed_GFX/

# Should see: TFT_eSPI.h, User_Setup.h, etc.

# If not, create symlink or copy:
ln -s ~/Documents/Framework-2025-Badge/Seeed_GFX-master \
      ~/Documents/Arduino/libraries/Seeed_GFX
```

---

### Compilation Errors with QRCode

**Error:** `'qrcode_getBufferSize' was not declared`

**Solution:** Update include statement
```cpp
// Wrong:
#include <qrcode.h>

// Correct:
#include "qrcode.h"  // Use quotes, not brackets
```

---

### Compilation Errors with ArduinoJson

**Error:** `StaticJsonDocument does not name a type`

**Solution:** Check version
```cpp
// ArduinoJson 7.x syntax:
#include <ArduinoJson.h>

JsonDocument doc;  // New in v7
// or
StaticJsonDocument<200> doc;  // Still works in v7
```

---

## Quick Test Sketch

Use this to verify all RP2040 libraries are working:

```cpp
#include <Adafruit_NeoPixel.h>
#include <TFT_eSPI.h>
#include "qrcode.h"

Adafruit_NeoPixel strip(105, 4, NEO_GRB + NEO_KHZ800);
TFT_eSPI tft = TFT_eSPI();

void setup() {
  Serial.begin(115200);
  delay(2000);

  Serial.println("Testing libraries...");

  // Test NeoPixel
  strip.begin();
  strip.setPixelColor(0, strip.Color(10, 0, 0));
  strip.show();
  Serial.println("✓ NeoPixel OK");

  // Test TFT
  tft.init();
  tft.fillScreen(TFT_BLACK);
  Serial.println("✓ TFT OK");

  // Test QRCode
  QRCode qrcode;
  uint8_t qrcodeData[qrcode_getBufferSize(3)];
  qrcode_initText(&qrcode, qrcodeData, 3, 0, "TEST");
  Serial.print("✓ QRCode OK (size: ");
  Serial.print(qrcode.size);
  Serial.println(")");

  Serial.println("\nAll libraries working!");
}

void loop() {}
```

**Expected Serial Output:**
```
Testing libraries...
✓ NeoPixel OK
✓ TFT OK
✓ QRCode OK (size: 29)

All libraries working!
```

---

## Quick Test for ESP32-S3

```cpp
#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <ArduinoJson.h>

void setup() {
  Serial.begin(115200);
  delay(2000);

  Serial.println("Testing ESP32 libraries...");

  // Test WiFi
  WiFi.mode(WIFI_AP);
  Serial.println("✓ WiFi OK");

  // Test WebServer
  WebServer server(80);
  Serial.println("✓ WebServer OK");

  // Test DNSServer
  DNSServer dns;
  Serial.println("✓ DNSServer OK");

  // Test ArduinoJson
  JsonDocument doc;
  doc["test"] = "working";
  String output;
  serializeJson(doc, output);
  Serial.print("✓ ArduinoJson OK: ");
  Serial.println(output);

  Serial.println("\nAll ESP32 libraries working!");
}

void loop() {}
```

**Expected Output:**
```
Testing ESP32 libraries...
✓ WiFi OK
✓ WebServer OK
✓ DNSServer OK
✓ ArduinoJson OK: {"test":"working"}

All ESP32 libraries working!
```

---

## Library Sizes (Flash/RAM Impact)

| Library | Flash Usage | RAM Usage |
|---------|-------------|-----------|
| Adafruit_NeoPixel | ~5 KB | ~500 bytes |
| Seeed_GFX | ~40 KB | ~2 KB |
| QRCode | ~3 KB | ~1 KB |
| ArduinoJson | ~15 KB | ~200 bytes |
| **Total (RP2040)** | ~48 KB | ~3.5 KB |
| **Total (ESP32-S3)** | ~15 KB | ~200 bytes |

**RP2040 Resources:**
- Flash: 2 MB (project uses ~48 KB = 2.4%)
- RAM: 264 KB (project uses ~40 KB = 15%)

**ESP32-S3 Resources:**
- Flash: 8 MB (project uses ~50 KB = 0.6%)
- RAM: 512 KB (project uses ~100 KB = 20%)

Both have plenty of space for expansion!

---

## Next Steps

After all libraries are installed:

1. ✓ Verify all libraries with test sketches
2. → Copy `User_Setup.h` to Seeed_GFX folder
3. → Upload `ESP32_S3_Enhanced.ino` to XIAO
4. → Upload `RP2040_Enhanced.ino` to Framework Badge
5. → Connect ESP32 to SAO port
6. → Test complete system

See `ENHANCED_FEATURES.md` for full usage instructions.

---

## Support Resources

- **Adafruit NeoPixel:** https://learn.adafruit.com/adafruit-neopixel-uberguide
- **TFT_eSPI:** https://github.com/Bodmer/TFT_eSPI
- **QRCode:** https://github.com/ricmoo/QRCode
- **ArduinoJson:** https://arduinojson.org/v7/doc/

If you encounter issues not covered here, check the serial monitor output for specific error messages.
