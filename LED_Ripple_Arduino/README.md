# LED Ripple Animation - Arduino Version

Arduino sketch for the Framework-2025-Badge that creates a rainbow ripple animation on the 15×7 LED matrix.

## Features

- **Continuous Operation**: Runs indefinitely on the RP2040 with automatic idle animation
- **Rainbow Ripple Effect**: Button-triggered rainbow ripple emanating from center
- **Idle Breathing Animation**: Gentle pulsing center pixel when waiting for input
- **Safe Power Management**: 15% brightness limit to prevent hardware damage
- **Button Debouncing**: Built-in debounce handling for reliable button presses

## Hardware Requirements

- **RP2040-based badge** (Raspberry Pi Pico or compatible)
- **15×7 WS2812B-2020 LED matrix** (105 LEDs total) on GPIO #4
- **Center button** on GPIO #6 (active low with internal pull-up)

## Required Arduino Library

Install via Arduino Library Manager:
- **Adafruit NeoPixel** by Adafruit

## Installation

### 1. Install Arduino IDE Setup
Follow the [Adafruit RP2040 setup guide](https://learn.adafruit.com/adafruit-feather-rp2040-pico/arduino-ide-setup)

### 2. Install Required Library
1. Open Arduino IDE
2. Go to **Sketch → Include Library → Manage Libraries**
3. Search for "Adafruit NeoPixel"
4. Click **Install**

### 3. Upload to Badge
1. Open `LED_Ripple_Arduino.ino` in Arduino IDE
2. Select **Tools → Board → Raspberry Pi RP2040 Boards → Raspberry Pi Pico**
3. Connect badge via USB Type-C
4. Put badge in bootloader mode:
   - Hold **BOOT** button
   - Tap **RESET** button
   - Release **BOOT** button
5. Select the RP2040 drive from **Tools → Port**
6. Click **Upload** (or press Ctrl+U)

## Usage

Once uploaded, the sketch runs automatically:

1. **Idle State**: Center pixel gently pulses with a breathing effect
2. **Press CENTER button** (labeled "MEOW"): Triggers rainbow ripple animation
3. **Ripple Effect**: Rainbow waves emanate from center for 2.5 seconds
4. **Return to Idle**: Returns to breathing animation after ripple completes

The badge will continuously run this loop until powered off.

## How It Works

### LED Matrix Addressing
The 15×7 LED matrix is addressed **by columns** (NOT snake pattern):
- **Column 0**: LEDs 0-6 (top to bottom)
- **Column 1**: LEDs 7-13 (top to bottom)
- **Column 14**: LEDs 98-104 (top to bottom)

### Color System
Uses HSV (Hue, Saturation, Value) color space for smooth rainbow transitions:
- **Hue**: 0-360° (creates rainbow colors)
- **Saturation**: 0.0-1.0 (color intensity)
- **Value**: 0.0-1.0 (brightness, limited to 0.15 max)

### Animation Details
**Rainbow Ripple:**
- Duration: 2.5 seconds
- Frame rate: 25 FPS
- Wave speed: Moderate (0.5 units/second)
- Rainbow bands rotate as they expand outward
- Trailing fade effect for smooth appearance

**Idle Breathing:**
- Frame rate: 20 FPS
- Color: Soft cyan/blue (Hue 200°)
- Intensity: 0-50% of max brightness
- Smooth sinusoidal breathing pattern

## Safety Features

### Power Management
**⚠️ IMPORTANT**: This sketch implements critical power management:

- **Maximum brightness**: 15% (MAX_BRIGHTNESS = 0.15)
- **Limited pixel count**: Only small portion lit at once during ripple
- **Safe for batteries**: 8+ hours runtime on 2×AA batteries

**Never modify MAX_BRIGHTNESS above 0.25 without proper power calculations!**

Exceeding safe power limits can cause:
- Hardware damage
- Component overheating/burns
- Rapid battery depletion
- Board failure

### Button Debouncing
- 300ms debounce delay prevents accidental double-triggers
- Reliable button press detection
- Waits for button release before next animation

## Customization

### Adjust Ripple Speed
Change wave propagation speed in `rainbowRippleFromCenter()`:
```cpp
float wavePos = elapsed * 0.5 * 10.0;  // Change 0.5 to adjust speed
```

### Adjust Ripple Duration
Modify the `RIPPLE_DURATION` constant:
```cpp
#define RIPPLE_DURATION  2500  // Duration in milliseconds
```

### Change Idle Color
Modify the hue value in `idleAnimation()`:
```cpp
uint32_t color = hsvToRgb(200, 0.7, intensity * MAX_BRIGHTNESS * 0.5);
                         // ^^ Change hue: 0=red, 120=green, 240=blue
```

### Adjust Animation Frame Rates
```cpp
#define RIPPLE_FPS       25    // Higher = smoother but more CPU usage
#define IDLE_FPS         20    // Idle animation frame rate
```

## Troubleshooting

### LEDs Don't Light Up
1. Check GPIO pin is correct (LED_PIN = 4)
2. Verify LED matrix is connected to GPIO #4
3. Check power connections (GND, +5V, +3.3V)
4. Ensure badge is receiving adequate power

### Button Doesn't Trigger Animation
1. Verify button is on GPIO #6
2. Check button is configured as INPUT_PULLUP (active low)
3. Increase debounce delay if needed
4. Test button with multimeter (should read LOW when pressed)

### Ripple Looks Choppy
1. Increase `RIPPLE_FPS` for smoother animation
2. Reduce complexity of other running code
3. Ensure adequate power supply

### Badge Resets During Animation
1. **Power issue**: LEDs drawing too much current
2. Reduce brightness (lower MAX_BRIGHTNESS)
3. Use fresh batteries or USB power supply
4. Check all ground connections

## Pin Reference

| Component | GPIO | Type | Notes |
|-----------|------|------|-------|
| LED Matrix | 4 | Output | WS2812B data line |
| Button Left | 5 | Input | Active low, internal pull-up |
| Button Center (MEOW) | 6 | Input | Active low, internal pull-up |
| Button Right | 7 | Input | Active low, internal pull-up |

## Code Structure

- **setup()**: Initializes pins, LED strip, serial communication (runs once)
- **loop()**: Main program loop (runs continuously):
  - Checks for button press
  - Runs idle animation
  - Triggers ripple effect when button pressed
  - Returns to idle state

- **rainbowRippleFromCenter()**: Rainbow ripple animation
- **idleAnimation()**: Breathing idle animation
- **hsvToRgb()**: Color space conversion
- **xyToIndex()**: Coordinate to LED index conversion
- **distance()**: Euclidean distance calculation

## License

MIT No Attribution - Free to use, modify, and distribute

## Credits

**Project**: Framework-2025-Badge / Pixel Kitty
**Event**: Frame:Work NYC 2025
**Sponsor**: Fuse Technical Group (https://www.fuse-tg.com)
**Author**: Michael Kohler (mkohler@icloud.com)
**Arduino Port**: Based on original MicroPython version

## Related Examples

- **MicroPython Version**: `led_ripple.py` (root directory)
- **LED + TFT Demo**: `Badge_LED_TFT_Demo/` (adds round TFT display)
- **WiFi Integration**: `Dual_MCU_Integration/` (ESP32-S3 expansion)
