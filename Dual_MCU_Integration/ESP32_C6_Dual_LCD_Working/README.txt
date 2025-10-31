# ESP32-C6 Dual LCD Cat Eyes - WORKING VERSION

This directory contains the ESP32-C6 compatible version of the dual LCD cat eyes firmware.

## Quick Start

1. Open Arduino IDE
2. File → Open → ESP32_C6_Dual_LCD_Working.ino
3. Tools → Board → ESP32 Arduino → XIAO_ESP32C6
4. Tools → Partition Scheme → Default 4MB with spiffs
5. Click Verify ✓

## Libraries Used

This version uses Adafruit libraries which are fully compatible with ESP32-C6:
- Adafruit_GFX
- Adafruit_BusIO
- Adafruit_ST7789 (GC9A01 compatible)

These are already installed if you ran install_arduino_deps.sh

## Why a Separate Directory?

Arduino IDE loads all .ino files in a directory as tabs. The other versions
(using TFT_eSPI and Arduino_GFX) don't compile on ESP32-C6, so this working
version is in its own directory to avoid conflicts.

## Features

✅ Dual GC9A01 round displays (240x240)
✅ Cat eye animations
✅ Web interface for control
✅ WiFi 6 AP (CatEyes-XXXX)
✅ Color customization
✅ Auto-blinking
✅ 8-direction eye movement

## Wiring

See WIRING_DUAL_LCD.txt for complete pin connections.

Quick reference:
- Display 1: CS=GPIO2, DC=GPIO3, RST=GPIO4
- Display 2: CS=GPIO5, DC=GPIO6, RST=GPIO7
- Shared: MOSI=GPIO21, SCK=GPIO20

## Web Interface

After upload:
1. Connect to WiFi "CatEyes-XXXX" (password: meow1234)
2. Open browser to http://192.168.4.1
3. Control eyes with arrow buttons

## Troubleshooting

If compilation fails, check:
- Board set to XIAO_ESP32C6
- Partition scheme selected
- Adafruit libraries installed
- Arduino IDE restarted after library install

For more help, see the main ESP32_C6_Dual_LCD directory documentation.
