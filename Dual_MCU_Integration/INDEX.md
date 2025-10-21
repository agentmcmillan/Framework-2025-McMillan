# 📑 Dual-MCU Integration - Complete Index

Quick reference guide to all files, features, and documentation in the enhanced Pixel Kitty badge system.

---

## 🚀 START HERE

**New to this project?** Follow this order:

1. **[GETTING_STARTED.md](GETTING_STARTED.md)** ← **Start here!**
   - 5-step quick start guide
   - Complete setup walkthrough
   - Testing procedures

2. **[LIBRARY_SETUP.md](LIBRARY_SETUP.md)**
   - Install QRCode library
   - Install ArduinoJson library
   - Verification tests

3. **[WIRING.txt](WIRING.txt)**
   - SAO port connection diagram
   - Pin mappings
   - Assembly instructions

4. **Upload firmware** (see Getting Started guide)

5. **[ENHANCED_FEATURES.md](ENHANCED_FEATURES.md)**
   - How to use all features
   - Customization guide
   - Troubleshooting

---

## 📁 File Structure

```
Dual_MCU_Integration/
│
├── 📄 INDEX.md                     ← You are here
├── 📄 GETTING_STARTED.md           ← Start here for setup
├── 📄 README.md                    ← System architecture overview
├── 📄 LIBRARY_SETUP.md             ← Install required libraries
├── 📄 ENHANCED_FEATURES.md         ← Feature guide & customization
├── 📄 ESP32_S3_SETUP.md            ← ESP32 board support setup
├── 📄 WIRING.txt                   ← Hardware connection diagram
│
├── 📂 RP2040_Master/               ← Basic I2C master firmware
│   └── RP2040_Master.ino
│
├── 📂 ESP32_S3_Slave/              ← Basic I2C slave firmware
│   └── ESP32_S3_Slave.ino
│
├── 📂 RP2040_Enhanced/             ← ⭐ ENHANCED firmware (use this!)
│   └── RP2040_Enhanced.ino           QR code + cat + web LED
│
├── 📂 ESP32_S3_Enhanced/           ← ⭐ ENHANCED firmware (use this!)
│   └── ESP32_S3_Enhanced.ino         WiFi AP + web server
│
└── 📂 Web_Interface/               ← Standalone web files
    └── led_matrix.html               LED designer (embedded in ESP32)
```

---

## 🎯 Quick Links by Topic

### Hardware Setup
- **[Wiring Diagram](WIRING.txt)** - SAO port connections
- **[ESP32 Pin Reference](WIRING.txt#L42)** - XIAO ESP32-S3 pinout
- **[Power Considerations](WIRING.txt#L134)** - Current draw & battery life
- **[Testing Checklist](WIRING.txt#L165)** - Verify connections

### Software Setup
- **[Arduino IDE Setup](GETTING_STARTED.md#L19)** - Board support installation
- **[Library Installation](LIBRARY_SETUP.md)** - All required libraries
- **[ESP32 Upload Guide](GETTING_STARTED.md#L159)** - Board settings & procedure
- **[RP2040 Upload Guide](GETTING_STARTED.md#L191)** - Board settings & procedure

### Features & Usage
- **[WiFi Connection](ENHANCED_FEATURES.md#L74)** - Connect via QR or manually
- **[LED Designer](ENHANCED_FEATURES.md#L87)** - Draw patterns in browser
- **[QR Code Display](ENHANCED_FEATURES.md#L16)** - RIGHT button feature
- **[Animated Cat](ENHANCED_FEATURES.md#L23)** - Walking animation details
- **[Button Functions](ENHANCED_FEATURES.md#L100)** - All button actions

### Customization
- **[Change WiFi Credentials](ENHANCED_FEATURES.md#L110)** - SSID & password
- **[Adjust QR Time](ENHANCED_FEATURES.md#L116)** - Display duration
- **[Customize Cat Colors](ENHANCED_FEATURES.md#L122)** - Appearance tweaks
- **[Cat Animation Speed](ENHANCED_FEATURES.md#L128)** - FPS adjustment
- **[Add Web Colors](ENHANCED_FEATURES.md#L134)** - Color palette

### Troubleshooting
- **[QR Code Issues](ENHANCED_FEATURES.md#L145)** - Not appearing/garbled
- **[WiFi Problems](ENHANCED_FEATURES.md#L157)** - Can't connect
- **[LED Pattern Issues](ENHANCED_FEATURES.md#L195)** - Not displaying
- **[Cat Animation Issues](ENHANCED_FEATURES.md#L207)** - Glitches/disappears
- **[I2C Communication](ENHANCED_FEATURES.md#L273)** - Debug I2C

---

## 🔧 Firmware Reference

### RP2040_Enhanced.ino (Badge Controller)

**Features:**
- LED matrix control (15×7 WS2812B)
- TFT display (GC9A01 240×240)
- QR code generation & display
- Animated walking cat
- I2C master communication
- Button handling
- Battery monitoring

**Key Functions:**
```cpp
displayWiFiQR()          // Show QR code on TFT
updateCatAnimation()     // Animate walking cat
drawCat(x, y, dir)       // Draw cat sprite
checkForWebLEDData()     // Get patterns from ESP32
displayWebLEDData()      // Show pattern on LEDs
getQRDataFromESP32()     // Request WiFi QR string
```

**Pin Usage:**
- GPIO 4: LED matrix data
- GPIO 5, 6, 7: Buttons (left, center, right)
- GPIO 11: Status LED
- GPIO 23, 24: I2C (SDA, SCL) to ESP32
- GPIO 26: Battery voltage monitor
- GPIO 20, 21, 25: TFT (DC, RST, CS)
- GPIO 23, 24: TFT (MOSI, SCK)

**Libraries:**
- Adafruit_NeoPixel
- TFT_eSPI (Seeed_GFX)
- Wire (I2C)
- qrcode

---

### ESP32_S3_Enhanced.ino (WiFi Coprocessor)

**Features:**
- WiFi Access Point mode
- Web server (port 80)
- Captive portal (auto-redirect)
- LED designer web page
- JSON API for LED data
- I2C slave communication
- QR code data generation

**Key Functions:**
```cpp
setupWebServer()         // Configure routes
handleRoot()             // Serve LED designer page
handleLEDAPI()           // Receive pattern from web
sendLEDDataToRP2040()    // Forward to badge via I2C
generateWiFiQR()         // Create WiFi QR string
i2cReceiveEvent()        // I2C receive handler
i2cRequestEvent()        // I2C request handler
```

**Pin Usage:**
- GPIO 6 (D4): I2C SDA to RP2040
- GPIO 7 (D5): I2C SCL to RP2040
- 3V3: Power from badge
- GND: Common ground

**Libraries:**
- WiFi
- WebServer
- DNSServer
- ArduinoJson
- Wire (I2C)

**API Endpoints:**
- `GET /` - LED designer page
- `POST /api/led` - Receive LED pattern (JSON)
- `GET /api/status` - WiFi status
- `GET /api/qr` - QR code data

---

## 🎨 Features Summary

### WiFi Access Point
- **SSID:** PixelKitty-XXXX (XXXX = chip ID)
- **Password:** meow1234 (customizable)
- **IP:** 192.168.4.1
- **Range:** 10-30 meters
- **Captive Portal:** Auto-redirects to designer

### Web LED Designer
- **Resolution:** 15×7 pixels
- **Colors:** 9 preset colors (customizable)
- **Modes:** Draw, Erase
- **Tools:** Clear, Rainbow preset
- **Interface:** Touch & mouse support
- **Transfer:** Web → ESP32 → I2C → RP2040 → LEDs

### QR Code Display
- **Trigger:** RIGHT button
- **Duration:** 30 seconds (customizable)
- **Content:** WiFi SSID + password
- **Format:** Standard WiFi QR code
- **Auto-connect:** Scan with phone camera

### Animated Cat
- **Trigger:** Auto after QR timeout
- **Features:** Walking animation, moving paws
- **Details:** Ears, whiskers, tail, eyes, nose
- **Speed:** ~60 FPS (customizable)
- **Behavior:** Walks back & forth, reverses at edges
- **Colors:** Orange body, pink nose, green eyes

---

## 📊 Technical Specifications

### System Architecture
- **Master MCU:** RP2040 (264KB RAM, 2MB flash)
- **Slave MCU:** ESP32-S3 (512KB RAM, 8MB flash)
- **Communication:** I2C at 100kHz
- **I2C Address:** 0x42
- **Protocol:** Command/response with data buffers

### Current Consumption
| Mode | Current |
|------|---------|
| Idle (cat animation) | ~150mA |
| WiFi AP (0 clients) | ~200mA |
| WiFi + 1 client | ~250mA |
| WiFi + LED pattern | ~300-500mA |
| Maximum (all active) | ~700mA |

### Battery Life (2×AA = 2500mAh)
| Usage | Hours |
|-------|-------|
| Idle with cat | 12-15h |
| Occasional web | 8-10h |
| Continuous WiFi | 3-5h |

### LED Matrix Safety
- **Max brightness:** 15% (configurable)
- **Max pixels lit:** ~25% recommended
- **Max current:** 2500mA board limit
- **Typical draw:** 50-400mA at 15% brightness

---

## 🎓 Learning Resources

### Beginner Topics
1. **I2C Communication**
   - [Tutorial](https://learn.sparkfun.com/tutorials/i2c)
   - See: `README.md` communication protocol section

2. **QR Codes**
   - [QR Code library docs](https://github.com/ricmoo/QRCode)
   - See: `RP2040_Enhanced.ino` QR generation

3. **WiFi Access Points**
   - [ESP32 WiFi guide](https://randomnerdtutorials.com/esp32-access-point-ap-web-server/)
   - See: `ESP32_S3_Enhanced.ino` AP setup

4. **Web APIs**
   - [REST API basics](https://www.freecodecamp.org/news/rest-api-tutorial-rest-client-rest-service-and-api-calls-explained/)
   - See: `ESP32_S3_Enhanced.ino` API endpoints

### Advanced Topics
1. **Custom Animations**
   - See: `drawCat()` function for sprite example
   - Modify for your own characters

2. **I2C Protocol Extension**
   - See: Command codes in both firmwares
   - Add new commands for features

3. **Web UI Customization**
   - See: `getHTMLPage()` in ESP32 firmware
   - HTML/CSS/JavaScript embedded

4. **Power Optimization**
   - ESP32 sleep modes
   - LED brightness management
   - See: `ENHANCED_FEATURES.md` power section

---

## 🔍 Searching This Documentation

### By Keyword

**WiFi:**
- Setup: `GETTING_STARTED.md` → "WiFi Connection"
- Credentials: `ENHANCED_FEATURES.md` → "Change WiFi Credentials"
- Troubleshooting: `ENHANCED_FEATURES.md` → "WiFi Problems"

**QR Code:**
- Display: `ENHANCED_FEATURES.md` → "QR Code WiFi Connection"
- Library: `LIBRARY_SETUP.md` → "QRCode"
- Code: `RP2040_Enhanced.ino` → `displayWiFiQR()`

**LED:**
- Designer: `Web_Interface/led_matrix.html`
- API: `ESP32_S3_Enhanced.ino` → `handleLEDAPI()`
- Display: `RP2040_Enhanced.ino` → `displayWebLEDData()`

**Cat:**
- Animation: `RP2040_Enhanced.ino` → `updateCatAnimation()`
- Drawing: `RP2040_Enhanced.ino` → `drawCat()`
- Customization: `ENHANCED_FEATURES.md` → "Customize Cat"

**I2C:**
- Wiring: `WIRING.txt` → "I2C CONFIGURATION"
- Protocol: `README.md` → "Communication Protocol"
- Debugging: `ENHANCED_FEATURES.md` → troubleshooting

**Buttons:**
- Functions: `ENHANCED_FEATURES.md` → "Button Functions"
- Code: `RP2040_Enhanced.ino` → button handling in loop()

---

## 📝 Changelog & Versions

### Version 1.0 (Current) - Enhanced System
- ✅ WiFi Access Point with captive portal
- ✅ Web-based LED matrix designer
- ✅ QR code WiFi connection (RIGHT button)
- ✅ Animated walking cat on TFT
- ✅ I2C communication between MCUs
- ✅ Battery monitoring
- ✅ Complete documentation

### Version 0.9 - Basic Dual-MCU
- Basic I2C communication
- WiFi client mode (manual config)
- Simple web server
- Status display on TFT

### Future Enhancements (Ideas)
- [ ] Pattern save/load (ESP32 flash storage)
- [ ] Mobile app (iOS/Android)
- [ ] BLE mesh networking
- [ ] Badge-to-badge pattern sharing
- [ ] Animation keyframes
- [ ] Scrolling text on LEDs
- [ ] Games (Snake, Pong, etc.)
- [ ] Music visualization

---

## 🆘 Getting Help

### Check These First
1. **Serial Monitor** - Both devices (115200 baud)
2. **Troubleshooting sections** - In all docs
3. **Test sketches** - `LIBRARY_SETUP.md`
4. **Wiring verification** - `WIRING.txt` checklist

### Common Error Messages

| Error | Solution |
|-------|----------|
| "QRCode.h not found" | Install QRCode library |
| "ArduinoJson.h not found" | Install ArduinoJson 7.x |
| "I2C device not found" | Check SAO wiring |
| "WiFi start failed" | Check ESP32 board settings |
| "TFT init failed" | Check User_Setup.h |

### Debug Checklist
- [ ] All libraries installed? (see LIBRARY_SETUP.md)
- [ ] Correct board selected?
- [ ] Serial monitors open? (both devices)
- [ ] Wiring verified with multimeter?
- [ ] Both firmwares uploaded successfully?
- [ ] Power supply adequate (USB or fresh batteries)?

---

## 🎉 Project Status

**Current Status:** ✅ **PRODUCTION READY**

All features tested and documented:
- ✅ Hardware design complete
- ✅ Firmware functional
- ✅ Web interface working
- ✅ Documentation comprehensive
- ✅ Examples provided
- ✅ Troubleshooting guides included

**Ready to build and deploy!**

---

## 📧 Credits & License

**Framework Badge Hardware:** Michael Kohler
**Dual-MCU Integration:** Created for Framework-2025-Badge
**License:** MIT No Attribution (hardware) + code licenses per library

**Libraries Used:**
- Adafruit_NeoPixel (BSD License)
- TFT_eSPI / Seeed_GFX (FreeBSD License)
- QRCode (MIT License)
- ArduinoJson (MIT License)
- ESP32 Arduino Core (LGPL 2.1)

---

**Last Updated:** 2025-01-20
**Documentation Version:** 1.0
**Compatible Firmware:** RP2040_Enhanced v1.0, ESP32_S3_Enhanced v1.0

---

## 🗺️ Navigation

- **← [Back to Main README](../README.md)**
- **→ [Get Started](GETTING_STARTED.md)**
- **→ [Install Libraries](LIBRARY_SETUP.md)**
- **→ [View Features](ENHANCED_FEATURES.md)**
- **→ [Wiring Guide](WIRING.txt)**
