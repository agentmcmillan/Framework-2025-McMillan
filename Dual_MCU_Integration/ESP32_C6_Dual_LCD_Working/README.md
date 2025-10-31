# ESP32-C6 Dual Round LCD Cat Eyes

An animated cat eyes display using two GC9A01 round LCD screens controlled by ESP32-C6 with WiFi 6 web interface.

![Cat Eyes Animation](https://img.shields.io/badge/Status-Ready-brightgreen) ![Platform](https://img.shields.io/badge/Platform-ESP32--C6-blue) ![Displays](https://img.shields.io/badge/Displays-2×GC9A01-orange)

## 📦 Firmware Versions

This project includes **two firmware versions**:

1. **`ESP32_C6_Dual_LCD_Adafruit.ino`** ⭐ **RECOMMENDED for ESP32-C6**
   - Uses Arduino_GFX library with full ESP32-C6 support
   - RISC-V architecture compatible
   - Will compile and run without issues
   - **Use this version**

2. **`ESP32_C6_Dual_LCD.ino`** (Reference only)
   - Uses TFT_eSPI library
   - **Does not compile on ESP32-C6** due to hardware register incompatibilities
   - Kept for reference and potential ESP32/ESP32-S3 ports

**Both versions have identical features and web interface - only the display library differs.**

## ✨ Features

- **Dual Round Displays**: Two 240×240 pixel GC9A01 round LCDs acting as cat eyes
- **Smooth Animations**: Eyes look left, right, up, down, and diagonals with smooth transitions
- **Auto Blinking**: Realistic automatic blinking every few seconds
- **Web Control**: WiFi 6 access point with mobile-friendly web interface
- **Color Customization**: Change eye color (green, blue, red, yellow, magenta, cyan)
- **Low Latency**: Responsive controls with <50ms reaction time
- **Portable**: Runs on USB power or battery

## 🎥 Demo

The cat eyes respond naturally to direction commands:
- Look left/right with synchronized eye movement
- Pupils move independently but naturally (mirrored for realism)
- Smooth easing animation creates organic movement
- Automatic blinking adds life to the display

## 🔌 Hardware Requirements

### Required Components

| Component | Quantity | Notes |
|-----------|----------|-------|
| Seeed XIAO ESP32-C6 | 1 | Main controller with WiFi 6 |
| GC9A01 Round LCD (240×240) | 2 | 1.28" round displays |
| Breadboard | 1 | For prototyping |
| Jumper Wires | ~15 | Male-to-Female recommended |
| USB-C Cable | 1 | For power and programming |

### Optional Components

- 3.7V LiPo Battery (1000mAh+) for portable operation
- 3D-printed case for displays
- Diffuser film for softer eye effect
- Power switch

## 📐 Pin Connections

### Display 1 (Left Eye)
| GC9A01 Pin | ESP32-C6 Pin | Function |
|------------|--------------|----------|
| VCC | 3.3V | Power |
| GND | GND | Ground |
| SCK | GPIO20 (D8) | SPI Clock |
| MOSI/SDA | GPIO21 (D9) | SPI Data |
| CS | GPIO2 (D0) | Chip Select |
| DC | GPIO3 (D1) | Data/Command |
| RST | GPIO4 (D2) | Reset |
| BL | 3.3V | Backlight |

### Display 2 (Right Eye)
| GC9A01 Pin | ESP32-C6 Pin | Function |
|------------|--------------|----------|
| VCC | 3.3V | Power (shared) |
| GND | GND | Ground (shared) |
| SCK | GPIO20 (D8) | SPI Clock (shared) |
| MOSI/SDA | GPIO21 (D9) | SPI Data (shared) |
| CS | GPIO5 (D3) | Chip Select |
| DC | GPIO6 (D4) | Data/Command |
| RST | GPIO7 (D5) | Reset |
| BL | 3.3V | Backlight |

**See `WIRING_DUAL_LCD.txt` for detailed wiring diagrams and troubleshooting.**

## 🚀 Getting Started

### 1. Install Arduino IDE

Download Arduino IDE 2.x from https://www.arduino.cc/en/software

### 2. Install ESP32-C6 Board Support

1. Open Arduino IDE
2. Go to **File → Preferences**
3. Add to **Additional Board Manager URLs**:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
4. Go to **Tools → Board → Boards Manager**
5. Search "esp32" and install "esp32 by Espressif Systems"
6. Select **Tools → Board → ESP32 Arduino → XIAO_ESP32C6**

### 3. Install Required Libraries

Go to **Tools → Manage Libraries** and install:

- **TFT_eSPI** by Bodmer (version 2.5.0 or later)
  - After installing, you need to configure it for GC9A01
  - See Configuration section below

Built-in libraries (no installation needed):
- WiFi
- WebServer
- DNSServer
- SPI

### 4. Configure TFT_eSPI for GC9A01

The TFT_eSPI library needs configuration for the GC9A01 displays:

**Option A: Create User_Setup.h in sketch folder** (Recommended)

Create a file `User_Setup.h` in the same folder as `ESP32_C6_Dual_LCD.ino`:

```cpp
#define GC9A01_DRIVER
#define TFT_WIDTH  240
#define TFT_HEIGHT 240

#define TFT_MOSI 21
#define TFT_SCLK 20
#define TFT_CS   2  // Will be overridden in code
#define TFT_DC   3  // Will be overridden in code
#define TFT_RST  4  // Will be overridden in code

#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_FONT6
#define LOAD_FONT7
#define LOAD_FONT8

#define SPI_FREQUENCY  40000000
#define SPI_READ_FREQUENCY  20000000
#define SPI_TOUCH_FREQUENCY  2500000
```

**Option B: Edit library User_Setup.h** (Alternative)

Locate the TFT_eSPI library folder:
- Windows: `Documents\Arduino\libraries\TFT_eSPI\`
- Mac: `~/Documents/Arduino/libraries/TFT_eSPI/`
- Linux: `~/Arduino/libraries/TFT_eSPI/`

Edit `User_Setup.h` and uncomment/add the lines from Option A.

### 5. Upload Firmware

1. Connect XIAO ESP32-C6 via USB-C
2. Select **Tools → Port** (select your ESP32-C6 port)
3. Configure board settings:
   - **USB CDC On Boot**: Enabled
   - **CPU Frequency**: 160MHz (WiFi 6)
   - **Flash Size**: 4MB
   - **Upload Speed**: 921600
4. Open `ESP32_C6_Dual_LCD.ino`
5. Click **Upload** button
6. Wait for compilation and upload (~1 minute)

### 6. Connect Displays

Follow the wiring guide in `WIRING_DUAL_LCD.txt` to connect both GC9A01 displays.

### 7. Power On and Connect

1. Power on the ESP32-C6
2. Both displays should light up showing cat eyes looking forward
3. Connect your phone/computer to WiFi network **"CatEyes-XXXX"** (XXXX is unique ID)
4. Password: **meow1234**
5. Browser should auto-open, or navigate to **http://192.168.4.1**

## 🎮 Web Interface Usage

The web interface provides intuitive control:

### Eye Direction Control

Use the 3×3 grid of directional buttons:
- **⬆️ ⬇️ ⬅️ ➡️**: Look in cardinal directions
- **↖️ ↗️ ↙️ ↘️**: Look diagonally
- **⚫ Center**: Look straight ahead

### Eye Color Selection

Click colored circles to change iris color:
- 🟢 Green (default)
- 🔵 Blue
- 🔴 Red
- 🟡 Yellow
- 🟣 Magenta
- 🔵 Cyan

### Actions

- **😉 BLINK**: Trigger manual blink animation

## 🎨 Customization

### Adjust Eye Parameters

Edit these constants in `ESP32_C6_Dual_LCD.ino`:

```cpp
// Eye appearance
#define EYE_RADIUS      100     // Outer eye size (default: 100)
#define IRIS_RADIUS     50      // Iris size (default: 50)
#define PUPIL_RADIUS    25      // Pupil size (default: 25)

// Eye movement
#define MAX_PUPIL_OFFSET_X  30  // How far pupils move horizontally
#define MAX_PUPIL_OFFSET_Y  20  // How far pupils move vertically

// Animation speed
pupilOffsetX += (targetPupilX - pupilOffsetX) * 0.2;  // Change 0.2 for speed
```

### Change WiFi Credentials

```cpp
#define AP_SSID_PREFIX  "CatEyes"   // Change SSID prefix
#define AP_PASSWORD     "meow1234"  // Change password (min 8 chars)
```

### Adjust Blink Rate

```cpp
unsigned long blinkInterval = 5000;  // Blink every 5 seconds (default)
int blinkDuration = 150;             // Blink lasts 150ms (default)
```

### Custom Iris Colors

Add custom colors to the color palette:

```cpp
// In handleColorChange() function, add:
else if (body.indexOf("purple") >= 0) {
  irisColor = 0x8010;  // Purple (RGB565 format)
}
```

RGB565 color calculator: http://www.barth-dev.de/online/rgb565-color-picker/

## 📊 Technical Specifications

| Specification | Value |
|---------------|-------|
| Controller | ESP32-C6 (RISC-V, 160MHz) |
| WiFi | WiFi 6 (802.11ax), 2.4GHz |
| Display Type | GC9A01 IPS LCD |
| Display Resolution | 240×240 per display (480×240 total) |
| Display Driver | 16-bit SPI |
| Frame Rate | ~50 FPS |
| Power Consumption | ~300mA @ 3.3V (both displays + WiFi) |
| Operating Voltage | 3.3V |
| Response Latency | <50ms (web command to display update) |

## 🔋 Power Options

### USB Power (Development)
- Connect USB-C cable to ESP32-C6
- Power consumption: ~300mA
- Suitable for testing and development

### Battery Power (Portable)
- Use 3.7V LiPo battery (1000mAh minimum)
- Add 3.3V LDO regulator (e.g., AMS1117-3.3)
- Expected runtime: 3-4 hours at full brightness
- Add power switch for convenience

### External 5V Supply (Permanent Installation)
- Use 5V 1A wall adapter
- Connect to 5V pin on ESP32-C6
- Suitable for fixed installations

## 🐛 Troubleshooting

### Problem: Displays don't turn on
**Solutions:**
- Check 3.3V and GND connections
- Verify USB cable supports data (not charge-only)
- Connect backlight (BL) pin to 3.3V
- Check if displays require 5V input (some have onboard regulator)

### Problem: Displays show garbage/random colors
**Solutions:**
- Verify SPI connections (SCK on GPIO20, MOSI on GPIO21)
- Check CS pins match code (GPIO2 and GPIO5)
- Ensure TFT_eSPI configured for GC9A01
- Try lower SPI frequency: `#define SPI_FREQUENCY 20000000`

### Problem: Only one display works
**Solutions:**
- Test each display individually by swapping CS pins
- Verify DC and RST pins for second display
- Check both displays share same SCK/MOSI lines
- Ensure power supply can provide 200mA for backlights

### Problem: Eyes don't mirror properly
**Solutions:**
- Check code line: `drawEye(-pupilOffsetX, pupilOffsetY);` for Display 2
- Negative sign on X offset creates mirror effect
- Adjust MAX_PUPIL_OFFSET values if range looks wrong

### Problem: Web interface won't load
**Solutions:**
- Verify connected to "CatEyes-XXXX" WiFi
- Try browsing to 192.168.4.1 directly
- Check Serial Monitor for WiFi status
- Restart ESP32-C6 if AP didn't start

### Problem: Laggy animations
**Solutions:**
- Reduce animation update rate: `delay(50)` in loop() instead of 20
- Lower SPI frequency in User_Setup.h
- Check power supply voltage (low voltage causes lag)
- Disable Serial.print statements for better performance

## 🔬 Advanced Features

### Adding More Eye Expressions

Modify `drawEye()` function to add expressions:

```cpp
void drawAngryEye() {
  // Add angled eyebrow above eye
  tft.drawLine(EYE_CENTER_X - 80, EYE_CENTER_Y - 80,
               EYE_CENTER_X + 20, EYE_CENTER_Y - 100, EYE_OUTLINE);
}

void drawHappyEye() {
  // Draw eye as crescent (half-closed happy eye)
  tft.fillCircle(EYE_CENTER_X, EYE_CENTER_Y - 20, EYE_RADIUS, EYE_WHITE);
}
```

### Add Accelerometer for Auto-Tracking

Connect an IMU (e.g., MPU6050) to make eyes follow device tilt:

```cpp
#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

void setup() {
  // ... existing setup ...
  mpu.initialize();
}

void loop() {
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  // Map accelerometer to pupil position
  targetPupilX = map(ax, -16384, 16384, -MAX_PUPIL_OFFSET_X, MAX_PUPIL_OFFSET_X);
  targetPupilY = map(ay, -16384, 16384, -MAX_PUPIL_OFFSET_Y, MAX_PUPIL_OFFSET_Y);

  // ... rest of loop ...
}
```

### Sleep Mode for Battery Saving

Add deep sleep when idle:

```cpp
#include <esp_sleep.h>

unsigned long lastActivityTime = 0;
#define SLEEP_TIMEOUT 60000  // Sleep after 60 seconds idle

void loop() {
  // ... existing code ...

  if (millis() - lastActivityTime > SLEEP_TIMEOUT) {
    Serial.println("Going to sleep...");
    esp_deep_sleep_start();
  }
}
```

## 📝 API Reference

### HTTP Endpoints

#### POST /api/eyes
Control eye direction.

**Request Body:**
```json
{
  "direction": "left"
}
```

**Valid directions:**
- `center`, `left`, `right`, `up`, `down`
- `up-left`, `up-right`, `down-left`, `down-right`

**Response:**
```json
{
  "success": true
}
```

#### POST /api/color
Change iris color.

**Request Body:**
```json
{
  "color": "blue"
}
```

**Valid colors:**
- `green`, `blue`, `red`, `yellow`, `magenta`, `cyan`

#### GET /api/blink
Trigger blink animation.

**Response:**
```json
{
  "success": true
}
```

#### GET /api/status
Get system status.

**Response:**
```json
{
  "ap_ssid": "CatEyes-1234",
  "ap_ip": "192.168.4.1",
  "clients": 1,
  "direction": "2",
  "uptime": 12345
}
```

## 🎯 Use Cases

- **Interactive Art Installation**: Eyes follow viewers or respond to sound
- **Pet Companion**: Animated eyes for robot or toy
- **Status Indicator**: Eyes look different directions for system states
- **Accessibility Device**: Non-verbal communication tool
- **Halloween Decoration**: Spooky animated eyes
- **Educational Tool**: Teach animation and embedded systems
- **Conference Badge**: Fun interactive badge accessory

## 📦 Project Structure

```
ESP32_C6_Dual_LCD/
├── ESP32_C6_Dual_LCD.ino    # Main firmware file
├── README.md                 # This file
├── WIRING_DUAL_LCD.txt      # Detailed wiring guide
└── User_Setup.h             # TFT_eSPI configuration (optional)
```

## 🤝 Contributing

Improvements welcome! Areas for contribution:
- Additional eye expressions (sleepy, surprised, etc.)
- More iris color options
- Eye animations (rolling, darting, etc.)
- Touch sensor integration
- Voice control via microphone
- Integration with Framework Badge LED matrix

## 📄 License

MIT No Attribution License - Free to use, modify, and distribute.

## 🙏 Acknowledgments

- **Seeed Studio** for XIAO ESP32-C6 platform
- **Bodmer** for TFT_eSPI library
- **Espressif** for ESP32 Arduino core
- **Framework NYC 2025** for inspiring this project

## 📞 Support

Issues or questions:
1. Check `WIRING_DUAL_LCD.txt` for wiring help
2. Review Troubleshooting section above
3. Open GitHub issue with:
   - Serial Monitor output
   - Photos of wiring
   - Description of problem

## 🔗 Related Projects

- **Framework Badge Main Project**: LED matrix badge hardware
- **ESP32_C6_Enhanced**: Single display with QR code and animations
- **RP2040_Enhanced**: Badge firmware with I2C communication

---

**Made with 💚 for Frame:Work NYC 2025**

*Bringing cat eyes to life, one pixel at a time.* 🐱
