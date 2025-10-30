# Framework Badge 2025 - Quick Start Guide

Get up and running in 5 minutes! 🚀

## 🎯 Choose Your Platform

### macOS (Recommended Path)

```bash
# 1. Install Arduino IDE (if not installed)
# Download from: https://www.arduino.cc/en/software

# 2. Run automated setup
cd ~/Documents/GitHub/Framework-2025-McMillan
./install_arduino_deps.sh

# 3. Follow on-screen instructions to:
#    - Install ESP32 board support (Arduino IDE → Boards Manager)
#    - Install RP2040 board support (Arduino IDE → Boards Manager)

# 4. Restart Arduino IDE

# Done! 🎉
```

### Windows

```batch
REM 1. Install Arduino IDE (if not installed)
REM Download from: https://www.arduino.cc/en/software

REM 2. Run automated setup
cd %USERPROFILE%\Documents\GitHub\Framework-2025-McMillan
install_arduino_deps.bat

REM 3. Follow on-screen instructions

REM Done! 🎉
```

### Linux

Same as macOS:
```bash
cd ~/Documents/GitHub/Framework-2025-McMillan
chmod +x install_arduino_deps.sh
./install_arduino_deps.sh
```

## 📦 What Gets Installed Automatically

✅ **8 Arduino Libraries**:
- Adafruit_NeoPixel (LED matrix)
- QRCode (WiFi QR codes)
- ArduinoJson (Web API)
- TFT_eSPI (LCD displays for RP2040)
- Arduino_GFX (LCD displays for ESP32-C6)
- FastLED (Advanced LEDs)
- Seeed_GFX (Graphics)
- Seeed_Arduino_RoundDisplay (Round LCDs)

✅ **Display Library Configuration**:
- TFT_eSPI configured for GC9A01 on RP2040
- Arduino_GFX for ESP32-C6 compatibility
- Original configs backed up

✅ **Board Manager URLs**:
- ESP32 package URL added
- RP2040 package URL ready

## ⚙️ Manual Steps (After Running Script)

### Step 1: Install ESP32 Board Support

1. Open **Arduino IDE**
2. **Tools → Board → Boards Manager**
3. Search **"esp32"**
4. Install **"esp32 by Espressif Systems"**
5. Wait 5-10 minutes for download

### Step 2: Install RP2040 Board Support

1. **File → Preferences**
2. **Additional Board Manager URLs** - Add:
   ```
   https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json
   ```
3. **Tools → Board → Boards Manager**
4. Search **"pico"**
5. Install **"Raspberry Pi Pico/RP2040"**

### Step 3: Restart Arduino IDE

Close and reopen Arduino IDE to load everything.

## 🎮 Upload Your First Project

### Dual Cat Eyes (ESP32-C6 + 2 LCDs)

1. **Connect ESP32-C6** via USB
2. **Tools → Board → ESP32 Arduino → XIAO_ESP32C6**
3. **Tools → Partition Scheme → Default 4MB with spiffs**
4. **Tools → Port → Select ESP32-C6**
5. **Open**: `Dual_MCU_Integration/ESP32_C6_Dual_LCD/ESP32_C6_Dual_LCD_Adafruit.ino`
6. **Click Upload** ⬆️
7. **Wire displays** (see `WIRING_DUAL_LCD.txt`)
8. **Connect to WiFi** "CatEyes-XXXX" (password: meow1234)
9. **Open browser**: http://192.168.4.1
10. **Control the eyes!** 👁️👁️

**Note:** Use the `_Adafruit.ino` version for ESP32-C6 compatibility (uses Arduino_GFX library).

### LED Ripple (RP2040 Badge - MicroPython)

1. **Connect badge** via USB
2. **Hold BOOT + tap RESET** (enters bootloader)
3. **Download MicroPython** for RP2040:
   - https://micropython.org/download/rp2-pico/
4. **Copy `.uf2` file** to RP2040 USB drive
5. **Copy `led_ripple.py`** to badge
6. **Press center button** to trigger animation

### WiFi Enhanced Badge (RP2040 + ESP32-C6)

1. **Upload ESP32-C6 firmware first**:
   - `Dual_MCU_Integration/ESP32_C6_Enhanced/ESP32_C6_Enhanced.ino`
2. **Connect ESP32 to badge** via SAO port (see `WIRING.txt`)
3. **Upload RP2040 firmware**:
   - `Dual_MCU_Integration/RP2040_Enhanced/RP2040_Enhanced.ino`
4. **Press RIGHT button** to show WiFi QR code
5. **Scan QR** with phone to connect
6. **Draw LED patterns** on web interface

## 🔧 Troubleshooting

### ❌ Script says "Arduino IDE not found"
**→** Install Arduino IDE from https://www.arduino.cc/en/software

### ❌ Script says "Git not found"
**→ macOS**: Run `xcode-select --install`
**→ Windows**: Install from https://git-scm.com/download/win
**→ Linux**: `sudo apt-get install git`

### ❌ Board not in Tools → Board menu
**→** Complete manual Step 1 & 2 above
**→** Restart Arduino IDE

### ❌ Libraries don't show up
**→** Restart Arduino IDE
**→** Check `Sketch → Include Library`
**→** Re-run installation script

### ❌ TFT_eSPI compile errors
**→** Re-run script to reconfigure
**→** Check `~/Documents/Arduino/libraries/TFT_eSPI/User_Setup.h`

### ❌ Upload fails / Port not found
**→** Try different USB cable (must support data)
**→** Check device shows up in system (lsusb / Device Manager)
**→** Install USB drivers (Windows/Linux)

## 📚 Full Documentation

- **Installation Guide**: `INSTALLATION.md` (detailed setup)
- **Project Overview**: `CLAUDE.md` (hardware + firmware)
- **ESP32-C6 Setup**: `ESP32_C6_SETUP.md` (board configuration)
- **Dual LCD Guide**: `Dual_MCU_Integration/ESP32_C6_Dual_LCD/README.md`
- **System Architecture**: `Dual_MCU_Integration/README.md`

## 🎯 Project Locations

```
Framework-2025-McMillan/
├── install_arduino_deps.sh          ← Run this first! (macOS/Linux)
├── install_arduino_deps.bat         ← Run this first! (Windows)
├── INSTALLATION.md                  ← Full installation guide
├── QUICKSTART.md                    ← This file
├── CLAUDE.md                        ← Project documentation
├── led_ripple.py                    ← MicroPython LED demo
│
├── Dual_MCU_Integration/
│   ├── ESP32_C6_Slave/             ← Basic I2C firmware
│   ├── ESP32_C6_Enhanced/          ← WiFi AP + Web server
│   ├── ESP32_C6_Dual_LCD/          ← Cat eyes display ⭐
│   ├── RP2040_Master/              ← Badge I2C master
│   └── RP2040_Enhanced/            ← Badge with QR + cat
│
└── HARDWARE/                        ← KiCad design files
```

## 💡 Pro Tips

- **Use latest Arduino IDE 2.x** - Faster compilation, better UI
- **Enable verbose output** - Tools → Upload → Show verbose output (helps debug)
- **Serial Monitor baud rate** - Set to 115200 for all projects
- **Save often** - Arduino IDE auto-saves, but Ctrl+S is your friend
- **Check Serial Monitor** - Most issues show debug info there
- **Power cycle** - When in doubt, unplug and replug USB
- **Keep backup** - Copy working `.ino` files before major changes

## 🌟 Next Level Projects

After basic setup works:

1. **Add accelerometer** - Eyes follow device tilt
2. **Add microphone** - Eyes look toward sound
3. **Integrate with badge** - Control eyes from badge buttons
4. **Custom animations** - Sleepy, angry, surprised eyes
5. **LED rings** - RGB glow around displays
6. **Touch sensors** - Pet the cat, eyes respond
7. **Voice control** - ESP32 voice recognition
8. **MQTT integration** - Control over internet

## 📞 Need Help?

1. **Check troubleshooting** above
2. **Read full docs** - `INSTALLATION.md`, `CLAUDE.md`
3. **Check Serial Monitor** - Debug output at 115200 baud
4. **Try examples first** - Start with simple projects
5. **GitHub Issues** - Report bugs with details:
   - OS and version
   - Arduino IDE version
   - Script output / error messages
   - Photos of wiring (if hardware issue)

## 🎉 You're Ready!

Installation complete? Great! Now:

1. ✅ Pick a project
2. ✅ Open the `.ino` file
3. ✅ Select board in Tools menu
4. ✅ Click Upload
5. ✅ Connect hardware
6. ✅ Have fun!

---

**Made with 💚 for Frame:Work NYC 2025**

*Get coding, stop configuring!* 🚀
