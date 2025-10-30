# ESP32-C6 Board Support Installation Guide

Complete setup instructions for XIAO ESP32-C6 development with Arduino IDE.

## Arduino IDE Installation

### Method 1: Arduino IDE Board Manager (Recommended)

This is the easiest method using the official Espressif ESP32 board package.

#### Step 1: Install Arduino IDE

Download Arduino IDE 2.x from: https://www.arduino.cc/en/software

(Arduino IDE 1.x also works, but 2.x is recommended)

#### Step 2: Add ESP32 Board Manager URL

1. Open Arduino IDE
2. Go to **File → Preferences** (Arduino IDE → Settings on macOS)
3. Find **Additional Board Manager URLs** field
4. Add this URL:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
5. If you already have other URLs, separate with commas or click the window icon to add on a new line
6. Click **OK**

#### Step 3: Install ESP32 Board Support

1. Go to **Tools → Board → Boards Manager**
2. Search for "esp32"
3. Find "esp32 by Espressif Systems"
4. Click **Install** (latest version 3.3.2 recommended)
5. Wait for installation to complete (~5-10 minutes, downloads ~27MB)

#### Step 4: Select XIAO ESP32-C6 Board

1. Go to **Tools → Board → ESP32 Arduino**
2. Scroll down and select **XIAO_ESP32C6**

#### Step 5: Configure Board Settings

- **Tools → USB CDC On Boot:** Enabled
- **Tools → USB Mode:** Hardware CDC and JTAG
- **Tools → Upload Mode:** UART0 / Hardware CDC
- **Tools → Upload Speed:** 921600
- **Tools → CPU Frequency:** 160MHz (WiFi 6 mode)
- **Tools → Flash Mode:** QIO 80MHz
- **Tools → Flash Size:** 4MB (Note: C6 has 4MB vs S3's 8MB)
- **Tools → Partition Scheme:** Default 4MB with spiffs (1.2MB APP/1.5MB SPIFFS)
- **Tools → Port:** Select your XIAO's port (COM port on Windows, /dev/ttyACM0 on Linux, /dev/cu.usbmodem* on macOS)

**Note:** ESP32-C6 does NOT have PSRAM (unlike S3). It has 512KB SRAM built-in.

---

### Method 2: Using Downloaded Package (Manual)

If you have the `Esp32.json` file or want to install manually.

#### Step 1: Download ESP32 Package

From the JSON file, the package URL is:
```
https://github.com/espressif/arduino-esp32/releases/download/3.3.2/esp32-3.3.2.zip
```

Download manually or let Arduino IDE download automatically using Method 1.

#### Step 2: Manual Installation (Advanced)

**Not recommended** - Use Method 1 instead. Manual installation is error-prone.

If you must install manually:
1. Download the ZIP from the URL above
2. Extract to Arduino hardware folder:
   - Windows: `C:\Users\[USERNAME]\AppData\Local\Arduino15\packages\esp32\`
   - macOS: `~/Library/Arduino15/packages/esp32/`
   - Linux: `~/.arduino15/packages/esp32/`
3. Install required tools (python, esptool, etc.) - complex!

**Recommendation:** Stick with Method 1 (Board Manager).

---

## Required Libraries

Install these libraries via **Tools → Manage Libraries** (Library Manager):

### Core Libraries (Required)

These are usually included with ESP32 board support:
- ✓ **Wire** (I2C communication) - Built-in
- ✓ **WiFi** (WiFi functionality) - Built-in
- ✓ **WebServer** (HTTP server) - Built-in
- ✓ **BLE** (Bluetooth Low Energy) - Built-in

### Additional Libraries (Recommended)

Search and install these from Library Manager:

1. **ArduinoJson** (for JSON parsing)
   - Author: Benoit Blanchon
   - Version: 7.x or newer

2. **PubSubClient** (for MQTT if needed)
   - Author: Nick O'Leary
   - Version: 2.8 or newer

3. **HTTPClient** (for REST API calls)
   - Usually included with ESP32 core

---

## Uploading Firmware to XIAO ESP32-C6

### First-Time Upload

1. **Connect XIAO to computer** via USB-C cable
2. **Enter bootloader mode:**
   - Hold **BOOT** button
   - While holding BOOT, press and release **RESET** button
   - Release BOOT button
   - LED should flash or stay on indicating bootloader mode

3. **Verify port selection:**
   - **Tools → Port** - should show new port (e.g., COM3, /dev/ttyACM0)

4. **Click Upload** in Arduino IDE

5. **Wait for upload** (~30-60 seconds)

6. **Press RESET** button after upload to run new firmware

### Subsequent Uploads

If you have **USB CDC On Boot** enabled, you usually don't need to enter bootloader mode manually:

1. Just click **Upload**
2. Arduino IDE will auto-reset XIAO into bootloader

If auto-reset fails, repeat the manual bootloader entry steps.

---

## Testing Your Setup

### 1. Blink Test

Upload this simple test to verify board support is working:

```cpp
#define LED_PIN LED_BUILTIN // Built-in LED

void setup() {
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_PIN, HIGH);
  delay(500);
  digitalWrite(LED_PIN, LOW);
  delay(500);
}
```

**Expected result:** Built-in LED blinks every second

### 2. Serial Test

```cpp
void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);
  Serial.println("XIAO ESP32-S3 is working!");
}

void loop() {
  Serial.println("Hello from ESP32-S3");
  delay(1000);
}
```

**Expected result:** Serial monitor shows messages every second

### 3. WiFi Scan Test

```cpp
#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("WiFi 6 scan starting...");
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

**Expected result:** List of nearby WiFi networks (supports WiFi 6/802.11ax)

### 4. I2C Scan Test

```cpp
#include <Wire.h>

void setup() {
  Serial.begin(115200);
  Wire.begin(6, 7); // SDA=GPIO6, SCL=GPIO7 on XIAO ESP32-C6

  Serial.println("I2C Scanner");
  byte error, address;
  int nDevices = 0;

  for(address = 1; address < 127; address++ ) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address<16) Serial.print("0");
      Serial.println(address,HEX);
      nDevices++;
    }
  }

  if (nDevices == 0)
    Serial.println("No I2C devices found");
  else
    Serial.println("done");
}

void loop() {}
```

**Expected result:** Should detect RP2040 at address 0x42 when connected

---

## Uploading the Dual-MCU Firmware

### For ESP32-C6 (I2C Slave)

1. Open `ESP32_C6_Slave/ESP32_C6_Slave.ino` (or `ESP32_S3_Slave/ESP32_S3_Slave.ino` if not renamed yet)
2. Select board: **Tools → Board → XIAO_ESP32C6**
3. Configure settings (see Step 5 in Method 1 above)
4. Connect XIAO ESP32-C6 via USB
5. Enter bootloader mode if needed (BOOT + RESET)
6. Click **Upload**
7. Open **Serial Monitor** (115200 baud)
8. Should see: "XIAO ESP32-C6 I2C Slave - Ready for commands"

### For RP2040 (I2C Master)

1. First, make sure ESP32-C6 firmware is uploaded and running
2. Disconnect XIAO ESP32-C6 from USB
3. **Connect** ESP32-C6 to badge SAO port (see WIRING.txt)
4. Connect Framework Badge to computer via USB
5. Enter RP2040 bootloader mode (BOOT + RESET on badge)
6. Select board: **Tools → Board → Raspberry Pi Pico**
7. Open `RP2040_Master/RP2040_Master.ino`
8. Click **Upload**
9. Open **Serial Monitor** (115200 baud)
10. Should see: "ESP32-C6 connected!"

---

## Troubleshooting

### "Board not found" or Port not appearing

**Windows:**
- Install CH340 USB driver: https://wiki.seeedstudio.com/XIAO_ESP32C6_Getting_Started/#driver-installation
- Try different USB cable (must support data, not charge-only)

**macOS:**
- Driver usually works automatically
- Try: System Settings → Privacy & Security → Allow blocked driver

**Linux:**
- Add user to dialout group: `sudo usermod -a -G dialout $USER`
- Logout and login again
- Check permissions: `ls -l /dev/ttyACM0` or `/dev/ttyUSB0`

### Upload fails with "Timed out waiting for packet header"

1. Enter bootloader mode manually (BOOT + RESET)
2. Try lower upload speed: **Tools → Upload Speed → 115200**
3. Press RESET button right before clicking Upload
4. Check USB cable quality (use short cable <1m)

### "Sketch too big" error

- Change partition scheme: **Tools → Partition Scheme → No OTA (2MB APP/2MB SPIFFS)**
- Or reduce code size by removing unused libraries

### WiFi doesn't work

1. Check antenna connection (built-in PCB antenna on XIAO)
2. ESP32-C6 supports **both 2.4GHz and WiFi 6** (but still no 5GHz band)
3. Try closer to router
4. Check if board supports WiFi (some clones don't have WiFi chip)
5. WiFi 6 features work best with WiFi 6-enabled routers

### BLE doesn't work

1. Make sure WiFi is initialized first (ESP32 shares radio)
2. ESP32-C6 supports **BLE 5.3** with improved range and speed
3. **Note:** ESP32-C6 has NO PSRAM (unlike S3), so skip PSRAM settings
4. Try BLE example from **File → Examples → ESP32 BLE Arduino**

### I2C communication fails with RP2040

1. Check wiring (SDA, SCL, GND, VCC)
2. Verify I2C address (0x42 in both firmwares)
3. Add external 4.7kΩ pull-ups on SDA and SCL
4. Run I2C scanner on both devices to verify

---

## Useful Resources

### Official Documentation

- **XIAO ESP32-C6 Wiki:** https://wiki.seeedstudio.com/xiao_esp32c6_getting_started/
- **ESP32-C6 Datasheet:** https://www.espressif.com/sites/default/files/documentation/esp32-c6_datasheet_en.pdf
- **ESP32 Arduino Core:** https://github.com/espressif/arduino-esp32
- **ESP32 API Reference:** https://docs.espressif.com/projects/arduino-esp32/en/latest/

### Example Code

- **XIAO ESP32-C6 Examples:** https://github.com/Seeed-Studio/Seeed_Arduino_XIAO_ESP32C6
- **ESP32 WiFi Examples:** File → Examples → WiFi (in Arduino IDE)
- **ESP32 BLE Examples:** File → Examples → ESP32 BLE Arduino

### Tools

- **Esptool (firmware flashing):** https://github.com/espressif/esptool
- **ESP32 Flash Download Tool:** https://www.espressif.com/en/support/download/other-tools
- **Serial Monitor:** Built into Arduino IDE or use PuTTY, screen, minicom

### Community

- **Seeed Forum:** https://forum.seeedstudio.com/
- **ESP32 Forum:** https://esp32.com/
- **Arduino Forum:** https://forum.arduino.cc/

---

## Quick Reference

### Board Settings (XIAO ESP32-C6)

| Setting | Value |
|---------|-------|
| Board | XIAO_ESP32C6 |
| USB CDC On Boot | Enabled |
| USB Mode | Hardware CDC and JTAG |
| Upload Speed | 921600 |
| CPU Frequency | 160MHz (WiFi 6) |
| Flash Size | 4MB (Quad SPI) |
| Partition | Default 4MB with spiffs |
| Architecture | RISC-V (32-bit) |
| PSRAM | None (512KB SRAM built-in) |

### Key ESP32-C6 Advantages
- **WiFi 6 (802.11ax)** - Better performance in crowded networks
- **BLE 5.3** - Longer range, faster speeds
- **Lower power consumption** - Better for battery operation
- **RISC-V architecture** - Open-source instruction set

### Pin Reference (XIAO ESP32-C6)

| Pin Label | GPIO | Function | Used For |
|-----------|------|----------|----------|
| D4 | GPIO 6 | I2C SDA | RP2040 communication |
| D5 | GPIO 7 | I2C SCL | RP2040 communication |
| 3V3 | - | Power out | Powers XIAO (from badge) |
| GND | - | Ground | Common ground |

### I2C Slave Example (Minimal)

```cpp
#include <Wire.h>

#define I2C_ADDR 0x42

void onReceive(int bytes) {
  while (Wire.available()) {
    uint8_t data = Wire.read();
    // Process data
  }
}

void onRequest() {
  Wire.write(0xA5); // Send response
}

void setup() {
  Wire.begin(6, 7, I2C_ADDR); // SDA, SCL, Address
  Wire.onReceive(onReceive);
  Wire.onRequest(onRequest);
}

void loop() {
  // Your code here
}
```

---

## Next Steps

After successful installation:

1. ✓ Test basic blink sketch
2. ✓ Test WiFi scan
3. ✓ Test I2C communication
4. → Upload ESP32_C6_Slave firmware (or ESP32_S3_Slave with updated code)
5. → Connect to Framework Badge via SAO port
6. → Upload RP2040_Master firmware
7. → Test dual-MCU system

See `README.md` in `Dual_MCU_Integration/` for complete usage instructions.
