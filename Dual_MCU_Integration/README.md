# Framework Badge + ESP32 ProS3 Dual-Microcontroller System

Integration of **Unexpected Maker ESP32 ProS3** via SAO port to add WiFi/Bluetooth capabilities to the RP2040-based Framework Badge.

**Why ProS3?**
- **16MB Flash** - Plenty of space for web interfaces and OTA updates
- **8MB PSRAM** - Smooth graphics and buffering
- **STEMMA QT Connector** - Plug-and-play I2C connection (GPIO8/GPIO9)
- **Dual 700mA LDOs** - Rock-solid power for badge + WiFi operation
- **Battery Charging** - Built-in LiPo management
- **Ultra-low Deep Sleep** - Only 10µA in deep sleep mode

<img width="617" height="845" alt="image" src="https://github.com/user-attachments/assets/d501fa13-9304-41f4-819b-10ed1fb17859" />



## System Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                    Framework Badge (RP2040)                 │
│                                                             │
│  ┌──────────────┐  ┌────────────┐  ┌───────────────┐       │
│  │  LED Matrix  │  │  Buttons   │  │   Battery     │       │
│  │  15x7 WS2812B│  │  (3 btns)  │  │   Monitor     │       │
│  │  IO#4        │  │  IO#5,6,7  │  │   IO#26       │       │
│  └──────────────┘  └────────────┘  └───────────────┘       │
│                                                             │
│  ┌──────────────────────────────────────────────────┐       │
│  │         I2C Master Controller                    │       │
│  │         (Whisker/SAO GPIO: IO#23, IO#24)         │       │
│  └──────────────┬───────────────────────────────────┘       │
│                 │ I2C Bus (SDA, SCL)                        │
└─────────────────┼───────────────────────────────────────────┘
                  │
                  │ SAO Port (4-pin: VCC, GND, SDA, SCL)
                  │
┌─────────────────┼───────────────────────────────────────────┐
│                 │    XIAO ESP32-S3 (I2C Slave)              │
│                 ▼                                           │
│  ┌──────────────────────────────────────────────────┐       │
│  │         I2C Slave (Address: 0x42)                │       │
│  │         Receives commands from RP2040            │       │
│  └──────────────────────────────────────────────────┘       │
│                                                             │
│  ┌──────────────┐  ┌────────────┐  ┌───────────────┐       │
│  │   WiFi       │  │ Bluetooth  │  │   Optional    │       │
│  │   Client/AP  │  │ BLE beacon │  │   Camera/SD   │       │
│  │   Web server │  │ Mesh net   │  │   (GPIO avail)│       │
│  └──────────────┘  └────────────┘  └───────────────┘       │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

## Hardware Overview

### Framework Badge (RP2040) - Master
- **Role:** Main controller, LED display, user interface
- **I2C Master:** Controls ESP32-S3 via commands
- **Duties:**
  - LED matrix animations
  - Button input handling
  - Battery monitoring
  - Sending display/action requests to ESP32-S3

### ESP32 ProS3 - I2C Slave
- **Role:** Wireless communication coprocessor
- **I2C Slave Address:** 0x42 (configurable)
- **I2C Pins:** GPIO8 (SDA), GPIO9 (SCL) via STEMMA QT connector
- **Capabilities:**
  - WiFi (802.11b/g/n, 2.4GHz)
  - Bluetooth 5.0 (BLE)
  - Web server for badge control
  - BLE beacon/mesh networking
  - HTTP client for cloud integration
  - OTA firmware updates
  - 27 additional GPIOs for expansion

## Pin Connections

### Option 1: STEMMA QT Connector (Recommended - Easiest!)

The ProS3 has a built-in STEMMA QT connector that makes I2C connection plug-and-play.

```
Framework Badge SAO Port          ESP32 ProS3 STEMMA QT
─────────────────────────         ─────────────────────────
Pin 1: VCC (+3.3V)            →   Red wire (3.3V)
Pin 2: GND                    →   Black wire (GND)
Pin 3: SDA (IO#23 or IO#24)   →   Blue wire (GPIO 8 SDA)
Pin 4: SCL (IO#24 or IO#25)   →   Yellow wire (GPIO 9 SCL)
```

**Note:** You'll need a custom SAO-to-STEMMA QT adapter cable since the pinouts differ. Alternatively, use Option 2 below.

### Option 2: Direct GPIO Wiring

Wire directly to the ProS3's GPIO header pins:

```
Framework Badge SAO Port          ESP32 ProS3
─────────────────────────         ─────────────────
Pin 1: VCC (+3.3V)            →   3V3 pin
Pin 2: GND                    →   GND
Pin 3: SDA (IO#23 or IO#24)   →   GPIO 8 (SDA)
Pin 4: SCL (IO#24 or IO#25)   →   GPIO 9 (SCL)
```

**Note:** SAO port reuses Whisker header pins in a different physical arrangement. See `WIRING.txt` for complete wiring diagrams.

### RP2040 I2C Configuration
- **SDA:** IO#23 (or IO#24, configurable)
- **SCL:** IO#24 (or IO#25, configurable)
- **I2C Speed:** 100kHz (standard) or 400kHz (fast mode)
- **Pull-ups:** 4.7kΩ on both SDA and SCL (may be built into badge)

### ESP32 ProS3 I2C Configuration
- **SDA:** GPIO 8 (STEMMA QT connector)
- **SCL:** GPIO 9 (STEMMA QT connector)
- **I2C Address:** 0x42
- **Clock Speed:** 100kHz (standard I2C)
- **Pull-ups:** Internal 45kΩ (external 4.7kΩ recommended for reliability)

## Communication Protocol

### I2C Command Format

**Command Structure (Master → Slave):**
```
[CMD_BYTE] [DATA_LENGTH] [DATA_0] [DATA_1] ... [DATA_N]
```

**Response Structure (Slave → Master):**
```
[STATUS] [DATA_LENGTH] [DATA_0] [DATA_1] ... [DATA_N]
```

### Command Set

| Command | Value | Description | Data Format |
|---------|-------|-------------|-------------|
| `CMD_PING` | 0x00 | Ping ESP32-S3 | None → Returns 0xA5 |
| `CMD_GET_STATUS` | 0x01 | Get WiFi/BLE status | None → [wifi_status, ble_status, rssi] |
| `CMD_WIFI_CONNECT` | 0x10 | Connect to WiFi | [ssid_len, ssid..., pwd_len, pwd...] |
| `CMD_WIFI_DISCONNECT` | 0x11 | Disconnect WiFi | None |
| `CMD_WIFI_SCAN` | 0x12 | Scan WiFi networks | None → [num_networks, ...] |
| `CMD_BLE_BEACON` | 0x20 | Start BLE beacon | [beacon_data...] |
| `CMD_BLE_STOP` | 0x21 | Stop BLE beacon | None |
| `CMD_WEB_SERVER` | 0x30 | Start web server | [port_high, port_low] |
| `CMD_HTTP_GET` | 0x31 | HTTP GET request | [url_len, url...] |
| `CMD_LED_NOTIFY` | 0x40 | LED notification received | [r, g, b, pattern] |
| `CMD_BUTTON_EVENT` | 0x41 | Button press notification | [button_id, press_type] |
| `CMD_BATTERY_LEVEL` | 0x42 | Send battery level | [voltage_high, voltage_low] |

### Status Codes

| Status | Value | Description |
|--------|-------|-------------|
| `STATUS_OK` | 0x00 | Command successful |
| `STATUS_ERROR` | 0xFF | Generic error |
| `STATUS_BUSY` | 0x01 | ESP32-S3 busy, retry later |
| `STATUS_INVALID_CMD` | 0x02 | Unknown command |
| `STATUS_WIFI_DISCONNECTED` | 0x10 | WiFi not connected |
| `STATUS_WIFI_CONNECTED` | 0x11 | WiFi connected |
| `STATUS_BLE_ACTIVE` | 0x20 | BLE beacon active |

## Use Cases

### 1. Badge-to-Badge Communication
- **RP2040:** Detects button press for "send message"
- **ESP32-S3:** Broadcasts BLE beacon with message
- **Other badges:** Receive BLE beacon, display on LED matrix

### 2. Web-Controlled LED Matrix
- **ESP32-S3:** Runs web server on port 80
- **User:** Connects via smartphone to badge's WiFi
- **Web interface:** Sends animation commands to RP2040 via I2C
- **RP2040:** Displays custom animations on LED matrix

### 3. Live Status Dashboard
- **RP2040:** Periodically sends battery level, button states
- **ESP32-S3:** Uploads to cloud (MQTT, HTTP POST)
- **Dashboard:** Real-time monitoring of badge fleet at conference

### 4. OTA Badge Updates
- **ESP32-S3:** Downloads new RP2040 firmware via WiFi
- **RP2040:** Enters bootloader mode
- **ESP32-S3:** Transfers firmware chunks to RP2040 flash

### 5. Interactive Badge Games
- **Multi-badge tag:** Use BLE RSSI for proximity detection
- **IR + BLE hybrid:** IR for line-of-sight "zapping", BLE for score sync
- **Mesh network:** ESP32-S3 creates badge mesh for group activities

## Features Enabled by ESP32-S3

### WiFi Capabilities
- **Web Server:** Control badge via browser
- **REST API:** External control and monitoring
- **WiFi Direct:** Badge-to-badge file transfer
- **NTP Client:** Accurate timekeeping for clock displays
- **mDNS:** Discoverable as `badge-XXXX.local`

### Bluetooth Capabilities
- **BLE Beacon:** Broadcast badge ID and status
- **BLE GATT Server:** Mobile app control
- **BLE Mesh:** Multi-badge networking
- **Proximity Detection:** RSSI-based distance estimation
- **Nordic UART Service:** Serial communication over BLE

### Additional Features
- **Camera (optional):** ESP32-S3 Sense variant has camera connector
- **MicroSD (optional):** Store animations, logs, photos
- **Extra GPIOs:** 11+ available for sensors, displays, etc.

## Power Considerations

### Current Consumption

| Component | Typical | Peak | Notes |
|-----------|---------|------|-------|
| RP2040 (badge) | 40-60mA | 100mA | Running LED animations |
| LED Matrix | 50-200mA | 400mA | At 15% brightness, ~25% pixels |
| ESP32 ProS3 (idle) | 25-45mA | - | WiFi off, BLE off |
| ESP32 ProS3 (WiFi) | 90-180mA | 200mA | Active WiFi transmission |
| ESP32 ProS3 (BLE) | 30-50mA | 100mA | BLE advertising |
| ESP32 ProS3 (sleep) | 10µA | - | Ultra-low deep sleep mode |
| **Total (typical)** | **150-350mA** | **800mA** | Depends on usage |

### Battery Life Estimates (2×AA, 2500mAh)

| Usage Mode | Current | Battery Life |
|------------|---------|--------------|
| LED idle + ESP32 sleep | 100mA | ~25 hours |
| LED animations + BLE beacon | 250mA | ~10 hours |
| LED + WiFi web server | 350mA | ~7 hours |
| Everything active | 500mA | ~5 hours |

### Power Management Strategies

1. **ESP32-S3 Sleep Modes:**
   - Light sleep: 0.8mA (wake on I2C)
   - Deep sleep: 10-150µA (wake on GPIO)
   - Modem sleep: WiFi off between transmissions

2. **RP2040 Control:**
   - Send `CMD_SLEEP` when WiFi/BLE not needed
   - Wake ESP32-S3 only when user requests network features
   - Reduce LED brightness when battery <2.5V

3. **Voltage Monitoring:**
   - RP2040 monitors battery (IO#26)
   - Send battery level to ESP32-S3 for cloud reporting
   - Auto-disable WiFi when battery <2.3V

## Software Architecture

### RP2040 Firmware (Master)
```cpp
// Main loop structure
void loop() {
  // 1. Handle LED animations
  updateLEDMatrix();

  // 2. Check button inputs
  if (buttonPressed()) {
    sendButtonEventToESP32();
  }

  // 3. Monitor battery
  if (timeToUpdateBattery()) {
    sendBatteryLevelToESP32();
  }

  // 4. Check for ESP32 notifications
  if (esp32HasData()) {
    receiveESP32Command();
  }
}
```

### ESP32-S3 Firmware (Slave)
```cpp
// I2C event handlers
void onReceive(int bytes) {
  // Process command from RP2040
  uint8_t cmd = Wire.read();
  handleCommand(cmd);
}

void onRequest() {
  // Send response to RP2040
  Wire.write(responseData, responseLength);
}

// Main loop
void loop() {
  // Handle WiFi/BLE tasks
  handleWiFi();
  handleBLE();
  handleWebServer();
}
```

## Installation Steps

### 1. Hardware Assembly
- Solder SAO header to badge (top side, 2×2 pin arrangement)
- Prepare XIAO ESP32-S3 with male header pins
- Connect ESP32-S3 to SAO port
- Verify 3.3V and GND connections with multimeter

### 2. Software Setup
- Install Arduino IDE board support for both platforms
- Install required libraries (Wire, WiFi, BLE)
- Upload RP2040 firmware to badge
- Upload ESP32-S3 firmware to XIAO
- Test I2C communication with ping command

### 3. Testing
- Use Serial monitors on both devices
- Test basic I2C ping/pong
- Test WiFi connection
- Test BLE beacon
- Test LED control via web interface

## Troubleshooting

### I2C Communication Issues
- **No response:** Check pull-up resistors (4.7kΩ)
- **Random data:** Reduce I2C speed to 100kHz
- **Address conflict:** Change ESP32-S3 address (avoid 0x3C, 0x68, etc.)

### Power Issues
- **ESP32-S3 brown-out:** Add 10µF capacitor across 3.3V/GND
- **Badge resets:** Reduce LED brightness when WiFi active
- **Battery drains fast:** Enable ESP32-S3 light sleep

### WiFi Issues
- **Can't connect:** Verify 2.4GHz network (ESP32 doesn't support 5GHz)
- **Weak signal:** ESP32-S3 antenna orientation matters
- **Slow response:** Reduce LED animation complexity

## Firmware and Documentation

### Firmware Directories:
- `RP2040_Master/` - Badge firmware with I2C master (basic)
- `RP2040_Enhanced/` - Badge firmware with QR code, cat animation, web LED
- `ESP32_ProS3_Slave/` - ProS3 WiFi/BLE coprocessor (basic)
- `ESP32_ProS3_Enhanced/` - ProS3 WiFi AP + web server with LED designer
- `Web_Interface/` - HTML/JS for LED matrix web control

### Documentation:
- `README.md` - This file (system overview)
- `WIRING.txt` - Complete wiring guide and pinout diagrams
- `ESP32_PROS3_SETUP.md` - **ProS3 Arduino IDE setup guide**
- `GETTING_STARTED.md` - Quick start tutorial
- `ENHANCED_FEATURES.md` - QR code, cat animation, web interface guide
- `LIBRARY_SETUP.md` - Library installation instructions

## Safety Notes

⚠️ **Important:**
- Verify 3.3V supply can handle combined current (RP2040 + ESP32-S3 + LEDs)
- Monitor battery voltage closely with WiFi enabled
- Use voltage regulator if powering from batteries
- ESP32-S3 has 3.3V I/O - do not connect to 5V signals

## Next Steps

1. Test basic I2C communication (ping/pong)
2. Implement WiFi connection manager
3. Create web interface for LED control
4. Add BLE beacon for badge-to-badge communication
5. Develop mobile app for badge control
6. Create badge mesh network protocol

## References

### Hardware:
- **ESP32 ProS3:** https://esp32s3.com/pros3.html
- **ProS3 GitHub:** https://github.com/UnexpectedMaker/esp32s3
- **SAO Specification:** https://hackaday.io/project/52950-shitty-add-ons
- **STEMMA QT:** https://learn.adafruit.com/introducing-adafruit-stemma-qt

### Software:
- **ESP32 Arduino Core:** https://github.com/espressif/arduino-esp32
- **ArduinoJson:** https://arduinojson.org/
- **I2C Protocol:** https://www.i2c-bus.org/

### Community & Support:
- **Unexpected Maker Discord:** https://unexpectedmaker.com/discord
- **ESP32 Forum:** https://esp32.com/
- **Framework Badge Repository:** https://github.com/mkohler99/Framework-2025-Badge
