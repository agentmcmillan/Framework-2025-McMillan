# ESP32 ProS3 Board Support Installation Guide

Complete setup instructions for **Unexpected Maker ESP32 ProS3** development with Arduino IDE.

## Why Choose ESP32 ProS3?

The ProS3 is the recommended board for this project due to:

- **16MB Flash** (vs 4MB on C6) - More space for web interface and firmware
- **8MB PSRAM** (vs none on C6) - Better for graphics and buffering
- **Dual 700mA LDOs** - Stable power for badge + WiFi operation
- **STEMMA QT connector** - Plug-and-play I2C connection (GPIO8/GPIO9)
- **Battery charging circuit** - Built-in LiPo management
- **Ultra-low deep sleep** (10µA) - Better battery life
- **27 GPIOs** - More expansion options

## Arduino IDE Installation

### Step 1: Install Arduino IDE

Download Arduino IDE 2.x from: https://www.arduino.cc/en/software

(Arduino IDE 1.x also works, but 2.x is recommended)

### Step 2: Add ESP32 Board Manager URL

1. Open Arduino IDE
2. Go to **File → Preferences** (Arduino IDE → Settings on macOS)
3. Find **Additional Board Manager URLs** field
4. Add this URL:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
5. If you already have other URLs, separate with commas or click the window icon to add on a new line
6. Click **OK**

### Step 3: Install ESP32 Board Support

1. Go to **Tools → Board → Boards Manager**
2. Search for "esp32"
3. Find "esp32 by Espressif Systems"
4. Click **Install** (version 3.0.0 or newer recommended)
5. Wait for installation to complete (~5-10 minutes, downloads ~300MB)

### Step 4: Select ProS3 Board

1. Go to **Tools → Board → ESP32 Arduino**
2. Scroll down and select **ESP32S3 Dev Module**

   **Note:** There's no specific "ProS3" board entry. Use "ESP32S3 Dev Module" with custom settings below.

### Step 5: Configure Board Settings for ProS3

Critical settings for ProS3:

| Setting | Value | Notes |
|---------|-------|-------|
| **USB CDC On Boot** | Enabled | Required for USB serial |
| **USB Mode** | Hardware CDC and JTAG | For programming |
| **Upload Mode** | UART0 / Hardware CDC | Standard upload |
| **Upload Speed** | 921600 | Fast upload |
| **CPU Frequency** | 240MHz | Maximum performance |
| **Flash Mode** | QIO 80MHz | Quad I/O for speed |
| **Flash Size** | **16MB (128Mb)** | ProS3 has 16MB! |
| **Partition Scheme** | **16M Flash (3MB APP/9.9MB FATFS)** | For web files |
| **PSRAM** | **QSPI PSRAM** | ProS3 has 8MB PSRAM |
| **Arduino Runs On** | Core 1 | Default |
| **Events Run On** | Core 1 | Default |

**Important:** The Flash Size and Partition Scheme settings are critical for ProS3!

### Step 6: Select Port

- **Tools → Port** - Select your ProS3's port
  - macOS: `/dev/cu.usbmodem*` or `/dev/cu.wchusbserial*`
  - Windows: `COM3` or similar
  - Linux: `/dev/ttyACM0` or `/dev/ttyUSB0`

---

## Required Libraries

Install these libraries via **Tools → Manage Libraries** (Library Manager):

### Core Libraries (Built-in with ESP32 package)

These are already included when you install ESP32 board support:

- ✓ **Wire** (I2C communication)
- ✓ **WiFi** (WiFi functionality)
- ✓ **WebServer** (HTTP server)
- ✓ **DNSServer** (Captive portal DNS)
- ✓ **BLEDevice** (Bluetooth Low Energy)

### Additional Libraries (Install from Library Manager)

Search and install these:

1. **ArduinoJson** (for JSON parsing in web interface)
   - Author: Benoit Blanchon
   - Version: 7.x or newer
   - Used by Enhanced firmware for LED data transfer

2. **PubSubClient** (optional, for MQTT)
   - Author: Nick O'Leary
   - Version: 2.8 or newer
   - Only needed if adding MQTT features

---

## Uploading Firmware to ProS3

### First-Time Upload

1. **Connect ProS3 to computer** via USB-C cable

2. **Enter bootloader mode** (usually not needed with auto-reset):
   - Hold **BOOT** button (labeled "0" or "BOOT")
   - While holding BOOT, press and release **RESET** button
   - Release BOOT button
   - RGB LED should turn purple/magenta indicating bootloader mode

3. **Verify port selection:**
   - **Tools → Port** - should show new port

4. **Click Upload** in Arduino IDE

5. **Wait for upload** (~30-90 seconds depending on sketch size)

6. **Press RESET** button after upload to run new firmware

### Subsequent Uploads

With **USB CDC On Boot** enabled, you usually don't need to enter bootloader mode manually:

1. Just click **Upload**
2. Arduino IDE will auto-reset ProS3 into bootloader

If auto-reset fails, repeat the manual bootloader entry steps.

---

## Testing Your ProS3 Setup

### 1. RGB LED Blink Test

Upload this to verify board support and RGB LED:

```cpp
#include <Adafruit_NeoPixel.h>

// ProS3 has RGB LED on GPIO48
#define RGB_LED_PIN 48
#define RGB_BRIGHTNESS 20  // 0-255

Adafruit_NeoPixel rgb(1, RGB_LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  rgb.begin();
  rgb.setBrightness(RGB_BRIGHTNESS);
}

void loop() {
  // Red
  rgb.setPixelColor(0, rgb.Color(255, 0, 0));
  rgb.show();
  delay(500);

  // Green
  rgb.setPixelColor(0, rgb.Color(0, 255, 0));
  rgb.show();
  delay(500);

  // Blue
  rgb.setPixelColor(0, rgb.Color(0, 0, 255));
  rgb.show();
  delay(500);
}
```

**Expected result:** RGB LED cycles through red, green, blue

**Note:** You'll need to install **Adafruit_NeoPixel** library first!

### 2. Serial Test

```cpp
void setup() {
  Serial.begin(115200);
  delay(1000);  // Wait for serial to initialize

  Serial.println("\n\n=== ESP32 ProS3 Test ===");
  Serial.print("Flash Size: ");
  Serial.print(ESP.getFlashChipSize() / (1024 * 1024));
  Serial.println(" MB");

  Serial.print("PSRAM Size: ");
  Serial.print(ESP.getPsramSize() / (1024 * 1024));
  Serial.println(" MB");

  Serial.print("Free Heap: ");
  Serial.print(ESP.getFreeHeap() / 1024);
  Serial.println(" KB");
}

void loop() {
  Serial.println("ProS3 is working!");
  delay(1000);
}
```

**Expected result:** Serial monitor shows:
```
=== ESP32 ProS3 Test ===
Flash Size: 16 MB
PSRAM Size: 8 MB
Free Heap: ~350 KB
ProS3 is working!
```

### 3. WiFi Scan Test

```cpp
#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("WiFi scan starting...");
  int n = WiFi.scanNetworks();

  Serial.print("Networks found: ");
  Serial.println(n);

  for (int i = 0; i < n; i++) {
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.print(WiFi.SSID(i));
    Serial.print(" (");
    Serial.print(WiFi.RSSI(i));
    Serial.println(" dBm)");
  }
}

void loop() {}
```

**Expected result:** List of nearby WiFi networks

### 4. I2C Scan Test (for Badge Connection)

```cpp
#include <Wire.h>

#define I2C_SDA 8   // ProS3 STEMMA QT SDA
#define I2C_SCL 9   // ProS3 STEMMA QT SCL

void setup() {
  Serial.begin(115200);
  delay(1000);

  Wire.begin(I2C_SDA, I2C_SCL);  // Initialize I2C on STEMMA QT pins

  Serial.println("\n=== I2C Scanner (ProS3) ===");
  Serial.println("Scanning I2C bus (GPIO8=SDA, GPIO9=SCL)...");

  byte error, address;
  int nDevices = 0;

  for(address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
      nDevices++;
    }
  }

  if (nDevices == 0)
    Serial.println("No I2C devices found");
  else
    Serial.println("Scan complete");
}

void loop() {}
```

**Expected result:** Should detect RP2040 at address 0x42 when badge is connected

---

## ProS3 Specific Features

### STEMMA QT Connector (I2C)

The ProS3 has a built-in STEMMA QT (Qwiic compatible) connector:

- **SDA:** GPIO8
- **SCL:** GPIO9
- **VCC:** 3.3V (from LDO1)
- **GND:** Ground

This is **perfect for connecting to the Framework Badge** - just need a STEMMA QT to SAO adapter cable!

### Dual LDO Power Management

The ProS3 has two independent 3.3V regulators:

- **LDO1:** Always on, powers core and STEMMA QT (700mA)
- **LDO2:** Controlled by GPIO17, auto-shuts down in deep sleep (700mA)

For badge project: Connect to LDO1 (STEMMA QT or 3V3 pin)

### RGB LED Control

ProS3 has a low-power RGB LED on **GPIO48**:

```cpp
#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel rgb(1, 48, NEO_GRB + NEO_KHZ800);

void setup() {
  rgb.begin();
  rgb.setBrightness(20);  // Keep low for power savings
  rgb.setPixelColor(0, rgb.Color(0, 255, 0));  // Green
  rgb.show();
}
```

### Battery Voltage Monitoring

ProS3 can read battery voltage on **VBAT** pin (labeled "BAT"):

```cpp
#define VBAT_PIN 4  // ADC pin for battery monitoring

float getBatteryVoltage() {
  int raw = analogRead(VBAT_PIN);
  return (raw / 4095.0) * 3.3 * 2.0;  // Voltage divider
}
```

---

## Uploading the Dual-MCU Firmware

### For ESP32 ProS3 (I2C Slave)

1. Open firmware sketch:
   - Basic: `Dual_MCU_Integration/ESP32_S3_Slave/ESP32_S3_Slave.ino`
   - Enhanced: `Dual_MCU_Integration/ESP32_S3_Enhanced/ESP32_S3_Enhanced.ino`

2. **Update I2C pin definitions** in the sketch:
   ```cpp
   // Change from:
   #define I2C_SDA 6
   #define I2C_SCL 7

   // To ProS3 STEMMA QT pins:
   #define I2C_SDA 8
   #define I2C_SCL 9
   ```

3. Select board: **ESP32S3 Dev Module**

4. Configure settings (see Step 5 above)

5. Connect ProS3 via USB

6. Enter bootloader mode if needed (BOOT + RESET)

7. Click **Upload**

8. Open **Serial Monitor** (115200 baud)

9. Should see: "ESP32 ProS3 I2C Slave - Ready for commands"

### For RP2040 (I2C Master)

1. First ensure ProS3 firmware is uploaded and running

2. Disconnect ProS3 from USB

3. **Connect ProS3 to badge** via SAO port or STEMMA QT cable

4. Connect Framework Badge to computer via USB

5. Enter RP2040 bootloader mode (BOOT + RESET on badge)

6. Select board: **Tools → Board → Raspberry Pi Pico**

7. Open RP2040 firmware:
   - Basic: `RP2040_Master/RP2040_Master.ino`
   - Enhanced: `RP2040_Enhanced/RP2040_Enhanced.ino`

8. Click **Upload**

9. Open **Serial Monitor** (115200 baud)

10. Should see: "ProS3 connected!" (if I2C communication works)

---

## Troubleshooting

### "Board not found" or Port not appearing

**Windows:**
- Install CH340/CP210x USB driver from ProS3 manufacturer
- Try different USB cable (must support data, not charge-only)

**macOS:**
- Driver usually works automatically
- If not: System Settings → Privacy & Security → Allow blocked driver

**Linux:**
- Add user to dialout group: `sudo usermod -a -G dialout $USER`
- Logout and login again
- Check permissions: `ls -l /dev/ttyACM0`

### Upload fails with "Timed out waiting for packet header"

1. Enter bootloader mode manually (BOOT + RESET)
2. Try lower upload speed: **Tools → Upload Speed → 115200**
3. Press RESET button right before clicking Upload
4. Check USB cable quality (use short cable <1m)

### "Sketch too big" error

- Check **Flash Size** is set to **16MB (128Mb)**, not 4MB
- Change **Partition Scheme** to one with larger APP space
- Remove unused libraries from sketch

### WiFi doesn't work

1. ProS3 supports **2.4GHz only** (no 5GHz)
2. Check antenna connection (built-in PCB antenna or u.FL)
3. Move closer to router
4. Try WiFi scan test to verify radio works

### I2C communication fails with RP2040

1. Verify wiring (SDA=GPIO8, SCL=GPIO9, GND, 3V3)
2. Check I2C address (0x42 in both firmwares)
3. Add external 4.7kΩ pull-ups on SDA and SCL
4. Run I2C scanner on both devices separately
5. Try slower I2C speed: `Wire.setClock(50000);` // 50kHz

### PSRAM not detected

1. Verify **PSRAM** setting is **QSPI PSRAM**
2. Not all ProS3 variants have PSRAM - check your specific model
3. Run serial test (above) to check PSRAM size

---

## Comparison: ProS3 vs XIAO ESP32-C6

| Feature | ESP32 ProS3 | XIAO ESP32-C6 |
|---------|-------------|---------------|
| **Flash** | 16MB | 4MB |
| **PSRAM** | 8MB | None (512KB SRAM) |
| **WiFi** | 802.11b/g/n | **WiFi 6 (802.11ax)** ✓ |
| **Bluetooth** | BLE 5.0 | **BLE 5.3** ✓ |
| **Architecture** | Xtensa dual-core | RISC-V single-core |
| **I2C Connector** | **STEMMA QT built-in** ✓ | Pins only |
| **Battery Charging** | **Built-in LiPo** ✓ | External required |
| **Power Regulators** | **Dual 700mA LDOs** ✓ | Single LDO |
| **GPIOs** | **27 pins** ✓ | Limited |
| **Deep Sleep** | **10µA** ✓ | ~150µA |
| **Size** | Larger (Feather-like) | **Tiny (21×17.5mm)** ✓ |
| **Price** | ~$20 | ~$10 |

**Recommendation:**
- Choose **ProS3** for: More storage, better power, STEMMA QT convenience
- Choose **C6** for: WiFi 6, compact size, lower cost

---

## Useful Resources

### Official Documentation

- **ProS3 Product Page:** https://esp32s3.com/pros3.html
- **ProS3 Schematic:** https://esp32s3.com/pros3.html#downloads
- **Unexpected Maker GitHub:** https://github.com/UnexpectedMaker/esp32s3
- **ESP32-S3 Datasheet:** https://www.espressif.com/sites/default/files/documentation/esp32-s3_datasheet_en.pdf
- **ESP32 Arduino Core:** https://github.com/espressif/arduino-esp32
- **ESP32 API Reference:** https://docs.espressif.com/projects/arduino-esp32/en/latest/

### Example Code

- **ProS3 Arduino Examples:** https://github.com/UnexpectedMaker/esp32s3/tree/main/code
- **ESP32 WiFi Examples:** File → Examples → WiFi (in Arduino IDE)
- **ESP32 BLE Examples:** File → Examples → ESP32 BLE Arduino

### Community

- **Unexpected Maker Discord:** https://unexpectedmaker.com/discord
- **ESP32 Forum:** https://esp32.com/
- **Arduino Forum:** https://forum.arduino.cc/

---

## Quick Reference

### Board Settings Summary (ProS3)

```
Board: ESP32S3 Dev Module
USB CDC On Boot: Enabled
USB Mode: Hardware CDC and JTAG
Upload Speed: 921600
CPU Frequency: 240MHz
Flash Mode: QIO 80MHz
Flash Size: 16MB (128Mb)  ← CRITICAL
Partition Scheme: 16M Flash (3MB APP/9.9MB FATFS)
PSRAM: QSPI PSRAM  ← CRITICAL
```

### Pin Reference (ProS3)

| Function | GPIO | Notes |
|----------|------|-------|
| **I2C SDA** | **GPIO 8** | STEMMA QT connector |
| **I2C SCL** | **GPIO 9** | STEMMA QT connector |
| **RGB LED** | GPIO 48 | Built-in NeoPixel |
| **Battery Monitor** | GPIO 4 | VBAT with voltage divider |
| **LDO2 Enable** | GPIO 17 | Controls second regulator |
| **USB D-** | GPIO 19 | Native USB |
| **USB D+** | GPIO 20 | Native USB |
| **TX** | GPIO 43 | Serial UART |
| **RX** | GPIO 44 | Serial UART |

### I2C Slave Code (Minimal for ProS3)

```cpp
#include <Wire.h>

#define I2C_SDA 8   // ProS3 STEMMA QT
#define I2C_SCL 9
#define I2C_ADDR 0x42

void onReceive(int bytes) {
  while (Wire.available()) {
    uint8_t data = Wire.read();
    Serial.printf("Received: 0x%02X\n", data);
  }
}

void onRequest() {
  Wire.write(0xA5);  // Send response
}

void setup() {
  Serial.begin(115200);
  Wire.begin(I2C_SDA, I2C_SCL, I2C_ADDR);
  Wire.onReceive(onReceive);
  Wire.onRequest(onRequest);
  Serial.println("ProS3 I2C slave ready!");
}

void loop() {
  delay(10);
}
```

---

## Next Steps

After successful ProS3 setup:

1. ✓ Test basic blink sketch with RGB LED
2. ✓ Test WiFi scan
3. ✓ Test I2C communication with I2C scanner
4. → Upload ESP32 slave firmware (update I2C pins to 8/9!)
5. → Connect to Framework Badge via SAO port or STEMMA QT
6. → Upload RP2040 master firmware
7. → Test dual-MCU system

See `README.md` in `Dual_MCU_Integration/` for complete usage instructions.

---

**Note:** This guide is specifically for the **Unexpected Maker ProS3**. For XIAO ESP32-C6/S3 setup, see `ESP32_C6_SETUP.md`.
