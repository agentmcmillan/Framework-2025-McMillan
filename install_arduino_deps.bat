@echo off
REM Arduino IDE Dependencies Installer for Framework Badge Project
REM Windows Version
REM
REM This script installs all required libraries and board support packages
REM for the Framework Badge 2025 dual-MCU system (RP2040 + ESP32-C6).
REM
REM Usage: install_arduino_deps.bat
REM

setlocal enabledelayedexpansion

REM Project paths
set "PROJECT_DIR=%USERPROFILE%\Documents\GitHub\Framework-2025-McMillan"
set "ARDUINO_DIR=%USERPROFILE%\Documents\Arduino"
set "ARDUINO_LIBRARIES=%ARDUINO_DIR%\libraries"
set "ARDUINO15_DIR=%LOCALAPPDATA%\Arduino15"
set "PREFERENCES_FILE=%ARDUINO15_DIR%\preferences.txt"

echo.
echo ====================================================================
echo    Framework Badge 2025 - Arduino IDE Setup Script
echo    Installing Board Support + Libraries
echo ====================================================================
echo.

REM Check if Arduino IDE is installed
echo [i] Checking for Arduino IDE installation...
if exist "C:\Program Files\Arduino\arduino.exe" (
    echo [OK] Arduino IDE found
) else if exist "C:\Program Files (x86)\Arduino\arduino.exe" (
    echo [OK] Arduino IDE found
) else if exist "%LOCALAPPDATA%\Programs\Arduino IDE\Arduino IDE.exe" (
    echo [OK] Arduino IDE found
) else (
    echo [ERROR] Arduino IDE not found!
    echo.
    echo Please install Arduino IDE first:
    echo   https://www.arduino.cc/en/software
    echo.
    pause
    exit /b 1
)

REM Create Arduino directories
echo [i] Setting up Arduino directories...
if not exist "%ARDUINO_LIBRARIES%" mkdir "%ARDUINO_LIBRARIES%"
if not exist "%ARDUINO15_DIR%" mkdir "%ARDUINO15_DIR%"
echo [OK] Arduino directories ready

REM Check for Git
echo [i] Checking for Git...
where git >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo [WARNING] Git not found!
    echo.
    echo Git is required to download libraries.
    echo Please install Git from: https://git-scm.com/download/win
    echo.
    echo After installing Git, run this script again.
    echo.
    pause
    exit /b 1
)
echo [OK] Git found

REM Configure board manager URLs
echo [i] Configuring board manager URLs...
if not exist "%PREFERENCES_FILE%" (
    echo board.manager.additional.urls=https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json > "%PREFERENCES_FILE%"
    echo [OK] Created preferences file
) else (
    findstr /C:"espressif/arduino-esp32" "%PREFERENCES_FILE%" >nul
    if !ERRORLEVEL! NEQ 0 (
        echo board.manager.additional.urls=https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json >> "%PREFERENCES_FILE%"
        echo [OK] Added ESP32 board manager URL
    ) else (
        echo [OK] ESP32 board manager URL already configured
    )
)

echo.
echo ====================================================================
echo    Installing Arduino Libraries
echo ====================================================================
echo.

REM Install Adafruit NeoPixel
echo [i] Installing Adafruit_NeoPixel...
if exist "%ARDUINO_LIBRARIES%\Adafruit_NeoPixel" (
    cd /d "%ARDUINO_LIBRARIES%\Adafruit_NeoPixel"
    git pull --quiet
    echo [OK] Updated Adafruit_NeoPixel
) else (
    git clone --quiet https://github.com/adafruit/Adafruit_NeoPixel.git "%ARDUINO_LIBRARIES%\Adafruit_NeoPixel"
    echo [OK] Installed Adafruit_NeoPixel
)

REM Install QRCode
echo [i] Installing QRCode...
if exist "%ARDUINO_LIBRARIES%\QRCode" (
    cd /d "%ARDUINO_LIBRARIES%\QRCode"
    git pull --quiet
    echo [OK] Updated QRCode
) else (
    git clone --quiet https://github.com/ricmoo/QRCode.git "%ARDUINO_LIBRARIES%\QRCode"
    echo [OK] Installed QRCode
)

REM Install ArduinoJson
echo [i] Installing ArduinoJson...
if exist "%ARDUINO_LIBRARIES%\ArduinoJson" (
    cd /d "%ARDUINO_LIBRARIES%\ArduinoJson"
    git pull --quiet
    echo [OK] Updated ArduinoJson
) else (
    git clone --quiet https://github.com/bblanchon/ArduinoJson.git "%ARDUINO_LIBRARIES%\ArduinoJson"
    echo [OK] Installed ArduinoJson
)

REM Install TFT_eSPI
echo [i] Installing TFT_eSPI...
if exist "%ARDUINO_LIBRARIES%\TFT_eSPI" (
    cd /d "%ARDUINO_LIBRARIES%\TFT_eSPI"
    git pull --quiet
    echo [OK] Updated TFT_eSPI
) else (
    git clone --quiet https://github.com/Bodmer/TFT_eSPI.git "%ARDUINO_LIBRARIES%\TFT_eSPI"
    echo [OK] Installed TFT_eSPI
)

REM Install FastLED
echo [i] Installing FastLED...
if exist "%ARDUINO_LIBRARIES%\FastLED" (
    cd /d "%ARDUINO_LIBRARIES%\FastLED"
    git pull --quiet
    echo [OK] Updated FastLED
) else (
    git clone --quiet https://github.com/FastLED/FastLED.git "%ARDUINO_LIBRARIES%\FastLED"
    echo [OK] Installed FastLED
)

REM Copy Seeed_GFX from project
echo [i] Installing Seeed_GFX...
if exist "%PROJECT_DIR%\Seeed_GFX-master" (
    if exist "%ARDUINO_LIBRARIES%\Seeed_GFX" rmdir /s /q "%ARDUINO_LIBRARIES%\Seeed_GFX"
    xcopy /E /I /Q "%PROJECT_DIR%\Seeed_GFX-master" "%ARDUINO_LIBRARIES%\Seeed_GFX" >nul
    echo [OK] Installed Seeed_GFX
) else (
    echo [WARNING] Seeed_GFX not found in project directory
)

REM Copy Seeed_Arduino_RoundDisplay from project
echo [i] Installing Seeed_Arduino_RoundDisplay...
if exist "%PROJECT_DIR%\Seeed_Arduino_RoundDisplay-main" (
    if exist "%ARDUINO_LIBRARIES%\Seeed_Arduino_RoundDisplay" rmdir /s /q "%ARDUINO_LIBRARIES%\Seeed_Arduino_RoundDisplay"
    xcopy /E /I /Q "%PROJECT_DIR%\Seeed_Arduino_RoundDisplay-main" "%ARDUINO_LIBRARIES%\Seeed_Arduino_RoundDisplay" >nul
    echo [OK] Installed Seeed_Arduino_RoundDisplay
) else (
    echo [WARNING] Seeed_Arduino_RoundDisplay not found in project directory
)

echo.
echo [OK] All libraries installed
echo.

REM Configure TFT_eSPI
echo [i] Configuring TFT_eSPI for GC9A01...
if exist "%ARDUINO_LIBRARIES%\TFT_eSPI\User_Setup.h" (
    if not exist "%ARDUINO_LIBRARIES%\TFT_eSPI\User_Setup.h.original" (
        copy "%ARDUINO_LIBRARIES%\TFT_eSPI\User_Setup.h" "%ARDUINO_LIBRARIES%\TFT_eSPI\User_Setup.h.original" >nul
        echo [i] Original User_Setup.h backed up
    )

    if exist "%PROJECT_DIR%\Dual_MCU_Integration\ESP32_C6_Dual_LCD\User_Setup.h" (
        copy /Y "%PROJECT_DIR%\Dual_MCU_Integration\ESP32_C6_Dual_LCD\User_Setup.h" "%ARDUINO_LIBRARIES%\TFT_eSPI\User_Setup.h" >nul
        echo [OK] TFT_eSPI configured for GC9A01
    ) else (
        echo [WARNING] Custom User_Setup.h not found
    )
)

REM Create reference file
echo [i] Creating library reference...
(
echo Framework Badge 2025 - Installed Libraries
echo ===========================================
echo.
echo This folder contains libraries for the Framework Badge dual-MCU project.
echo.
echo INSTALLED LIBRARIES:
echo -------------------
echo - Adafruit_NeoPixel      - LED matrix control ^(WS2812B^)
echo - QRCode                 - QR code generation
echo - ArduinoJson            - JSON parsing for web API
echo - TFT_eSPI               - LCD display driver ^(GC9A01^)
echo - FastLED                - Advanced LED control ^(optional^)
echo - Seeed_GFX              - Seeed graphics library
echo - Seeed_Arduino_RoundDisplay - Round display support
echo.
echo Generated: %date% %time%
) > "%ARDUINO_LIBRARIES%\FRAMEWORK_BADGE_LIBRARIES.txt"
echo [OK] Created FRAMEWORK_BADGE_LIBRARIES.txt

echo.
echo ====================================================================
echo    MANUAL STEPS REQUIRED IN ARDUINO IDE
echo ====================================================================
echo.
echo Step 1: Install ESP32 Board Support
echo   - Open Arduino IDE
echo   - Tools -^> Board -^> Boards Manager
echo   - Search 'esp32' -^> Install 'esp32 by Espressif Systems'
echo.
echo Step 2: Install RP2040 Board Support
echo   - File -^> Preferences -^> Additional Board Manager URLs
echo   - Add: https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json
echo   - Tools -^> Board -^> Boards Manager
echo   - Search 'pico' -^> Install 'Raspberry Pi Pico/RP2040'
echo.
echo Step 3: Select Board for Your Project
echo   For ESP32-C6: Tools -^> Board -^> ESP32 Arduino -^> XIAO_ESP32C6
echo   For RP2040: Tools -^> Board -^> Raspberry Pi RP2040 Boards -^> Raspberry Pi Pico
echo.
echo ====================================================================
echo    INSTALLATION COMPLETE!
echo ====================================================================
echo.
echo [OK] Libraries installed in: %ARDUINO_LIBRARIES%
echo [OK] TFT_eSPI configured for GC9A01 displays
echo.
echo Next Steps:
echo   1. Complete manual steps above
echo   2. Restart Arduino IDE
echo   3. Open project files
echo   4. Upload and enjoy!
echo.
echo Documentation:
echo   - %PROJECT_DIR%\ESP32_C6_SETUP.md
echo   - %PROJECT_DIR%\Dual_MCU_Integration\ESP32_C6_Dual_LCD\README.md
echo   - %PROJECT_DIR%\CLAUDE.md
echo.
echo ====================================================================
echo.
pause
