# Quick Compile Instructions for ESP32-C6 Dual LCD

## ⚡ Quick Start (5 minutes)

### Step 1: Open the Correct File

Open **`ESP32_C6_Dual_LCD_Adafruit.ino`** in Arduino IDE

❌ **DO NOT open** `ESP32_C6_Dual_LCD.ino` (TFT_eSPI version - won't compile on C6)

---

### Step 2: Select Board Settings

1. **Tools → Board → ESP32 Arduino → XIAO_ESP32C6**

2. Wait 2-3 seconds for board to load

3. **Tools → Partition Scheme → Default 4MB with spiffs (1.2MB APP/1.5MB SPIFFS)**

4. **Other settings (verify these):**
   - USB CDC On Boot: **Enabled**
   - CPU Frequency: **160MHz (WiFi)**
   - Flash Mode: **QIO 80MHz**
   - Flash Size: **4MB**
   - Upload Speed: **921600**

---

### Step 3: Verify Libraries

Arduino_GFX library should already be installed by the setup script.

**To verify:**
- **Sketch → Include Library**
- Look for **"GFX Library for Arduino"** in the list

**If missing:**
```bash
cd ~/Documents/Arduino/libraries
git clone https://github.com/moononournation/Arduino_GFX.git
```
Then restart Arduino IDE.

---

### Step 4: Compile

Click **✓ Verify** button (or Ctrl+R / Cmd+R)

**Expected output:**
```
Sketch uses XXXXX bytes (XX%) of program storage space.
Global variables use XXXXX bytes (XX%) of dynamic memory.
```

**If successful:** You'll see "Done compiling" in green

---

### Step 5: Upload

1. **Connect ESP32-C6** via USB-C cable

2. **Tools → Port** → Select `/dev/cu.usbmodem*` (macOS) or `COM*` (Windows)

3. Click **→ Upload** button (or Ctrl+U / Cmd+U)

**Expected output:**
```
Writing at 0x00010000... (XX %)
Writing at 0x00020000... (XX %)
...
Hard resetting via RTS pin...
```

**If successful:** Board will reset and start running

---

## 🔌 Wiring

See `WIRING_DUAL_LCD.txt` for complete wiring diagram.

**Quick reference:**

### Display 1 (Left Eye):
- VCC → 3.3V
- GND → GND
- SCK → GPIO20
- MOSI → GPIO21
- CS → GPIO2
- DC → GPIO3
- RST → GPIO4
- BL → 3.3V

### Display 2 (Right Eye):
- VCC → 3.3V
- GND → GND
- SCK → GPIO20 (shared)
- MOSI → GPIO21 (shared)
- CS → GPIO5
- DC → GPIO6
- RST → GPIO7
- BL → 3.3V

---

## 🌐 Testing

After upload:

1. **Open Serial Monitor** (Tools → Serial Monitor)
   - Set baud rate to **115200**

2. **Look for startup messages:**
   ```
   === ESP32-C6 Dual Cat Eyes ===
   Initializing displays...
   Display 1 initialized
   Display 2 initialized
   Starting WiFi AP...
   AP Started: CatEyes-XXXX
   IP: 192.168.4.1
   ```

3. **Connect to WiFi:**
   - SSID: **CatEyes-XXXX** (XXXX = last 4 digits of MAC)
   - Password: **meow1234**

4. **Open browser:**
   - Navigate to: **http://192.168.4.1**

5. **Test controls:**
   - Click direction arrows to move eyes
   - Change color from dropdown
   - Click "Blink" for manual blink
   - Watch automatic blinking every 3-7 seconds

---

## ❌ Common Errors

### Error: "Arduino_GFX.h: No such file"

**Solution:** Install Arduino_GFX library (see Step 3 above)

---

### Error: "{build.partitions}.csv: No such file"

**Solution:** Select partition scheme explicitly (see Step 2, item 3)

**Full fix:**
1. Restart Arduino IDE completely (Cmd+Q)
2. Select board: XIAO_ESP32C6
3. Wait 3 seconds
4. Select partition: Default 4MB with spiffs
5. Try compiling again

---

### Error: "A fatal error occurred: Failed to connect"

**Solution:** Put ESP32-C6 in bootloader mode

1. Hold **BOOT** button
2. While holding BOOT, press **RESET** button
3. Release both buttons
4. Try upload again

---

### Error: "Port not found"

**Solution:** Check USB connection

**macOS:**
```bash
ls /dev/cu.usbmodem*
```

**Windows:** Check Device Manager → Ports (COM & LPT)

**If no port appears:**
- Try different USB cable (must support data, not just charging)
- Try different USB port
- Check if board shows up in system (you may see "USB JTAG/serial debug unit")

---

### Displays show nothing or garbage

**Check:**
1. **Power**: Displays must have 3.3V and GND connected
2. **Backlight**: BL pin must be connected to 3.3V
3. **Wiring**: Double-check all pin connections match code
4. **Orientation**: Try rotating display 180° if mounted upside-down

**Serial Monitor should show:**
```
Display 1 initialized
Display 2 initialized
```

If it shows initialization errors, check wiring.

---

## 📊 Memory Usage

**Typical sketch size:** 600-700 KB
**RAM usage:** 50-80 KB

If you get "Sketch too big" error:
- Select larger partition: **No OTA (2MB APP/2MB SPIFFS)**

---

## 🔧 Advanced: Adjusting Animation

### Change eye colors (in code):

```cpp
// Line ~60
#define DEFAULT_EYE_COLOR 0x07E0  // Green (default)
// Other options:
// 0x001F - Blue
// 0xF800 - Red
// 0xFFE0 - Yellow
// 0xF81F - Magenta
// 0x07FF - Cyan
```

### Change blink frequency:

```cpp
// Line ~66
#define BLINK_INTERVAL_MIN 3000  // Minimum 3 seconds
#define BLINK_INTERVAL_MAX 7000  // Maximum 7 seconds
```

### Change animation speed:

```cpp
// Line ~68
#define ANIMATION_STEPS 10  // More = smoother but slower
```

### Change WiFi credentials:

```cpp
// Line ~72
const char* ap_ssid_prefix = "CatEyes";
const char* ap_password = "meow1234";  // Change this
```

---

## 📚 Documentation

- **Full project guide:** `README.md`
- **Wiring diagram:** `WIRING_DUAL_LCD.txt`
- **Troubleshooting:** `TROUBLESHOOTING.md`
- **Library compatibility:** `LIBRARY_COMPATIBILITY.md`
- **Web API reference:** See README.md → API Endpoints

---

## ✅ Success Checklist

- [ ] Arduino_GFX library installed
- [ ] Board set to XIAO_ESP32C6
- [ ] Partition scheme selected (Default 4MB with spiffs)
- [ ] Compiled without errors
- [ ] Uploaded successfully
- [ ] Serial monitor shows "Display 1/2 initialized"
- [ ] WiFi AP "CatEyes-XXXX" is visible
- [ ] Can connect to WiFi with password "meow1234"
- [ ] Web interface loads at 192.168.4.1
- [ ] Both displays show cat eyes
- [ ] Eyes move when clicking direction buttons
- [ ] Eyes blink automatically

---

**If all checklist items pass: You're ready to go! 🎉**

**If any fail: See TROUBLESHOOTING.md for detailed solutions**
