# Framework Badge 2025 - Installation Guide

Complete setup instructions for the Framework Badge dual-MCU system with automated dependency installation.

## 🚀 Quick Start

### For macOS Users

```bash
cd ~/Documents/GitHub/Framework-2025-McMillan
./install_arduino_deps.sh
```

### For Windows Users

```batch
cd %USERPROFILE%\Documents\GitHub\Framework-2025-McMillan
install_arduino_deps.bat
```

### For Linux Users

Use the macOS script (compatible with Linux):

```bash
cd ~/Documents/GitHub/Framework-2025-McMillan
chmod +x install_arduino_deps.sh
./install_arduino_deps.sh
```

## 📋 What Gets Installed

### Arduino Libraries

The installation script automatically installs these libraries:

| Library | Purpose | Used By |
|---------|---------|---------|
| **Adafruit_NeoPixel** | WS2812B LED control | RP2040 (badge LED matrix) |
| **QRCode** | QR code generation | RP2040 Enhanced (WiFi QR) |
| **ArduinoJson** | JSON parsing | ESP32-C6 (web API) |
| **TFT_eSPI** | LCD display driver | ESP32-C6 (round displays) |
| **FastLED** | Advanced LED effects | RP2040 (optional) |
| **Seeed_GFX** | Graphics primitives | RP2040 (TFT support) |
| **Seeed_Arduino_RoundDisplay** | Round display support | ESP32-C6 (optional) |

### Board Support Packages

**Must be installed manually through Arduino IDE:**

1. **ESP32 by Espressif Systems** (for ESP32-C6)
   - Supports XIAO ESP32-C6 board
   - WiFi 6 and BLE 5.3 support

2. **Raspberry Pi Pico/RP2040** (for Framework Badge)
   - Supports RP2040 microcontroller
   - USB mass storage bootloader

## 🛠️ Prerequisites

### All Platforms

- **Arduino IDE 2.x** (or 1.8.19+)
  - Download: https://www.arduino.cc/en/software
  - Recommended: Arduino IDE 2.3.0 or later

- **Git** (for cloning libraries)
  - macOS: Included with Xcode Command Line Tools
  - Windows: https://git-scm.com/download/win
  - Linux: `sudo apt-get install git`

### Platform-Specific

**macOS:**
- macOS 10.14 (Mojave) or later
- Xcode Command Line Tools: `xcode-select --install`

**Windows:**
- Windows 10 or later
- Administrator privileges (for library installation)

**Linux:**
- Ubuntu 20.04+ (or equivalent)
- Build tools: `sudo apt-get install build-essential`

## 📝 Installation Steps

### Step 1: Install Arduino IDE

If you haven't already, install Arduino IDE:

1. Download from: https://www.arduino.cc/en/software
2. Install according to your platform
3. Run Arduino IDE once to create default directories

### Step 2: Clone or Update Project

```bash
# If you haven't cloned yet:
cd ~/Documents/GitHub
git clone https://github.com/your-username/Framework-2025-McMillan.git

# If already cloned, update:
cd ~/Documents/GitHub/Framework-2025-McMillan
git pull
```

### Step 3: Run Installation Script

**macOS/Linux:**
```bash
cd ~/Documents/GitHub/Framework-2025-McMillan
./install_arduino_deps.sh
```

**Windows:**
```batch
cd %USERPROFILE%\Documents\GitHub\Framework-2025-McMillan
install_arduino_deps.bat
```

The script will:
- ✅ Check for Arduino IDE installation
- ✅ Create necessary directories
- ✅ Configure board manager URLs
- ✅ Clone/update all required libraries
- ✅ Configure TFT_eSPI for GC9A01 displays
- ✅ Create reference documentation
- ℹ️ Display manual steps for board support

### Step 4: Install Board Support (Manual)

**The script will guide you through these steps:**

#### Install ESP32 Board Support

1. Open Arduino IDE
2. Go to **Tools → Board → Boards Manager**
3. Search for **"esp32"**
4. Click **Install** on **"esp32 by Espressif Systems"**
5. Wait for download (~5-10 minutes, ~300MB)
6. Close Boards Manager when complete

#### Install RP2040 Board Support

1. Open Arduino IDE
2. Go to **File → Preferences** (Arduino IDE → Settings on macOS)
3. Find **"Additional Board Manager URLs"**
4. Add this URL:
   ```
   https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json
   ```
5. Click **OK**
6. Go to **Tools → Board → Boards Manager**
7. Search for **"pico"**
8. Click **Install** on **"Raspberry Pi Pico/RP2040"** by Earle F. Philhower
9. Wait for download
10. Close Boards Manager when complete

### Step 5: Restart Arduino IDE

Close and reopen Arduino IDE to load all new libraries and boards.

### Step 6: Verify Installation

1. Open Arduino IDE
2. Go to **Tools → Board**
3. Verify you see:
   - **ESP32 Arduino → XIAO_ESP32C6**
   - **Raspberry Pi RP2040 Boards → Raspberry Pi Pico**
4. Go to **Sketch → Include Library**
5. Verify you see all installed libraries

## 🎯 Select Board for Your Project

### For ESP32-C6 Projects (Dual LCD Cat Eyes, Enhanced WiFi)

1. **Tools → Board → ESP32 Arduino → XIAO_ESP32C6**
2. **Tools → USB CDC On Boot → Enabled**
3. **Tools → CPU Frequency → 160MHz (WiFi 6)**
4. **Tools → Flash Size → 4MB**
5. **Tools → Upload Speed → 921600**
6. **Tools → Port → Select your ESP32-C6 port**

### For RP2040 Projects (Framework Badge, RP2040 Master)

1. **Tools → Board → Raspberry Pi RP2040 Boards → Raspberry Pi Pico**
2. **Tools → Flash Size → 2MB (Sketch: 1MB, FS: 1MB)**
3. **Tools → CPU Speed → 133 MHz**
4. **Tools → USB Stack → Pico SDK**
5. **Tools → Upload Method → Default (UF2)**
6. **Tools → Port → Select your RP2040 port**

## 📂 Arduino Library Locations

Libraries are installed in the standard Arduino libraries folder:

- **macOS**: `~/Documents/Arduino/libraries/`
- **Windows**: `%USERPROFILE%\Documents\Arduino\libraries\`
- **Linux**: `~/Arduino/libraries/`

Board support packages are in:

- **macOS**: `~/Library/Arduino15/packages/`
- **Windows**: `%LOCALAPPDATA%\Arduino15\packages\`
- **Linux**: `~/.arduino15/packages/`

## 🔧 Troubleshooting

### Problem: Script says "Arduino IDE not found"

**Solution:**
- Install Arduino IDE from https://www.arduino.cc/en/software
- macOS: Ensure Arduino.app is in `/Applications/`
- Windows: Default installation path is `C:\Program Files\Arduino\`

### Problem: Script says "Git not found"

**Solution:**
- **macOS**: Install Xcode Command Line Tools: `xcode-select --install`
- **Windows**: Download Git from https://git-scm.com/download/win
- **Linux**: `sudo apt-get install git`

### Problem: Libraries don't show up in Arduino IDE

**Solution:**
1. Verify libraries are in `~/Documents/Arduino/libraries/` (macOS)
2. Restart Arduino IDE completely
3. Check **Sketch → Include Library** to see installed libraries
4. If still missing, re-run installation script

### Problem: Board not found in Tools → Board menu

**Solution:**
1. Complete manual board support installation (Step 4 above)
2. Restart Arduino IDE after installing board support
3. Check **Tools → Board → Boards Manager** to verify installation

### Problem: TFT_eSPI compilation errors

**Solution:**
1. The script backs up original `User_Setup.h` as `User_Setup.h.original`
2. Re-run script to reconfigure: `./install_arduino_deps.sh`
3. Verify configuration in: `~/Documents/Arduino/libraries/TFT_eSPI/User_Setup.h`
4. Should show `#define GC9A01_DRIVER` near the top

### Problem: "Permission denied" on macOS/Linux

**Solution:**
```bash
chmod +x install_arduino_deps.sh
./install_arduino_deps.sh
```

### Problem: Library update fails (git errors)

**Solution:**
```bash
# Remove problematic library and re-run script
rm -rf ~/Documents/Arduino/libraries/LibraryName
./install_arduino_deps.sh
```

## 🔄 Updating Libraries

To update all libraries to latest versions:

**Option 1: Re-run installation script**
```bash
./install_arduino_deps.sh
```
The script detects existing libraries and updates them.

**Option 2: Manual update via Arduino IDE**
1. **Sketch → Include Library → Manage Libraries**
2. Find library to update
3. Click **Update** button

**Option 3: Manual git pull**
```bash
cd ~/Documents/Arduino/libraries/LibraryName
git pull
```

## 🗑️ Uninstallation

To remove installed libraries:

**macOS/Linux:**
```bash
rm -rf ~/Documents/Arduino/libraries/Adafruit_NeoPixel
rm -rf ~/Documents/Arduino/libraries/QRCode
rm -rf ~/Documents/Arduino/libraries/ArduinoJson
rm -rf ~/Documents/Arduino/libraries/TFT_eSPI
rm -rf ~/Documents/Arduino/libraries/FastLED
rm -rf ~/Documents/Arduino/libraries/Seeed_GFX
rm -rf ~/Documents/Arduino/libraries/Seeed_Arduino_RoundDisplay
```

**Windows:**
Delete folders in: `%USERPROFILE%\Documents\Arduino\libraries\`

## 📚 Next Steps After Installation

1. **Read project documentation:**
   - `CLAUDE.md` - Project overview
   - `ESP32_C6_SETUP.md` - ESP32-C6 board setup
   - `Dual_MCU_Integration/README.md` - System architecture
   - `ESP32_C6_Dual_LCD/README.md` - Cat eyes display guide

2. **Choose your project:**
   - **Dual Cat Eyes**: `ESP32_C6_Dual_LCD/ESP32_C6_Dual_LCD.ino`
   - **WiFi Enhanced Badge**: `ESP32_C6_Enhanced/ESP32_C6_Enhanced.ino`
   - **Basic Badge**: `led_ripple.py` (MicroPython)

3. **Upload firmware:**
   - Select appropriate board (see "Select Board" section)
   - Open project `.ino` file
   - Click **Upload** button
   - Follow project-specific README

4. **Connect hardware:**
   - Follow wiring guides in project folders
   - See `WIRING_DUAL_LCD.txt` for LCD connections
   - See `WIRING.txt` for ESP32-Badge connections

## 🆘 Getting Help

If you encounter issues:

1. **Check script output** - Read error messages carefully
2. **Verify prerequisites** - Arduino IDE, Git installed
3. **Check paths** - Ensure project in correct location
4. **Restart Arduino IDE** - After any library changes
5. **Review troubleshooting** - See section above
6. **Check documentation** - Project-specific READMEs
7. **Open GitHub issue** - Include:
   - Your operating system
   - Arduino IDE version
   - Script output (copy/paste errors)
   - Steps you've already tried

## 📄 What the Script Does (Technical Details)

### macOS/Linux Script (`install_arduino_deps.sh`)

1. Checks for Arduino IDE in `/Applications/`
2. Creates `~/Documents/Arduino/libraries/` if missing
3. Modifies `~/Library/Arduino15/preferences.txt` to add board URLs
4. Clones libraries via `git clone` to libraries folder
5. Copies Seeed libraries from project to libraries folder
6. Backs up original `TFT_eSPI/User_Setup.h`
7. Installs custom `User_Setup.h` for GC9A01
8. Creates `FRAMEWORK_BADGE_LIBRARIES.txt` reference file
9. Displays manual installation steps for board support

### Windows Script (`install_arduino_deps.bat`)

Same functionality as macOS script, adapted for Windows paths:
- Uses `%USERPROFILE%\Documents\Arduino\libraries\`
- Uses `%LOCALAPPDATA%\Arduino15\`
- Uses `xcopy` instead of `cp -R`

## 🔐 Security Notes

- Script does **NOT** require administrator/sudo privileges
- Only modifies Arduino-related directories
- Backs up original configurations before changes
- Uses official library repositories (GitHub)
- No network downloads except via git (verifiable)

## 📊 Disk Space Requirements

- **Arduino Libraries**: ~150 MB
- **ESP32 Board Support**: ~300 MB
- **RP2040 Board Support**: ~100 MB
- **Total**: ~550 MB

## ⚡ Performance Tips

- Use SSD for Arduino libraries folder (faster compilation)
- Increase Arduino IDE memory: `arduino.prefs` → `compiler.warning_level=none`
- Close other applications during board support installation
- Use wired Ethernet for faster board support downloads

## 📝 License

This installation script and documentation are provided under the MIT No Attribution License - free to use, modify, and distribute.

---

**Made for Frame:Work NYC 2025**

*Simplifying setup, one script at a time.* ⚙️
