# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is the **Framework-2025-Badge** hardware design repository - an interactive RP2040-based pixel badge ("Pixel Kitty") designed for Frame:Work NYC 2025. This repository contains **hardware design files only** (KiCad schematics, PCB layouts, Gerber manufacturing files). Firmware/software development happens in a separate tutorial repository.

**Key Facts:**
- **Hardware Platform:** Raspberry Pi RP2040 microcontroller
- **Display:** 15×7 addressable RGB LED matrix (105 WS2812B-2020 LEDs)
- **Power:** 2×AA batteries or USB Type-C
- **Design Tool:** KiCad 6.x
- **License:** MIT No Attribution

## Repository Structure

```
HARDWARE/                                    # Hardware design files
├── Documentation/                          # Human-readable design docs, BOM, renders
│   ├── 3D/                                # STEP 3D model
│   ├── bom/                               # Bill of Materials (CSV + interactive HTML)
│   └── Renders/                           # Product renders (Front/Back/Isometric)
├── KiCad/TagTag/                          # Editable design files
│   ├── TagTag.kicad_pro                   # KiCad project file
│   ├── TagTag.kicad_sch                   # Schematic (56,272 lines)
│   ├── TagTag.kicad_pcb                   # PCB layout (4-layer, 2.8 MB)
│   └── Library/                           # Component symbols, footprints, 3D models
└── Gerber/                                # Manufacturing files (ready for PCB fab)

Seeed_Arduino_RoundDisplay-main/           # LVGL library for round TFT displays (expansion port)
Seeed_GFX-master/                          # TFT_eSPI-based graphics library (expansion port)
led_ripple.py                              # MicroPython LED ripple animation (button-triggered)
```

## Hardware Architecture

**Core Components:**
- **RP2040** microcontroller (dual-core ARM Cortex-M0+, 264KB RAM)
- **W25Q32JVSSIQ** 4MB SPI flash storage
- **105× WS2812B-2020** RGB LEDs in 15×7 matrix
- **TPS61022RWUR** boost converter for LED power regulation
- **IRM-H638T_TR2** 38kHz IR receiver
- **IR15-21C/TR8** 940nm IR transmitter LED

**Key Pinout (GPIO):**
- **IO#3:** Infrared Transmitter (active high)
- **IO#4:** LED Matrix (WS2812B data line)
- **IO#5, #6, #7:** Buttons Left/Center/Right (active low)
- **IO#11:** Status LED (red)
- **IO#12-19:** DVI display output (expansion port)
- **IO#23-25:** Whisker expansion I/O
- **IO#26:** Battery voltage monitor (ADC, 0.357× scaling for 0-9V range)
- **IO#27:** Infrared Receiver (38kHz)
- **IO#28:** Whisker analog input (ADC)

## Working with Hardware Design

### Opening the Design
```bash
# Navigate to KiCad project
cd HARDWARE/KiCad/TagTag/

# Open in KiCad 6.x (must be version 6.x or compatible)
kicad TagTag.kicad_pro
```

### Viewing Documentation
```bash
# View schematic PDF
open HARDWARE/Documentation/Framework\ Badge\ Tag\ Schematic.pdf

# View PCB layout PDF
open HARDWARE/Documentation/Framework\ Badge\ Tag\ PCB\ Layout.pdf

# View interactive BOM (component placement)
open HARDWARE/Documentation/bom/ibom.html

# View 3D model (requires CAD software)
open HARDWARE/Documentation/3D/FrameworkBadgeTagv1_2.STEP
```

### Manufacturing Files
Gerber files in `HARDWARE/Gerber/` are production-ready:
- **PCB Layers:** 4-layer design (F_Cu, In1_Cu, In2_Cu, B_Cu)
- **Files:** 22 total (copper, soldermask, silkscreen, paste, drill)
- **Format:** RS-274X Gerber + Excellon drill files

Submit `TagTag-job.gbrjob` along with all .gbr and .drl files to PCB manufacturer.

## Critical Design Notes

### ⚠️ LED Power Management
**NEVER power the full LED matrix at full brightness in white.** This is a hardware safety issue:

- Each LED can draw up to 36mA (12mA × 3 channels)
- 105 LEDs × 36mA = 3,780mA theoretical maximum
- Board can reliably supply only ~2,500mA
- **Exceeding limits causes:** hardware damage, battery depletion, component overheating/burns

**Safe Operating Parameters:**
- Limit brightness to ≤25%
- Light ≤1/4 of pixels simultaneously
- Use power management libraries (e.g., FastLED with runtime power estimation)
- Target: 8+ hours battery life with typical text display usage

### LED Matrix Addressing
- **Layout:** 15 columns × 7 rows
- **Numbering:** Top-left to bottom-right, **by columns** (NOT snake pattern)
- **First column:** LEDs 1-7 (top to bottom)
- **Second column:** LEDs 8-14 (top to bottom)
- **Last column:** LEDs 99-105 (top to bottom)

## Firmware Development

### Programming the Badge
1. Connect via USB Type-C
2. Hold **BOOT** button, tap **RESET** button
3. RP2040 enters bootloader mode (appears as USB drive)
4. Copy compiled UF2 firmware file to drive

### Development Environments

**Arduino IDE** (recommended for beginners):
- **Setup Guide:** https://learn.adafruit.com/adafruit-feather-rp2040-pico/arduino-ide-setup
- **Tutorial Repository:** https://github.com/mkohler99/FrameworkNYBadge_Tutorials
- **Required Libraries for LED Matrix:**
  - `Adafruit_NeoPixel` (for WS2812B LED control)
  - `FastLED` (alternative with power management)
- **TFT Display Libraries (if using Whisker expansion):**
  - See `Seeed_GFX-master/` for TFT/LCD displays
  - See `Seeed_Arduino_RoundDisplay-main/` for LVGL-based UIs
  - Requires `TFT_eSPI` configuration for RP2040 + your display

**MicroPython** (Python-based):
- Built-in `neopixel` module for LED matrix
- Example code: `led_ripple.py` (button-triggered rainbow ripple)
- Install: Download MicroPython UF2 from https://micropython.org/download/rp2040/
- No TFT library support in this repo (Arduino only)

**Raspberry Pi Pico SDK** (C/C++ with Visual Studio Code):
- Direct hardware access
- Best performance for complex animations
- WS2812B control via PIO (Programmable I/O)

## Expansion Ports

### Whisker Ports (1×5 headers)
Standard expansion I/O on bottom of board:
- **+5V** - LED power rail
- **+3.3V** - Logic power
- **GND** - Ground
- **IO#23, #24, #25** - Digital I/O (SPI/I2C capable)
- **IO#28 (ADC#2)** - Analog input

**TFT Display Expansion:**
This repository includes Seeed Studio graphics libraries for connecting TFT displays to the Whisker ports:

1. **Seeed_Arduino_RoundDisplay-main**: LVGL-based library for round TFT displays
   - Requires: IO pins for SPI (MOSI, SCK, CS, DC, RST)
   - Supports: Touch panels, RTC integration, SD card
   - Use cases: Interactive UI, clock displays, GIF player

2. **Seeed_GFX-master**: TFT_eSPI-based graphics library
   - 300+ example sketches (clocks, animations, sprites, meters)
   - E-Paper display support
   - DMA acceleration on RP2040
   - Configure using: Online tool at https://seeed-studio.github.io/Seeed_GFX/

**Pin Mapping for TFT Displays (via Whisker Port):**
- **SPI MOSI:** Can use IO#23 or IO#24
- **SPI SCK:** Can use IO#24 or IO#25
- **CS (Chip Select):** Any available GPIO
- **DC (Data/Command):** Any available GPIO
- **RST (Reset):** Any available GPIO (or use 3.3V rail)
- **Power:** 3.3V (logic) and 5V (backlight if needed)

### DVI Port (Adafruit DVI Sock)
HDMI output capability (640×480 DVI signal):
- **Uses:** IO#12-19
- **Compatible with:** Adafruit DVI Sock (#5957)
- **Reference:** https://github.com/Wren6991/Pico-DVI-Sock

### SAO Port (Simple Add-Ons)
4-pin badge accessory standard (v0.1):
- **Uses:** Whisker header pins in different configuration
- **Spec:** https://hackaday.io/project/52950-shitty-add-ons

## Component Sourcing

**Bill of Materials:** `HARDWARE/Documentation/bom/bom.csv`
- All components include LCSC part numbers
- 105× WS2812B-2020 LEDs (LCSC: C965555)
- RP2040 microcontroller
- ~40 passive components + ICs

## Key Datasheets

- **WS2812B-2020 LEDs:** https://www.lcsc.com/datasheet/C965555.pdf
- **IRM-H638T/TR2 IR Receiver:** https://www.everlighteurope.com/custom/files/datasheets/DMO-0000008.pdf
- **RP2040:** https://www.raspberrypi.com/documentation/microcontrollers/silicon.html#rp2040

## Design Validation

This is a **production-ready design** (manufactured and tested). When modifying:

1. **Run Design Rule Check (DRC)** in KiCad PCB editor
2. **Run Electrical Rule Check (ERC)** in KiCad schematic editor
3. **Verify component footprints** match datasheets
4. **Check power distribution** (trace widths, copper pours)
5. **Regenerate Gerber files** after changes (File → Plot)
6. **Export updated BOM** from schematic

## Firmware Examples in Repository

### 1. led_ripple.py (MicroPython)
Button-triggered rainbow ripple animation for the LED matrix.

**Features:**
- Press center button (IO#6) to trigger rainbow ripple effect
- Emanates from center of 15×7 matrix
- Safe brightness limiting (15% max) to prevent hardware damage
- Idle animation (breathing center pixel) when waiting

**Key Functions:**
- `rainbow_ripple_from_center()` - Main ripple effect
- `xy_to_index(x, y)` - Converts grid coords to LED index (column-based addressing)
- `hsv_to_rgb(h, s, v)` - Color conversion for rainbow effects

**Safety:** Implements power management to stay within 2,500mA board limit

### 2. Badge_LED_TFT_Demo/ (Arduino)
Comprehensive Arduino sketch combining LED matrix with GC9A01 round TFT display.

**Location:** `Badge_LED_TFT_Demo/Badge_LED_TFT_Demo.ino`

**Features:**
- LED matrix rainbow ripple (triggered by center button)
- TFT status display showing:
  - Real-time battery voltage with color-coded warnings
  - Button state indicators
  - Visual feedback for animations
- Simultaneous control of both displays
- Safe power management for battery operation

**Hardware:**
- Uses Whisker port (IO#23-25) for TFT SPI connection
- Additional GPIOs (IO#20-21) for TFT control signals
- GC9A01 240×240 round display via SPI

**Required Libraries:**
- `Adafruit_NeoPixel` - LED matrix control
- `Seeed_GFX` - TFT display driver (included in repo)

**Configuration:**
- See `Badge_LED_TFT_Demo/README.md` for wiring guide
- `User_Setup.h` configures TFT pins for RP2040
- Pin mapping matches Whisker port connections

**Key Files:**
- `Badge_LED_TFT_Demo.ino` - Main sketch (500+ lines)
- `User_Setup.h` - TFT_eSPI configuration for RP2040 + GC9A01
- `README.md` - Complete documentation, wiring, troubleshooting

### 3. Dual_MCU_Integration/ (Advanced) ⭐ NEW
Complete dual-microcontroller system adding WiFi/Bluetooth via ESP32-C6 coprocessor.

**Location:** `Dual_MCU_Integration/`

**System Architecture:**
- **RP2040 (Master):** Badge control, LED matrix, TFT display, buttons
- **ESP32-C6 (Slave):** WiFi 6 AP, web server, BLE 5.3 beacon, I2C communication

**ESP32-C6 Advantages:**
- **WiFi 6 (802.11ax)** - Better performance in crowded networks
- **BLE 5.3** - Longer range, faster speeds vs BLE 5.0
- **Lower power consumption** - Better battery life
- **RISC-V architecture** - Open-source instruction set

**Enhanced Features:**
- **WiFi 6 Access Point:** Connect via phone to "PixelKitty-XXXX" network
- **Web-based LED Designer:** Draw patterns on 15×7 grid in browser
- **QR Code Display:** RIGHT button shows WiFi QR code on TFT (30 seconds)
- **Animated Cat:** Pixel art cat walking on TFT after QR timeout
- **Pattern Transfer:** Web → ESP32 → I2C → RP2040 → LED Matrix

**Firmware Versions:**

**Basic (I2C Foundation):**
- `RP2040_Master/RP2040_Master.ino` - I2C master with basic commands
- `ESP32_S3_Slave/ESP32_S3_Slave.ino` - I2C slave with WiFi 6/BLE 5.3 (updated for C6)

**Enhanced (Full Features):**
- `RP2040_Enhanced/RP2040_Enhanced.ino` - QR code + animated cat + web LED
- `ESP32_S3_Enhanced/ESP32_S3_Enhanced.ino` - WiFi 6 AP + web server + captive portal (updated for C6)

**Required Libraries:**
- RP2040: `Adafruit_NeoPixel`, `Seeed_GFX`, `QRCode` (new)
- ESP32-C6: `ArduinoJson` (new), WiFi/WebServer/DNSServer (built-in)

**Key Files:**
- `README.md` - System architecture and use cases
- `WIRING.txt` - SAO port connection diagram
- `ESP32_C6_SETUP.md` - ESP32-C6 board support installation
- `ENHANCED_FEATURES.md` - Complete feature guide (QR code, cat, web)
- `LIBRARY_SETUP.md` - Step-by-step library installation
- `Web_Interface/led_matrix.html` - Interactive LED designer webpage

**Connection:**
- ESP32-C6 connects to badge via SAO port (4-pin: VCC, GND, SDA, SCL)
- I2C communication at 100kHz
- I2C pins: SDA=GPIO6 (D4), SCL=GPIO7 (D5) - same as S3
- Slave address: 0x42

## Version Control Notes

This repository has **13 commits** (hardware design complete). Large files:
- Total size: ~49.6 MB
- KiCad files: 41 MB (includes embedded 3D models)
- PCB file: 2.8 MB
- Seeed libraries: External Arduino libraries (not included in original design)

When making changes, commit only modified KiCad source files, not generated outputs (Gerber/BOM), unless those are being updated for production.

## Project Context

**Event:** Frame:Work NYC 2025
**Sponsor:** Fuse Technical Group (https://www.fuse-tg.com)
**Author:** Michael Kohler (mkohler@icloud.com)
**Repository:** https://github.com/mkohler99/Framework-2025-Badge
