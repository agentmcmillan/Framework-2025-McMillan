# 🚀 Getting Started - Enhanced Pixel Kitty Badge

Complete step-by-step guide to set up your Framework Badge with ESP32-S3 WiFi integration, web-based LED designer, QR code display, and animated cat.

## 📋 What You'll Need

### Hardware
- ✓ Framework Badge (RP2040-based with 15×7 LED matrix)
- ✓ Seeed XIAO ESP32-S3
- ✓ GC9A01 Round TFT Display (240×240)
- ✓ 4× female-to-female jumper wires (for SAO connection)
- ✓ USB-C cable (for programming)
- ✓ 2× AA batteries (for portable operation)
- ✓ Smartphone or computer (for web interface)

### Software
- ✓ Arduino IDE 2.x (or 1.8.19+)
- ✓ RP2040 board support
- ✓ ESP32 board support

### Time Required
- First-time setup: ~45 minutes
- Subsequent uploads: ~5 minutes

---

## 🎯 Quick Start (5 Steps)

### Step 1: Install Arduino Board Support

**RP2040:**
```
1. File → Preferences
2. Additional Board Manager URLs:
   https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json
3. Tools → Board Manager → Install "Raspberry Pi Pico/RP2040"
```

**ESP32-S3:**
```
1. File → Preferences
2. Additional Board Manager URLs (add comma after RP2040 URL):
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
3. Tools → Board Manager → Install "esp32" by Espressif
```

### Step 2: Install Required Libraries

Open Arduino IDE → Tools → Manage Libraries, then search and install:

**For RP2040:**
- Adafruit NeoPixel (v1.10.0+)
- QRCode (v0.0.1+) by Richard Moore

**For ESP32-S3:**
- ArduinoJson (v7.0.0+) by Benoit Blanchon

**Already in repo:**
- Seeed_GFX (symlink from `Seeed_GFX-master/`)

See `LIBRARY_SETUP.md` for detailed installation instructions.

### Step 3: Upload ESP32-S3 Firmware

```
1. Open: Dual_MCU_Integration/ESP32_S3_Enhanced/ESP32_S3_Enhanced.ino
2. Select Board: Tools → Board → esp32 → XIAO_ESP32S3
3. Select Port: Tools → Port → (your XIAO port)
4. Configure:
   - USB CDC On Boot: Enabled
   - CPU Frequency: 240MHz (WiFi)
   - Flash Size: 8MB
5. Click Upload
6. Open Serial Monitor (115200 baud)
7. Note the WiFi SSID shown (e.g., "PixelKitty-A1B2")
```

**Expected Serial Output:**
```
=== XIAO ESP32-S3 Enhanced ===
WiFi AP + Web Server + LED Control
AP SSID: PixelKitty-A1B2
AP Password: meow1234
✓ AP Started - IP: 192.168.4.1
✓ Web server started on port 80
Ready! Connect to WiFi and browse to: http://192.168.4.1
```

### Step 4: Upload RP2040 Firmware

```
1. Disconnect XIAO from USB
2. Connect Framework Badge to USB
3. Enter bootloader: Hold BOOT, tap RESET
4. Open: Dual_MCU_Integration/RP2040_Enhanced/RP2040_Enhanced.ino
5. Select Board: Tools → Board → Raspberry Pi Pico
6. Select Port: Tools → Port → (badge shows as USB drive)
7. Click Upload
8. Press RESET on badge after upload
9. Open Serial Monitor (115200 baud)
```

**Expected Serial Output:**
```
=== Framework Badge Enhanced ===
Setup complete!
```

### Step 5: Connect Hardware & Test

```
1. Power off badge (remove USB, remove batteries)
2. Connect XIAO ESP32-S3 to badge SAO port:
   - SAO Pin 1 (VCC) → XIAO 3V3
   - SAO Pin 2 (GND) → XIAO GND
   - SAO Pin 3 (SDA) → XIAO D4 (GPIO6)
   - SAO Pin 4 (SCL) → XIAO D5 (GPIO7)
3. Power on badge (USB or batteries)
4. Press RIGHT button → QR code should appear on TFT
5. Scan QR code with phone camera
6. Phone connects to WiFi automatically
7. LED designer page opens in browser
8. Draw a pattern and click "Send to Badge"
9. Pattern appears on LED matrix!
10. Wait 30 seconds → Animated cat appears on TFT
```

---

## 🎨 Detailed Setup Guide

### Part 1: Arduino IDE Configuration

#### Install Arduino IDE

**Download:** https://www.arduino.cc/en/software

**Recommended:** Arduino IDE 2.3.2 or newer

**Minimum:** Arduino IDE 1.8.19

#### Configure Preferences

```
File → Preferences → Additional Board Manager URLs:

https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json,https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
```

(Copy entire line above as one continuous URL)

#### Install Board Support Packages

**RP2040:**
1. Tools → Board → Boards Manager
2. Search: "pico"
3. Install: "Raspberry Pi Pico/RP2040 by Earle F. Philhower, III"
4. Wait for installation (~5 minutes)

**ESP32:**
1. Tools → Board → Boards Manager
2. Search: "esp32"
3. Install: "esp32 by Espressif Systems" (version 3.3.2)
4. Wait for installation (~10 minutes, downloads ~27MB)

### Part 2: Library Installation

See `LIBRARY_SETUP.md` for complete details. Quick summary:

```
Tools → Manage Libraries → Search and install:

✓ Adafruit NeoPixel
✓ ArduinoJson (version 7.x)
✓ QRCode (by Richard Moore)
```

**Verify installation:**
```cpp
#include <Adafruit_NeoPixel.h> // RP2040
#include <ArduinoJson.h>       // ESP32
#include "qrcode.h"            // RP2040
```
All should compile without errors.

### Part 3: Hardware Assembly

#### Connect TFT Display to Badge

Follow wiring from `Badge_LED_TFT_Demo/WIRING.txt`:

```
Badge Whisker Port:
  Pin 1 (+5V)    → TFT VCC/LED
  Pin 2 (GND)    → TFT GND
  Pin 3 (IO#23)  → TFT MOSI
  Pin 4 (IO#24)  → TFT SCK

Additional:
  IO#25 → TFT CS
  IO#20 → TFT DC
  IO#21 → TFT RST
```

#### Connect ESP32-S3 to Badge SAO Port

Follow wiring from `Dual_MCU_Integration/WIRING.txt`:

```
Badge SAO Port → XIAO ESP32-S3:
  Pin 1 (VCC)    → 3V3 or 5V
  Pin 2 (GND)    → GND
  Pin 3 (SDA)    → D4 (GPIO6)
  Pin 4 (SCL)    → D5 (GPIO7)
```

**SAO Port Location:**
- 2×2 pin header on **top** of badge
- Pin 1 = VCC (verify with multimeter: should be 3.3V)

#### Verify Connections

**Before powering on:**
1. Check VCC is 3.3V (not 5V!) for ESP32
2. Verify GND connections with continuity test
3. No short circuits between VCC and GND
4. All SPI/I2C connections secure

### Part 4: Firmware Upload

#### ESP32-S3 Firmware

**File:** `Dual_MCU_Integration/ESP32_S3_Enhanced/ESP32_S3_Enhanced.ino`

**Board Settings:**
```
Board: XIAO_ESP32S3
USB CDC On Boot: Enabled
USB Mode: Hardware CDC and JTAG
Upload Speed: 921600
CPU Frequency: 240MHz (WiFi)
Flash Size: 8MB (Quad SPI)
Partition: Default 4MB with spiffs
PSRAM: OPI PSRAM
```

**Upload Process:**
1. Connect XIAO to computer via USB-C
2. Hold BOOT button
3. Tap RESET button
4. Release BOOT button
5. Click Upload in Arduino IDE
6. Wait ~30 seconds
7. Open Serial Monitor (115200 baud)

**Success Indicators:**
- Serial shows "AP Started"
- Note the SSID: "PixelKitty-XXXX"
- Note password: "meow1234"

#### RP2040 Firmware

**File:** `Dual_MCU_Integration/RP2040_Enhanced/RP2040_Enhanced.ino`

**Before uploading:**
1. Copy `User_Setup.h` to Seeed_GFX library folder:
   ```bash
   cp Badge_LED_TFT_Demo/User_Setup.h \
      ~/Documents/Arduino/libraries/Seeed_GFX/
   ```

**Board Settings:**
```
Board: Raspberry Pi Pico
CPU Speed: 133 MHz
Flash Size: 2MB (FS: 1MB)
USB Stack: Adafruit TinyUSB
```

**Upload Process:**
1. Disconnect ESP32 from badge
2. Connect badge to computer via USB-C
3. Hold BOOT button on badge
4. Tap RESET button on badge
5. Badge appears as USB drive
6. Click Upload in Arduino IDE
7. Wait ~30 seconds
8. Press RESET button after upload

**Success Indicators:**
- TFT displays "PIXEL KITTY Ready!"
- Center LED breathing animation
- Serial shows "Setup complete!"

### Part 5: System Testing

#### Test 1: QR Code Display

```
1. Press RIGHT button on badge
2. TFT should display QR code within 1 second
3. QR code should be readable (scan with phone)
4. Phone should prompt to join WiFi network
```

**Troubleshooting:**
- No QR code? Check Serial monitor for "Generating QR code..."
- QR code garbled? QRCode library not installed
- TFT blank? Check TFT wiring and User_Setup.h

#### Test 2: WiFi Connection

```
1. On phone: Scan QR code OR manually connect
   Network: PixelKitty-XXXX
   Password: meow1234
2. Phone should auto-connect
3. Browser should open to http://192.168.4.1
4. LED designer page should load
```

**Troubleshooting:**
- Can't find network? ESP32 Serial monitor should show "AP Started"
- Wrong password? Check ESP32_S3_Enhanced.ino AP_PASSWORD
- Page won't load? Try http://192.168.4.1 manually

#### Test 3: LED Pattern Transfer

```
1. In LED designer:
   - Select red color
   - Draw a smiley face
   - Click "Send to Badge"
2. Should see "✓ Sent!" message
3. Badge LED matrix should display your pattern
```

**Troubleshooting:**
- Pattern not appearing? Check I2C connections (SDA, SCL)
- Serial shows "LED data received"? I2C working
- All black? Check brightness (MAX_LED_BRIGHTNESS)

#### Test 4: Animated Cat

```
1. Wait 30 seconds after showing QR code
2. Or just don't press any buttons
3. Cat should walk across TFT screen
4. Cat changes direction at edges
5. Paws should animate (walking motion)
```

**Troubleshooting:**
- Cat not moving? Check TFT library
- Cat disappears? Screen bounds issue in code
- Flickers? Increase SPI_FREQUENCY in User_Setup.h

---

## 🎮 Using Your Badge

### Connect to WiFi

**Option 1: QR Code**
1. Press RIGHT button
2. Scan with phone camera
3. Auto-connects

**Option 2: Manual**
1. WiFi settings on phone
2. Connect to "PixelKitty-XXXX"
3. Enter password: "meow1234"

### Draw LED Patterns

1. Browser opens automatically to LED designer
2. Or manually: http://192.168.4.1
3. Select color from palette
4. Click/tap pixels to draw
5. Use "Draw" or "Erase" mode
6. Click "Send to Badge"
7. Watch LED matrix update!

### Button Functions

| Button | Action |
|--------|--------|
| LEFT | (Available for custom features) |
| CENTER | Rainbow ripple animation |
| RIGHT | QR code (30s) → Animated cat |

### Battery Monitoring

- RP2040 monitors battery voltage (IO#26)
- Sends to ESP32 every 5 seconds
- Check Serial monitor for voltage readings
- Low battery: <2.3V = auto-disable WiFi

---

## 🔧 Customization

### Change WiFi Name/Password

Edit `ESP32_S3_Enhanced/ESP32_S3_Enhanced.ino`:
```cpp
#define AP_SSID_PREFIX  "MyBadge"     // Your name
#define AP_PASSWORD     "secret123"   // Your password (8+ chars)
```

### Adjust QR Display Time

Edit `RP2040_Enhanced/RP2040_Enhanced.ino`:
```cpp
const unsigned long QR_DISPLAY_TIME = 60000; // 60 seconds
```

### Customize Cat

Edit `drawCat()` function colors:
```cpp
tft.fillCircle(x, y, 15, TFT_BLUE);  // Blue cat!
```

### Add More Colors

Edit ESP32 HTML colors array:
```javascript
const colors=['#ff0000','#00ff00','#0000ff']; // Add more!
```

---

## 📚 Documentation Reference

| File | Purpose |
|------|---------|
| `README.md` | System architecture overview |
| `LIBRARY_SETUP.md` | Detailed library installation |
| `ENHANCED_FEATURES.md` | Feature guide and customization |
| `WIRING.txt` | Hardware connections diagram |
| `ESP32_S3_SETUP.md` | ESP32 board configuration |
| `GETTING_STARTED.md` | This file |

---

## 🆘 Common Issues

### "Library not found" errors

→ See `LIBRARY_SETUP.md` for installation instructions

### ESP32 won't enter bootloader

→ Hold BOOT, tap RESET, release BOOT

### TFT display blank

→ Check `User_Setup.h` copied to Seeed_GFX folder

### I2C communication fails

→ Verify SAO port wiring (SDA, SCL, GND)

### WiFi won't start

→ ESP32 Serial monitor should show "AP Started"

### LED patterns don't appear

→ Check I2C connection and Serial for "LED data received"

---

## ✅ Success Checklist

After completing setup, you should have:

- [ ] TFT displays "PIXEL KITTY Ready!" on boot
- [ ] CENTER button triggers rainbow ripple
- [ ] RIGHT button shows QR code for 30 seconds
- [ ] Animated cat appears after QR timeout
- [ ] Phone can scan QR code and connect to WiFi
- [ ] LED designer page loads in browser
- [ ] Drawing patterns and sending to badge works
- [ ] LED matrix displays web-designed patterns
- [ ] Battery voltage monitoring works
- [ ] All Serial monitors show expected output

---

## 🎉 You're Ready!

Your enhanced Pixel Kitty badge is now complete with:
- ✨ WiFi access point
- 🎨 Web-based LED designer
- 📱 QR code WiFi connection
- 🐱 Animated walking cat
- 🌈 Rainbow LED animations
- 🔋 Battery monitoring

**Enjoy your badge! 😺**

For advanced features and customization, see `ENHANCED_FEATURES.md`.

For troubleshooting, check Serial monitor output from both devices.
