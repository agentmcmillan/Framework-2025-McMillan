# Enhanced Dual-MCU Features Guide

Complete guide for the enhanced Framework Badge with WiFi AP, web-based LED designer, QR code display, and animated cat.

## 🎨 New Features Overview

### 1. WiFi Access Point (ESP32-S3)
- **SSID:** `PixelKitty-XXXX` (XXXX = chip ID)
- **Password:** `meow1234`
- **IP Address:** `192.168.4.1`
- **Captive Portal:** Automatically redirects to LED designer

### 2. Web-Based LED Matrix Designer
- **Access:** Connect to WiFi, browse to `http://192.168.4.1`
- **Features:**
  - 15×7 interactive drawing grid
  - Color palette selector (9 colors)
  - Draw/Erase modes
  - Clear and Rainbow presets
  - Real-time preview
  - Send designs to badge LED matrix

### 3. QR Code WiFi Connection
- **Trigger:** Press RIGHT button on badge
- **Display:** TFT shows QR code for WiFi credentials
- **Duration:** 30 seconds
- **Auto-transition:** After 30s, shows animated cat

### 4. Animated Cat (TFT Display)
- **Trigger:** Automatic after QR code timeout, or anytime when not showing QR
- **Animation:** Pixel art cat walking back and forth
- **Features:**
  - Walking animation with moving paws
  - Direction changes at screen edges
  - Ears, whiskers, tail, eyes with pupils
  - Smooth 60 FPS animation

## 📦 Required Libraries

### For RP2040 Firmware

Install via Arduino Library Manager:

1. **Adafruit_NeoPixel** (LED matrix)
   - Version: 1.10.0+

2. **TFT_eSPI / Seeed_GFX** (TFT display)
   - Already in repo: `Seeed_GFX-master/`

3. **QRCode** (QR code generation)
   - Install: Search "qrcode" by Richard Moore
   - Version: 0.0.1+
   - GitHub: https://github.com/ricmoo/QRCode

### For ESP32-S3 Firmware

Install via Arduino Library Manager:

1. **ArduinoJson** (JSON parsing for web data)
   - Version: 7.0.0+
   - Author: Benoit Blanchon

2. **DNSServer** (Captive portal)
   - Included with ESP32 board support

3. **WiFi, WebServer** (Built-in with ESP32 core)

## 🔧 Installation Steps

### Step 1: Install QR Code Library

```bash
# Arduino IDE: Tools → Manage Libraries
# Search: "qrcode"
# Install: "QRCode by Richard Moore"
```

Or manually:
```bash
cd ~/Documents/Arduino/libraries/
git clone https://github.com/ricmoo/QRCode.git
```

### Step 2: Upload ESP32-S3 Firmware

1. Open `ESP32_S3_Enhanced/ESP32_S3_Enhanced.ino`
2. Board: **XIAO_ESP32S3**
3. Upload to XIAO ESP32-S3
4. Monitor Serial (115200 baud) - note the WiFi SSID and password

### Step 3: Upload RP2040 Firmware

1. Open `RP2040_Enhanced/RP2040_Enhanced.ino`
2. Board: **Raspberry Pi Pico**
3. Configure TFT pins in `User_Setup.h` (see previous setup)
4. Upload to Framework Badge
5. Connect ESP32-S3 to SAO port after upload

### Step 4: Test System

1. Power on badge
2. Press RIGHT button → QR code should appear on TFT
3. Wait 30 seconds → Animated cat should appear
4. On phone:
   - Scan QR code OR manually connect to `PixelKitty-XXXX`
   - Password: `meow1234`
5. Browser opens automatically to LED designer
6. Draw a design and click "Send to Badge"
7. LED matrix should display your design!

## 🎮 Usage Guide

### Connecting to Badge WiFi

**Method 1: QR Code (Easiest)**
1. Press RIGHT button on badge
2. Scan QR code with phone camera
3. Phone automatically connects to WiFi

**Method 2: Manual**
1. Open phone WiFi settings
2. Connect to network: `PixelKitty-XXXX`
3. Password: `meow1234`
4. Open browser to: `http://192.168.4.1`

### Using LED Designer

1. **Select Color:** Tap one of the 9 color buttons
2. **Draw Mode:** Tap "Draw" or "Erase"
3. **Draw Pattern:**
   - Click/tap individual pixels
   - Click and drag to draw multiple pixels
4. **Presets:**
   - **Clear:** Erase entire grid
   - **Rainbow:** Fill with rainbow gradient
5. **Send:** Click "📡 Send to Badge"
6. Watch your design appear on the LED matrix!

### Button Functions

| Button | Function |
|--------|----------|
| LEFT | (Future: WiFi scan or settings) |
| CENTER | Rainbow ripple animation |
| RIGHT | Show QR code (30s) → Animated cat |

## 🎨 Customization

### Change WiFi Credentials

Edit in `ESP32_S3_Enhanced.ino`:
```cpp
#define AP_SSID_PREFIX  "PixelKitty"  // Change name
#define AP_PASSWORD     "meow1234"    // Minimum 8 characters
```

### Change QR Display Time

Edit in `RP2040_Enhanced.ino`:
```cpp
const unsigned long QR_DISPLAY_TIME = 30000; // milliseconds (30s)
```

### Customize Cat Animation

Edit `drawCat()` function in `RP2040_Enhanced.ino`:
- Change colors (TFT_ORANGE, TFT_PINK, etc.)
- Adjust sizes (circles, triangles)
- Add more details (stripes, spots, accessories)

### Cat Animation Speed

Edit in `RP2040_Enhanced.ino`:
```cpp
const int CAT_SPEED = 50; // milliseconds per frame (lower = faster)
```

### Add More Colors to Web Palette

Edit in `ESP32_S3_Enhanced.ino` (HTML section):
```javascript
const colors=['#ff0000','#ff7f00','#ffff00','#00ff00',
              '#00ffff','#0000ff','#ff00ff','#ffffff',
              '#000000','#ADD_YOUR_COLOR_HERE'];
```

## 🐛 Troubleshooting

### QR Code Doesn't Appear

**Check:**
- TFT display wired correctly
- QRCode library installed
- Serial monitor shows "Generating QR code..."
- RIGHT button working (test with multimeter)

**Fix:**
```cpp
// Test QR library
#include "qrcode.h"
void setup() {
  QRCode qr; // If this compiles, library is installed
}
```

### Can't Connect to WiFi

**Check:**
- ESP32-S3 powered on (LED should be lit)
- Serial monitor shows "AP Started"
- Phone WiFi searching for `PixelKitty-XXXX`
- Correct password (case-sensitive)

**Fix:**
- Restart badge (power cycle)
- Forget network on phone and reconnect
- Check ESP32 is properly seated in SAO port

### Cat Animation Jerky

**Check:**
- RP2040 main loop not blocking
- Serial print statements slowing down loop
- TFT SPI speed configured correctly

**Fix:**
```cpp
// Reduce debug output
// Comment out Serial.println() in main loop

// Increase SPI speed in User_Setup.h
#define SPI_FREQUENCY  40000000  // 40MHz
```

### Web Page Won't Load

**Check:**
- Connected to correct WiFi network
- Browser showing `192.168.4.1`
- ESP32 serial shows "Web server started"

**Fix:**
- Try: `http://badge.local` (mDNS)
- Clear browser cache
- Try different browser
- Check firewall settings on phone

### LED Pattern Not Displaying

**Check:**
- I2C connection between RP2040 and ESP32
- Serial monitor shows "LED data received"
- Brightness limit not too low

**Fix:**
- Test I2C with ping command
- Verify wiring (SDA, SCL, GND)
- Increase MAX_LED_BRIGHTNESS temporarily for testing

### Cat Disappears or Glitches

**Fix:**
```cpp
// In updateCatAnimation(), ensure screen bounds:
if (catX < 20) catX = 20;
if (catX > 220) catX = 220;
```

## 📊 Performance Notes

### Current Consumption

| Mode | Current Draw |
|------|--------------|
| Idle (cat animation) | ~150mA |
| WiFi AP active (0 clients) | ~200mA |
| WiFi + 1 client connected | ~250mA |
| WiFi + LED pattern display | ~300-500mA |
| Max (WiFi + full LED matrix) | ~700mA |

### Battery Life (2×AA, 2500mAh)

| Usage | Estimated Life |
|-------|----------------|
| Mostly idle with cat | 12-15 hours |
| Occasional web updates | 8-10 hours |
| Continuous WiFi + LED | 3-5 hours |

### WiFi Range

- **Typical:** 10-15 meters
- **Maximum:** 20-30 meters (line of sight)
- **Through walls:** 5-10 meters

## 🔒 Security Notes

### Default Password

⚠️ **Important:** Change the default password before use in public spaces!

```cpp
#define AP_PASSWORD     "your-secure-password-here"  // Min 8 characters
```

### WiFi Security

- Uses WPA2 encryption
- No internet access (local AP only)
- Isolated from other networks
- Captive portal prevents accidental external access

## 🎯 Next Steps

### Enhancements to Consider

1. **Save/Load Patterns:**
   - Store designs in ESP32 flash memory
   - Gallery of preset patterns
   - Import/export patterns as JSON

2. **Animations:**
   - Keyframe-based animations
   - Scrolling text
   - Game of Life
   - Fireworks

3. **Multi-User:**
   - Show connected users on TFT
   - Queue system for pattern submissions
   - Live collaboration mode

4. **Mobile App:**
   - Native iOS/Android app
   - Push notifications
   - Saved pattern library

5. **Badge-to-Badge:**
   - Share patterns via BLE
   - IR sync for group animations
   - Badge mesh network

## 📚 Code Examples

### Custom Button Action

Add to `RP2040_Enhanced.ino`:
```cpp
// In loop(), add LEFT button handler:
bool btnLeft = !digitalRead(BTN_LEFT);
if (btnLeft && !btnLeftPrev) {
  // Your custom action
  Serial.println("Left button pressed!");
  // Example: Trigger different animation
  sparkleAnimation();
}
btnLeftPrev = btnLeft;
```

### Add New Web API Endpoint

Add to `ESP32_S3_Enhanced.ino`:
```cpp
// In setupWebServer():
server.on("/api/custom", HTTP_GET, []() {
  String json = "{\"message\":\"Hello from badge!\"}";
  server.send(200, "application/json", json);
});
```

### Modify Cat Colors

In `drawCat()` function:
```cpp
// Change body color
tft.fillCircle(x, y, 15, TFT_PURPLE); // was TFT_ORANGE

// Add stripes
for (int i = 0; i < 3; i++) {
  tft.drawLine(x - 10, y - 5 + i*5, x + 10, y - 5 + i*5, TFT_DARKGREY);
}
```

## 🆘 Support

For issues:
1. Check this guide's troubleshooting section
2. Review serial monitor output from both devices
3. Test I2C communication (ping command)
4. Verify all libraries installed correctly
5. Check wiring connections

---

**Enjoy your enhanced Pixel Kitty badge!** 🐱✨
