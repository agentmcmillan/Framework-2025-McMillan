# ESP32-C6 Dual LCD - Troubleshooting Guide

## Compilation Error: "{build.partitions}.csv: No such file or directory"

### Problem
When compiling, you get this error:
```
/Users/username/Library/Arduino15/packages/esp32/hardware/esp32/3.3.2/tools/partitions/{build.partitions}.csv: No such file or directory
```

This means the partition scheme variable isn't being set properly by Arduino IDE.

### Solution 1: Explicitly Select Partition Scheme (Recommended)

1. **Restart Arduino IDE completely** (Cmd+Q, then reopen)

2. **Select the board:**
   - **Tools → Board → ESP32 Arduino → XIAO_ESP32C6**

3. **Wait 2-3 seconds** for Arduino IDE to load board options

4. **Select partition scheme:**
   - **Tools → Partition Scheme → Default 4MB with spiffs (1.2MB APP/1.5MB SPIFFS)**

5. **Verify other settings:**
   - **Tools → USB CDC On Boot → Enabled**
   - **Tools → CPU Frequency → 160MHz (WiFi)**
   - **Tools → Flash Mode → QIO 80MHz**
   - **Tools → Flash Size → 4MB**

6. **Try compiling again**

### Solution 2: Clear Arduino IDE Cache

If Solution 1 doesn't work:

1. **Close Arduino IDE**

2. **Delete build cache:**
   ```bash
   rm -rf ~/Library/Arduino15/packages/esp32/hardware/esp32/3.3.2/tools/sdk/esp32c6/
   rm -rf /var/folders/*/T/arduino_*
   ```

3. **Reopen Arduino IDE**

4. **Select board and partition scheme again** (see Solution 1)

### Solution 3: Use Arduino CLI (Alternative)

If Arduino IDE GUI continues to have issues:

1. **Install Arduino CLI:**
   ```bash
   brew install arduino-cli
   ```

2. **Compile from command line:**
   ```bash
   arduino-cli compile --fqbn esp32:esp32:XIAO_ESP32C6:PartitionScheme=default \
     ESP32_C6_Dual_LCD.ino
   ```

3. **Upload:**
   ```bash
   arduino-cli upload -p /dev/cu.usbmodem* --fqbn esp32:esp32:XIAO_ESP32C6:PartitionScheme=default \
     ESP32_C6_Dual_LCD.ino
   ```

### Solution 4: Check Board Installation

Verify ESP32 board support is properly installed:

1. **Open Arduino IDE**

2. **Tools → Board → Boards Manager**

3. **Search:** "esp32"

4. **Verify:** "esp32 by Espressif Systems" version 3.0.0 or higher is installed

5. If not, **install or update** the board package

6. **Restart Arduino IDE**

## Other Common Errors

### Error: TFT_eSPI compilation errors on ESP32-C6

**Symptoms:**
```
error: 'VSPI' was not declared in this scope
error: 'SPI_MOSI_DLEN_REG' was not declared in this scope
error: no match for 'operator=' (operand types are 'volatile gpio_out_w1tc_reg_t' and 'int')
```

**Cause:**
TFT_eSPI library uses ESP32/ESP32-S3 specific hardware registers that don't exist on ESP32-C6 (RISC-V architecture).

**Solution:**
Use the Arduino_GFX version instead:

1. **Open the Adafruit version:**
   - `ESP32_C6_Dual_LCD/ESP32_C6_Dual_LCD_Adafruit.ino`

2. **Arduino_GFX library should already be installed** by the installation script
   - If not: Library Manager → Search "GFX Library for Arduino" → Install

3. **Compile and upload** - This version has full ESP32-C6 compatibility

**Note:** The original `ESP32_C6_Dual_LCD.ino` (TFT_eSPI version) is kept for reference but won't compile on ESP32-C6.

### Error: "TFT_eSPI.h: No such file"

**Solution:**
- Run the installation script: `./install_arduino_deps.sh`
- Or manually install TFT_eSPI from Library Manager

### Error: "WiFi.h: No such file"

**Solution:**
- This means ESP32 board support isn't installed
- Install via **Tools → Board → Boards Manager → esp32**

### Error: Sketch too big

**Solution:**
- Change partition scheme to larger APP partition:
  - **Tools → Partition Scheme → No OTA (2MB APP/2MB SPIFFS)**

### Error: Port not found

**Solution:**
1. Check USB cable supports data (not charge-only)
2. Press BOOT button while connecting USB
3. Check **Tools → Port** for new device
4. On macOS: `ls /dev/cu.usbmodem*`
5. Install USB drivers if needed (usually automatic on macOS)

### Warning: "Could not update Adafruit_NeoPixel"

**This is normal** - Library was manually installed. Safe to ignore.

## Board Settings Reference

### Recommended Settings for ESP32-C6

| Setting | Value |
|---------|-------|
| Board | XIAO_ESP32C6 |
| USB CDC On Boot | Enabled |
| CPU Frequency | 160MHz (WiFi) |
| Flash Mode | QIO 80MHz |
| Flash Size | 4MB |
| **Partition Scheme** | **Default 4MB with spiffs** ⭐ |
| Upload Speed | 921600 |
| Core Debug Level | None (or Info for debugging) |

### Alternative Partition Schemes

If you need more program space:

- **No OTA (2MB APP/2MB SPIFFS)** - For larger sketches
- **No OTA (1MB APP/3MB SPIFFS)** - For projects with lots of data files

## Verification Steps

After fixing the partition scheme issue:

1. ✅ Code compiles without errors
2. ✅ Sketch size shown (e.g., "Sketch uses 234567 bytes")
3. ✅ Upload succeeds
4. ✅ Serial Monitor shows startup messages
5. ✅ Displays initialize (if connected)
6. ✅ WiFi AP starts (SSID: CatEyes-XXXX)

## Still Having Issues?

1. **Check Serial Monitor output** (Tools → Serial Monitor, 115200 baud)
2. **Enable verbose compilation:**
   - File → Preferences → Show verbose output during: ✅ compilation
3. **Copy full error message** and check against this guide
4. **Try a simple ESP32-C6 example first:**
   - File → Examples → WiFi → WiFiScan
   - If this doesn't compile, board support has issues

## Getting Help

If none of these solutions work:

1. Note your Arduino IDE version (Help → About)
2. Note ESP32 board package version (Boards Manager)
3. Copy complete error message
4. Check Arduino IDE issues: https://github.com/espressif/arduino-esp32/issues
5. Open issue with full details

---

**Quick Fix Checklist:**
- [ ] Restart Arduino IDE
- [ ] Select XIAO_ESP32C6 board
- [ ] Select "Default 4MB with spiffs" partition
- [ ] Verify all Tools menu settings
- [ ] Try compiling again
