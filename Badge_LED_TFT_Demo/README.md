# Framework Badge - LED Matrix + TFT Display Demo

Arduino sketch demonstrating simultaneous control of the WS2812B LED matrix and a GC9A01 round TFT display via the Whisker expansion port.

## Features

- **LED Matrix (15×7 WS2812B):**
  - Rainbow ripple animation triggered by center button
  - Gentle breathing idle animation
  - Safe power management (15% max brightness)

- **TFT Display (240×240 GC9A01):**
  - Real-time battery voltage monitoring
  - Button state indicators (visual feedback)
  - Status information display
  - Visual ripple trigger indicator

## Hardware Requirements

### Core (Built into Framework Badge)
- RP2040 microcontroller
- 105 WS2812B-2020 RGB LEDs (15×7 matrix) on GPIO 4
- 3 buttons on GPIO 5, 6, 7
- Battery voltage monitor on GPIO 26
- USB Type-C for programming

### Additional (Expansion)
- **GC9A01 240×240 Round TFT Display**
  - 3.3V/5V compatible
  - SPI interface
  - Optional backlight control

### Wiring - Whisker Port to GC9A01

**Whisker Port (1×5 header on bottom of badge):**
```
Pin 1: +5V      -> Display VCC or LED+ (if 5V tolerant)
Pin 2: +3.3V    -> Display VCC (if 3.3V only)
Pin 3: GND      -> Display GND
Pin 4: IO#23    -> Display MOSI/SDA
Pin 5: IO#24    -> Display SCK/SCL
```

**Additional GPIO Connections (solder to nearby pads or use IO#28):**
```
IO#25 -> Display CS  (Chip Select)
IO#20 -> Display DC  (Data/Command)
IO#21 -> Display RST (Reset) - or tie to 3.3V via 10kΩ resistor
```

**Display Backlight:**
- **Always On:** Connect BL/LED to +3.3V or +5V on Whisker port
- **PWM Control:** Connect to an available GPIO (requires code modification)

### Pin Conflicts to Avoid
- **IO#4:** Reserved for LED matrix (do not use)
- **IO#5, 6, 7:** Reserved for buttons
- **IO#12-19:** Reserved for DVI port (if using)
- **IO#23-25, 28:** Used by Whisker port

## Required Libraries

Install these via Arduino Library Manager:

1. **Adafruit_NeoPixel** (for WS2812B LED matrix)
   - Author: Adafruit
   - Version: 1.10.0 or newer

2. **Seeed_GFX** (TFT_eSPI fork, included in this repo)
   - Location: `../Seeed_GFX-master/`
   - Already in repository, add to Arduino libraries folder

### Installing Seeed_GFX

**Option 1: Copy to Arduino Libraries**
```bash
cp -r ~/Documents/Framework-2025-Badge/Seeed_GFX-master ~/Documents/Arduino/libraries/
```

**Option 2: Symbolic Link (recommended)**
```bash
ln -s ~/Documents/Framework-2025-Badge/Seeed_GFX-master ~/Documents/Arduino/libraries/Seeed_GFX
```

## Configuration

### 1. Configure Seeed_GFX for RP2040 + GC9A01

Copy the included `User_Setup.h` file:

```bash
cp User_Setup.h ~/Documents/Arduino/libraries/Seeed_GFX/User_Setup.h
```

**Or** edit `Seeed_GFX/User_Setup_Select.h`:
- Comment out: `#include <User_Setup.h>`
- Add: `#include <../Badge_LED_TFT_Demo/User_Setup.h>`

### 2. Arduino IDE Board Setup

1. **Install RP2040 Board Support:**
   - Arduino IDE → Preferences
   - Additional Board Manager URLs: `https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json`
   - Tools → Board → Boards Manager → Search "rp2040" → Install

2. **Select Board:**
   - Tools → Board → Raspberry Pi RP2040 Boards → Raspberry Pi Pico

3. **Configure Settings:**
   - CPU Speed: 133 MHz (standard) or 250 MHz (overclock for better performance)
   - Flash Size: 2MB (FS: 1MB)
   - USB Stack: Adafruit TinyUSB

### 3. Upload to Badge

1. Connect badge via USB Type-C
2. Hold **BOOT** button, tap **RESET** button
3. Badge appears as USB drive
4. Upload sketch via Arduino IDE (or copy UF2 file manually)

## Usage

Once uploaded:

1. **Power On:** Badge starts with idle animation (center pixel breathing)
2. **TFT Display Shows:**
   - Title: "PIXEL KITTY"
   - Battery voltage with color-coded indicator:
     - Green: >2.7V (good)
     - Yellow: 2.4-2.7V (medium)
     - Red: <2.4V (low - replace batteries)
   - Button states (left/center/right) as colored circles
   - Instructions: "Press CENTER for ripple!"

3. **Press CENTER Button:** Triggers rainbow ripple animation
   - Ripple emanates from center of LED matrix
   - TFT shows brief visual feedback (cyan circle)
   - Animation lasts ~2.5 seconds
   - Safe brightness levels maintained

4. **Monitor Battery:** TFT updates voltage every 500ms

## Code Structure

### Main Components

- `xyToIndex(x, y)` - Converts grid coordinates to LED strip index
- `hsvToRgb(h, s, v)` - Color conversion for rainbow effects
- `rainbowRipple()` - Main LED animation (rainbow ripple from center)
- `idleAnimation()` - Gentle breathing effect when idle
- `readBatteryVoltage()` - Reads and scales battery voltage
- `updateTFTStatus()` - Updates TFT display with current status
- `drawStaticUI()` - Draws TFT layout (labels, titles, separators)

### Timing

- Main loop: ~50 FPS (20ms delay)
- TFT updates: Every 500ms
- LED animations: 25-40 FPS depending on effect
- Button debounce: 500ms minimum between ripples

## Safety Features

### LED Matrix Power Management
- Maximum brightness limited to 15% (`MAX_LED_BRIGHTNESS = 0.15`)
- Prevents exceeding 2,500mA board current limit
- Never displays full white (all pixels at max)
- Safe for 2×AA battery operation

### Battery Monitoring
- Real-time voltage display on TFT
- Color-coded warnings for low battery
- 10-sample average for stable readings
- Voltage divider scaling: 0.357× (supports 0-9V range)

## Customization

### Change Ripple Colors
Modify `rainbowRipple()` function:
```cpp
float hue = fmod(dist * 40.0 + elapsed * 60.0, 360.0);
// Increase first number for more color bands
// Increase second number for faster color rotation
```

### Adjust Brightness
Modify `MAX_LED_BRIGHTNESS` constant:
```cpp
#define MAX_LED_BRIGHTNESS 0.15  // 15% (safe)
// Max recommended: 0.25 (25%) if <50% pixels lit
```

### Change TFT Colors
Modify color constants (RGB565 format):
```cpp
#define TFT_ACCENT      0x07FF  // Cyan
#define TFT_GOOD        0x07E0  // Green
// Use: http://www.barth-dev.de/online/rgb565-color-picker/
```

### Add More Animations
Template for new animation:
```cpp
void myAnimation() {
  for (int frame = 0; frame < 100; frame++) {
    for (int x = 0; x < LED_COLS; x++) {
      for (int y = 0; y < LED_ROWS; y++) {
        int index = xyToIndex(x, y);
        // Calculate color based on x, y, frame
        uint32_t color = hsvToRgb(hue, 1.0, brightness);
        strip.setPixelColor(index, color);
      }
    }
    strip.show();
    delay(20);
  }
}
```

## Troubleshooting

### TFT Display Issues

**Display not working:**
- Check wiring (especially CS, DC, RST pins)
- Verify `User_Setup.h` pin definitions match your wiring
- Try lower SPI frequency (change `SPI_FREQUENCY` to `10000000`)
- Check power: TFT needs 3.3V or 5V depending on model

**Wrong colors/inverted:**
- Uncomment in `User_Setup.h`: `#define TFT_RGB_ORDER TFT_BGR`

**Display flickers:**
- Add decoupling capacitor (10µF) across VCC and GND near display
- Reduce SPI frequency
- Check power supply stability

### LED Matrix Issues

**LEDs not lighting:**
- Verify GPIO 4 connection
- Check NeoPixel library installed
- Try example: File → Examples → Adafruit_NeoPixel → simple

**Wrong colors:**
- Check LED type in initialization: `NEO_GRB` vs `NEO_RGB`
- Some WS2812B variants use different color orders

**Erratic behavior:**
- Add 470Ω resistor on data line (GPIO 4 to LED DIN)
- Add 1000µF capacitor across LED power supply
- Ensure good ground connection

### Battery Voltage Reading Issues

**Shows 0V or incorrect voltage:**
- GPIO 26 must be configured as analog input
- Check voltage divider on PCB (0.357× scaling)
- Verify batteries are installed and have charge

**Unstable readings:**
- Increase averaging samples (change loop count in `readBatteryVoltage()`)
- Add small delay between samples

## Performance Notes

- **CPU Usage:** ~30% at 133MHz (plenty of headroom)
- **Memory:** ~40KB RAM used (RP2040 has 264KB)
- **Battery Life:**
  - Idle animation: ~10-12 hours on 2×AA
  - Frequent ripples: ~8 hours
  - TFT adds ~20-40mA (minimal impact)

## Next Steps

- Add more button-triggered animations (left/right buttons)
- Display FPS counter on TFT
- Add IR communication with other badges (IO#3, IO#27)
- Create menu system on TFT for selecting animations
- Add sound output (requires additional hardware)
- Store custom patterns in flash memory

## License

MIT No Attribution - Same as Framework Badge hardware

## Credits

- Framework Badge Hardware: Michael Kohler
- LED Animation Code: Based on `led_ripple.py` MicroPython example
- TFT Library: Seeed Studio (fork of Bodmer's TFT_eSPI)
- NeoPixel Library: Adafruit Industries
