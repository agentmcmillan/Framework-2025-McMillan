# Arduino_GFX Library Fix for ESP32 Core 3.x

## Problem

When compiling `ESP32_C6_Dual_LCD_Adafruit.ino` with ESP32 Arduino Core 3.x, you may encounter these errors:

```
error: operator '||' has no left operand
   #if defined(ESP32) && (CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2 ...
                          ^~
```

## Cause

The Arduino_GFX library uses `CONFIG_IDF_TARGET_*` macros without the `defined()` wrapper. ESP32 Arduino Core 3.x doesn't pre-define these macros as 0/1 - they're only defined when true. This causes the preprocessor to treat undefined macros as having no value, resulting in syntax errors.

## Solution

The Arduino_GFX library has been automatically patched by the installation script (`install_arduino_deps.sh`).

### If You Already Ran the Installation Script

The fix is already applied! The library should compile correctly.

### If You Installed Arduino_GFX Manually

Run the fix script from the project root:

```bash
cd ~/Documents/GitHub/Framework-2025-McMillan
python3 fix_arduino_gfx.py
```

**Output:**
```
Fixing Arduino_GFX library for ESP32 Core 3.x...
Library path: /Users/username/Documents/Arduino/libraries/Arduino_GFX
Fixed: Arduino_ESP32SPI.h
Fixed: Arduino_ESP32SPIDMA.h
...
Done! Fixed 37 files
Backup files saved with .bak extension
```

### Manual Fix (If Script Fails)

If the Python script doesn't work, you can manually edit the files:

**Before:**
```cpp
#if defined(ESP32) && (CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2 || ...)
```

**After:**
```cpp
#if defined(ESP32) && (defined(CONFIG_IDF_TARGET_ESP32) || defined(CONFIG_IDF_TARGET_ESP32S2) || ...)
```

Apply this change to all files in:
- `~/Documents/Arduino/libraries/Arduino_GFX/src/databus/Arduino_ESP32*.h`
- `~/Documents/Arduino/libraries/Arduino_GFX/src/databus/Arduino_ESP32*.cpp`

## What the Fix Does

The `fix_arduino_gfx.py` script automatically adds `defined()` wrappers around all `CONFIG_IDF_TARGET_*` macro checks in the Arduino_GFX library:

- **Line 5 changes:** `(CONFIG_IDF_TARGET_ESP32 || ...` → `(defined(CONFIG_IDF_TARGET_ESP32) || ...`
- **#if changes:** `#if CONFIG_IDF_TARGET_ESP32` → `#if defined(CONFIG_IDF_TARGET_ESP32)`
- **#elif changes:** `#elif CONFIG_IDF_TARGET_ESP32` → `#elif defined(CONFIG_IDF_TARGET_ESP32)`

This makes the library compatible with ESP32 Arduino Core 3.x while maintaining backward compatibility with earlier versions.

## Verification

After applying the fix, verify it worked:

```bash
grep "defined(CONFIG_IDF_TARGET_ESP32)" ~/Documents/Arduino/libraries/Arduino_GFX/src/databus/Arduino_ESP32SPI.h
```

Should return:
```
#if defined(ESP32) && (defined(CONFIG_IDF_TARGET_ESP32) || defined(CONFIG_IDF_TARGET_ESP32S2) ...
```

## Testing Compilation

Try compiling the sketch again:

1. **Restart Arduino IDE** (important - reloads library)
2. **Open** `ESP32_C6_Dual_LCD_Adafruit.ino`
3. **Tools → Board → XIAO_ESP32C6**
4. **Tools → Partition Scheme → Default 4MB with spiffs**
5. **Click Verify** ✓

**Expected:** Compiles successfully with no errors

## Rollback (If Needed)

If the fix causes issues, restore the original files:

```bash
cd ~/Documents/Arduino/libraries/Arduino_GFX/src
find . -name "*.bak" | while read backup; do
    original="${backup%.bak}"
    cp "$backup" "$original"
    echo "Restored: $original"
done
```

## Future Updates

When updating Arduino_GFX library (via `git pull`), you may need to re-apply the fix:

```bash
cd ~/Documents/Arduino/libraries/Arduino_GFX
git pull
cd ~/Documents/GitHub/Framework-2025-McMillan
python3 fix_arduino_gfx.py
```

The installation script (`install_arduino_deps.sh`) automatically applies this fix when installing libraries.

## Technical Details

### Why ESP32 Core 3.x Changed This

ESP32 Arduino Core 3.x uses a different build system (ESP-IDF 5.x) that defines these macros only when true, rather than defining them as 0/1. This is more efficient but breaks libraries that assumed they were always defined.

### Affected ESP32 Variants

All CONFIG_IDF_TARGET_* macros are affected:
- `CONFIG_IDF_TARGET_ESP32` (original ESP32)
- `CONFIG_IDF_TARGET_ESP32S2`
- `CONFIG_IDF_TARGET_ESP32S3`
- `CONFIG_IDF_TARGET_ESP32C2`
- `CONFIG_IDF_TARGET_ESP32C3`
- `CONFIG_IDF_TARGET_ESP32C6` ← Our target
- `CONFIG_IDF_TARGET_ESP32H2`
- `CONFIG_IDF_TARGET_ESP32P4`
- `CONFIG_IDF_TARGET_ESP32C5`

### Files Patched (37 total)

The fix modifies these Arduino_GFX files:
- `src/Arduino_GFX_Library.cpp/h`
- `src/databus/Arduino_ESP32*.cpp/h` (18 files)
- `src/databus/Arduino_ESP32S2*.cpp/h` (8 files)
- `src/Arduino_RGB_Display.cpp/h`
- `src/databus/Arduino_HWSPI.cpp`
- `src/databus/Arduino_SWPAR*.cpp` (2 files)
- `src/databus/Arduino_SWSPI.cpp`

## Status

✅ **Fix Applied:** Arduino_GFX library patched for ESP32 Core 3.x
✅ **Automatic:** Future installations will auto-apply fix
✅ **Tested:** Confirmed working with ESP32-C6 and Core 3.3.2

## Support

If you still get compilation errors after applying the fix:

1. Check ESP32 board support version:
   - Tools → Board → Boards Manager → Search "esp32"
   - Should be version 3.0.0 or higher

2. Verify fix was applied:
   ```bash
   head -10 ~/Documents/Arduino/libraries/Arduino_GFX/src/databus/Arduino_ESP32SPI.h
   ```
   Line 5 should have `defined(CONFIG_IDF_TARGET_ESP32)`

3. Check for backup files (indicates fix ran):
   ```bash
   ls ~/Documents/Arduino/libraries/Arduino_GFX/src/databus/*.bak | wc -l
   ```
   Should show 20+ backup files

4. Try cleaning and recompiling:
   - Arduino IDE → Sketch → Show Sketch Folder
   - Delete `build` folder if it exists
   - Try compiling again

## References

- Arduino_GFX Library: https://github.com/moononournation/Arduino_GFX
- ESP32 Arduino Core 3.x: https://github.com/espressif/arduino-esp32
- Issue Discussion: ESP-IDF 5.x build system changes

---

**Summary:** The Arduino_GFX library needs `defined()` wrappers around CONFIG_IDF_TARGET macros for ESP32 Core 3.x. The fix has been automatically applied by `install_arduino_deps.sh` and `fix_arduino_gfx.py`.
