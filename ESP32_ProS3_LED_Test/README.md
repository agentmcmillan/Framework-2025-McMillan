# ESP32 ProS3 RGB LED Test

Simple blink sketch for the Unexpected Maker ESP32-S3 ProS3's onboard WS2812B RGB LED.

## Hardware

- **Board:** Unexpected Maker ESP32-S3 ProS3
- **RGB LED:** WS2812B on GPIO48 (built-in)
- **Power:** USB-C

## What It Does

Cycles through colors on the onboard RGB LED:
1. 🔴 Red
2. 🟢 Green
3. 🔵 Blue
4. 🔵 Cyan
5. 🟣 Magenta
6. 🟡 Yellow
7. ⚪ White
8. ⚫ Off

Each color displays for 500ms, then repeats.

## Required Library

Install via Arduino Library Manager:
- **Adafruit_NeoPixel**

## Board Setup

### Arduino IDE Settings

**Tools → Board:**
- Select: **ESP32S3 Dev Module**
- Or: **UM ProS3** (if available in your ESP32 board package)

**Tools → Configuration:**
```
USB CDC On Boot: Enabled
USB DFU On Boot: Disabled
Flash Size: 16MB (128Mb)
Partition Scheme: Default 4MB with spiffs
PSRAM: OPI PSRAM
Upload Speed: 921600
```

### Installing ESP32 Board Support

If you don't have ESP32 boards installed:

1. **File → Preferences**
2. Add to "Additional Board Manager URLs":
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
3. **Tools → Board → Boards Manager**
4. Search: **esp32**
5. Install: **esp32 by Espressif Systems** (version 3.0.0+)

## Upload Instructions

1. Connect ProS3 via USB-C
2. Open `ESP32_ProS3_LED_Test.ino`
3. Select board and port (Tools → Port)
4. Click **Upload**

**Note:** If upload fails:
- Press and hold **BOOT** button
- Press **RESET** button (while holding BOOT)
- Release **RESET**, then release **BOOT**
- Click Upload immediately

## Serial Monitor Output

Open Serial Monitor (115200 baud) to see:
```
╔════════════════════════════════════════╗
║  UM ESP32 ProS3 - RGB LED Test         ║
╚════════════════════════════════════════╝

RGB LED initialized on GPIO48
Starting color cycle...

🔴 Red
🟢 Green
🔵 Blue
🔵 Cyan
🟣 Magenta
🟡 Yellow
⚪ White
⚫ Off
```

## Troubleshooting

### LED Doesn't Light Up

1. **Check GPIO pin:** ProS3 RGB LED is on **GPIO48** (not GPIO38 like some other boards)
2. **Verify board selection:** Must be ESP32-S3 variant
3. **Check USB cable:** Some cables are charge-only (no data)
4. **Brightness too low:** Try increasing brightness in sketch (line 32)

### Upload Fails

1. **Install CH340 drivers** (if using older ProS3 units)
2. **Use manual boot mode:** Hold BOOT, tap RESET
3. **Try different USB port**
4. **Reduce upload speed:** Tools → Upload Speed → 115200

### Serial Monitor Shows Gibberish

1. **Check baud rate:** Must be 115200
2. **Enable USB CDC:** Tools → USB CDC On Boot → Enabled
3. **Reset board** after upload

## Advanced Examples

### Breathing Effect

```cpp
void loop() {
  // Breathe red
  for (int brightness = 0; brightness < 255; brightness++) {
    led.setPixelColor(0, led.Color(brightness, 0, 0));
    led.show();
    delay(5);
  }
  for (int brightness = 255; brightness >= 0; brightness--) {
    led.setPixelColor(0, led.Color(brightness, 0, 0));
    led.show();
    delay(5);
  }
}
```

### Rainbow Cycle

```cpp
void loop() {
  for (int hue = 0; hue < 65536; hue += 256) {
    uint32_t color = led.ColorHSV(hue);
    led.setPixelColor(0, color);
    led.show();
    delay(10);
  }
}
```

### Status Indicator

```cpp
// Red = error, Green = ready, Blue = processing, Yellow = warning
void setStatus(String status) {
  if (status == "error") {
    led.setPixelColor(0, led.Color(255, 0, 0));
  } else if (status == "ready") {
    led.setPixelColor(0, led.Color(0, 255, 0));
  } else if (status == "processing") {
    led.setPixelColor(0, led.Color(0, 0, 255));
  } else if (status == "warning") {
    led.setPixelColor(0, led.Color(255, 255, 0));
  }
  led.show();
}
```

## Technical Details

### WS2812B Specifications

- **Protocol:** Single-wire addressable RGB
- **Voltage:** 3.3V logic (ProS3 native)
- **Data Rate:** 800kHz
- **Color Order:** GRB (not RGB!)
- **Brightness:** 0-255 per channel
- **Power:** ~60mA at full white brightness

### ProS3 RGB LED Pinout

```
ESP32-S3 ProS3
┌─────────────────┐
│                 │
│  [USB-C Port]   │
│                 │
│  GPIO48 ───┐    │
│            │    │
│        ┌───▼──┐ │
│        │ RGB  │ │  ← WS2812B LED
│        │ LED  │ │
│        └──────┘ │
│                 │
└─────────────────┘
```

### Memory Usage

- **Sketch:** ~250KB
- **RAM:** ~8KB
- **Flash:** <1KB

Plenty of room for expansion!

## Using with Badge Project

This RGB LED is useful for status indication when the ProS3 is used as the ESP32 coprocessor:

- **Red:** WiFi connection failed
- **Green:** WiFi connected, web server running
- **Blue:** BLE beacon active
- **Yellow:** I2C communication error
- **Cyan:** Receiving data from RP2040
- **Magenta:** OTA update in progress

See `Dual_MCU_Integration/ESP32_S3_Enhanced/` for integration example.

## Reference

- **ProS3 Pinout:** https://esp32s3.com/pros3.html
- **ProS3 Schematic:** https://github.com/UnexpectedMaker/esp32s3-docs
- **WS2812B Datasheet:** https://cdn-shop.adafruit.com/datasheets/WS2812B.pdf
- **Adafruit NeoPixel Guide:** https://learn.adafruit.com/adafruit-neopixel-uberguide

## License

MIT No Attribution (MIT-0)
