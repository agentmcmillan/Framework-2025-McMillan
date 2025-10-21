# Framework Badge + XIAO ESP32-S3 Dual-Microcontroller System

Integration of Seeed XIAO ESP32-S3 via SAO port to add WiFi/Bluetooth capabilities to the RP2040-based Framework Badge.

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

### XIAO ESP32-S3 - I2C Slave
- **Role:** Wireless communication coprocessor
- **I2C Slave Address:** 0x42 (configurable)
- **Capabilities:**
  - WiFi (2.4GHz 802.11 b/g/n)
  - Bluetooth 5.0 (BLE)
  - Optional: Camera, microSD, extra GPIOs
  - Web server for badge control
  - BLE beacon/mesh networking
  - OTA firmware updates

## Pin Connections

### SAO Port (4-pin standard v0.1)

```
Framework Badge SAO Port          XIAO ESP32-S3
─────────────────────────         ─────────────────
Pin 1: VCC (+3.3V)            →   5V or 3V3 pin
Pin 2: GND                    →   GND
Pin 3: SDA (IO#23 or IO#24)   →   GPIO 6 (D4/SDA)
Pin 4: SCL (IO#24 or IO#25)   →   GPIO 7 (D5/SCL)
```

**Note:** SAO port reuses Whisker header pins in a different physical arrangement.

### RP2040 I2C Configuration
- **SDA:** IO#23 (or IO#24, configurable)
- **SCL:** IO#24 (or IO#25, configurable)
- **I2C Speed:** 100kHz (standard) or 400kHz (fast mode)
- **Pull-ups:** 4.7kΩ on both SDA and SCL (may be built into badge)

### ESP32-S3 I2C Configuration
- **SDA:** GPIO 6 (D4, default I2C)
- **SCL:** GPIO 7 (D5, default I2C)
- **I2C Address:** 0x42 (default, avoid conflicts with 0x3C for OLED, etc.)

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
| RP2040 | 40-60mA | 100mA | Running LED animations |
| LED Matrix | 50-200mA | 400mA | At 15% brightness, ~25% pixels |
| ESP32-S3 (idle) | 20-40mA | - | WiFi off, BLE off |
| ESP32-S3 (WiFi) | 80-120mA | 200mA | Active WiFi transmission |
| ESP32-S3 (BLE) | 30-50mA | 100mA | BLE advertising |
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

## Example Applications

See the following directories:
- `RP2040_Master/` - Badge firmware with I2C master
- `ESP32_S3_Slave/` - WiFi/BLE coprocessor firmware
- `Web_Interface/` - HTML/JS for web control
- `Examples/` - Complete use case implementations

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

- XIAO ESP32-S3 Wiki: https://wiki.seeedstudio.com/xiao_esp32s3_getting_started/
- SAO Specification: https://hackaday.io/project/52950-shitty-add-ons
- ESP32 Arduino Core: https://github.com/espressif/arduino-esp32
- I2C Protocol: https://www.i2c-bus.org/
