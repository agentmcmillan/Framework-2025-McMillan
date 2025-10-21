# Quick Start Guide - Framework Badge LED + TFT Demo

## 1. Install Arduino Libraries

```bash
# Install via Arduino Library Manager:
# - Adafruit NeoPixel

# Copy Seeed_GFX to Arduino libraries folder:
cp -r ../Seeed_GFX-master ~/Documents/Arduino/libraries/Seeed_GFX

# Copy TFT configuration file:
cp User_Setup.h ~/Documents/Arduino/libraries/Seeed_GFX/
```

## 2. Wire the Display

**Whisker Port (1×5 header):**
- Pin 1 (+5V) → TFT VCC (or LED+)
- Pin 2 (+3.3V) → TFT VCC (if 3.3V only)
- Pin 3 (GND) → TFT GND
- Pin 4 (IO#23) → TFT MOSI
- Pin 5 (IO#24) → TFT SCK

**Additional GPIO:**
- IO#25 → TFT CS
- IO#20 → TFT DC
- IO#21 → TFT RST

See `WIRING.txt` for detailed diagram.

## 3. Configure Arduino IDE

**Board Setup:**
1. File → Preferences → Additional Board Manager URLs:
   ```
   https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json
   ```

2. Tools → Board Manager → Install "Raspberry Pi Pico/RP2040"

3. Tools → Board → Raspberry Pi Pico

**Settings:**
- CPU Speed: 133 MHz
- Flash Size: 2MB (FS: 1MB)
- USB Stack: Adafruit TinyUSB

## 4. Upload Sketch

1. Connect badge via USB
2. Hold **BOOT** button, tap **RESET**
3. Badge appears as USB drive
4. Arduino IDE → Upload

## 5. Test

**Expected Behavior:**
- LED matrix shows breathing animation (center pixel)
- TFT displays:
  - Title: "PIXEL KITTY"
  - Battery voltage
  - Button states
  - Instructions

**Press CENTER button:**
- Rainbow ripple animation on LEDs
- TFT shows visual feedback

## Troubleshooting

**TFT not working?**
- Check wiring (especially GND)
- Verify `User_Setup.h` copied correctly
- Try lower SPI speed: Change `SPI_FREQUENCY` to `10000000` in `User_Setup.h`

**LEDs not working?**
- Install Adafruit_NeoPixel library
- Check GPIO 4 connection

**Compilation errors?**
- Verify RP2040 board support installed
- Check both libraries installed correctly

**Battery shows 0V?**
- Install batteries
- Check if USB powered (battery monitor may not work via USB)

## Next Steps

- Read full `README.md` for detailed documentation
- Customize colors and animations
- Add more button-triggered effects
- Monitor battery life during testing

## Pin Reference (Quick)

```
LEDs:    IO#4
Buttons: IO#5 (L), IO#6 (C), IO#7 (R)
Battery: IO#26
TFT SPI: IO#23 (MOSI), IO#24 (SCK)
TFT Ctl: IO#25 (CS), IO#20 (DC), IO#21 (RST)
```

## Safety Reminder

⚠️ **LED matrix limited to 15% brightness** - Do not modify `MAX_LED_BRIGHTNESS` without understanding power consumption limits!
