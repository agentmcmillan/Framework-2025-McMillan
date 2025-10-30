#!/bin/bash
#
# Arduino IDE Dependencies Installer for Framework Badge Project
# macOS Version
#
# This script installs all required libraries and board support packages
# for the Framework Badge 2025 dual-MCU system (RP2040 + ESP32-C6).
#
# Usage: ./install_arduino_deps.sh
#

set -e  # Exit on error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Project and Arduino paths
PROJECT_DIR="$HOME/Documents/GitHub/Framework-2025-McMillan"
ARDUINO_DIR="$HOME/Documents/Arduino"
ARDUINO_LIBRARIES="$ARDUINO_DIR/libraries"
ARDUINO15_DIR="$HOME/Library/Arduino15"
BOARD_MANAGER_URL="$ARDUINO15_DIR/preferences.txt"

echo -e "${BLUE}"
echo "╔════════════════════════════════════════════════════════════════╗"
echo "║   Framework Badge 2025 - Arduino IDE Setup Script             ║"
echo "║   Installing Board Support + Libraries                        ║"
echo "╚════════════════════════════════════════════════════════════════╝"
echo -e "${NC}"

# Function to print status messages
print_status() {
    echo -e "${GREEN}[✓]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[!]${NC} $1"
}

print_error() {
    echo -e "${RED}[✗]${NC} $1"
}

print_info() {
    echo -e "${BLUE}[i]${NC} $1"
}

# Check if Arduino IDE is installed
check_arduino_ide() {
    print_info "Checking for Arduino IDE installation..."

    # Check for both Arduino IDE 1.x (Arduino.app) and 2.x (Arduino IDE.app)
    if [ -d "/Applications/Arduino.app" ] || \
       [ -d "/Applications/Arduino IDE.app" ] || \
       [ -d "$HOME/Applications/Arduino.app" ] || \
       [ -d "$HOME/Applications/Arduino IDE.app" ]; then

        # Detect which version
        if [ -d "/Applications/Arduino IDE.app" ] || [ -d "$HOME/Applications/Arduino IDE.app" ]; then
            print_status "Arduino IDE 2.x found"
        else
            print_status "Arduino IDE 1.x found"
        fi
        return 0
    else
        print_error "Arduino IDE not found!"
        echo ""
        echo "Searched locations:"
        echo "  - /Applications/Arduino.app (Arduino IDE 1.x)"
        echo "  - /Applications/Arduino IDE.app (Arduino IDE 2.x)"
        echo "  - $HOME/Applications/Arduino.app"
        echo "  - $HOME/Applications/Arduino IDE.app"
        echo ""
        echo "Please install Arduino IDE first:"
        echo "  https://www.arduino.cc/en/software"
        echo ""
        exit 1
    fi
}

# Create Arduino directories if they don't exist
setup_arduino_directories() {
    print_info "Setting up Arduino directories..."

    mkdir -p "$ARDUINO_LIBRARIES"
    mkdir -p "$ARDUINO15_DIR"

    print_status "Arduino directories ready"
}

# Add board manager URLs
setup_board_manager_urls() {
    print_info "Configuring board manager URLs..."

    # Create preferences.txt if it doesn't exist
    if [ ! -f "$BOARD_MANAGER_URL" ]; then
        cat > "$BOARD_MANAGER_URL" << 'EOF'
# Arduino IDE Preferences
board.manager.additional.urls.windows=
board.manager.additional.urls.linux=
board.manager.additional.urls.macos=
EOF
    fi

    # Check if ESP32 URL already exists
    if ! grep -q "espressif/arduino-esp32" "$BOARD_MANAGER_URL"; then
        print_info "Adding ESP32 board manager URL..."

        # Backup preferences
        cp "$BOARD_MANAGER_URL" "$BOARD_MANAGER_URL.backup"

        # Add or update board manager URLs
        if grep -q "board.manager.additional.urls" "$BOARD_MANAGER_URL"; then
            # Append to existing URLs
            sed -i '' 's|board.manager.additional.urls=.*|board.manager.additional.urls=https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json|g' "$BOARD_MANAGER_URL"
        else
            # Add new line
            echo "board.manager.additional.urls=https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json" >> "$BOARD_MANAGER_URL"
        fi

        print_status "ESP32 board manager URL added"
    else
        print_status "ESP32 board manager URL already configured"
    fi
}

# Install library via git clone
install_library_git() {
    local lib_name=$1
    local git_url=$2
    local lib_dir="$ARDUINO_LIBRARIES/$lib_name"

    print_info "Installing library: $lib_name"

    if [ -d "$lib_dir" ]; then
        print_warning "Library $lib_name already exists, updating..."
        cd "$lib_dir"
        git pull --quiet || print_warning "Could not update $lib_name"
        cd - > /dev/null
    else
        print_info "Cloning $lib_name..."
        git clone --quiet "$git_url" "$lib_dir"
        print_status "Installed $lib_name"
    fi
}

# Install Seeed GFX library (already in repo)
install_seeed_gfx() {
    print_info "Installing Seeed_GFX library..."

    local source_dir="$PROJECT_DIR/Seeed_GFX-master"
    local target_dir="$ARDUINO_LIBRARIES/Seeed_GFX"

    if [ -d "$source_dir" ]; then
        if [ -d "$target_dir" ]; then
            rm -rf "$target_dir"
        fi
        cp -R "$source_dir" "$target_dir"
        print_status "Seeed_GFX library installed"
    else
        print_warning "Seeed_GFX not found in project directory"
    fi
}

# Install Seeed Round Display library (already in repo)
install_seeed_round_display() {
    print_info "Installing Seeed_Arduino_RoundDisplay library..."

    local source_dir="$PROJECT_DIR/Seeed_Arduino_RoundDisplay-main"
    local target_dir="$ARDUINO_LIBRARIES/Seeed_Arduino_RoundDisplay"

    if [ -d "$source_dir" ]; then
        if [ -d "$target_dir" ]; then
            rm -rf "$target_dir"
        fi
        cp -R "$source_dir" "$target_dir"
        print_status "Seeed_Arduino_RoundDisplay library installed"
    else
        print_warning "Seeed_Arduino_RoundDisplay not found in project directory"
    fi
}

# Install required Arduino libraries
install_arduino_libraries() {
    print_info "Installing Arduino libraries..."
    echo ""

    # Adafruit NeoPixel
    install_library_git "Adafruit_NeoPixel" \
        "https://github.com/adafruit/Adafruit_NeoPixel.git"

    # QRCode library for QR code generation
    install_library_git "QRCode" \
        "https://github.com/ricmoo/QRCode.git"

    # ArduinoJson (for ESP32 JSON parsing)
    install_library_git "ArduinoJson" \
        "https://github.com/bblanchon/ArduinoJson.git"

    # TFT_eSPI (for round LCD displays - RP2040 projects)
    install_library_git "TFT_eSPI" \
        "https://github.com/Bodmer/TFT_eSPI.git"

    # Arduino_GFX (for ESP32-C6 LCD displays - better compatibility)
    install_library_git "Arduino_GFX" \
        "https://github.com/moononournation/Arduino_GFX.git"

    # FastLED (optional, for advanced LED control)
    install_library_git "FastLED" \
        "https://github.com/FastLED/FastLED.git"

    # Install Seeed libraries from repo
    install_seeed_gfx
    install_seeed_round_display

    echo ""
    print_status "All libraries installed"
}

# Configure TFT_eSPI for GC9A01 displays
configure_tft_espi() {
    print_info "Configuring TFT_eSPI for GC9A01 displays..."

    local tft_espi_dir="$ARDUINO_LIBRARIES/TFT_eSPI"
    local user_setup="$tft_espi_dir/User_Setup.h"
    local user_setup_backup="$tft_espi_dir/User_Setup.h.original"

    if [ ! -d "$tft_espi_dir" ]; then
        print_warning "TFT_eSPI not installed yet, skipping configuration"
        return
    fi

    # Backup original User_Setup.h if not already backed up
    if [ -f "$user_setup" ] && [ ! -f "$user_setup_backup" ]; then
        cp "$user_setup" "$user_setup_backup"
        print_info "Original User_Setup.h backed up"
    fi

    # Copy our custom User_Setup.h
    local project_user_setup="$PROJECT_DIR/Dual_MCU_Integration/ESP32_C6_Dual_LCD/User_Setup.h"

    if [ -f "$project_user_setup" ]; then
        cp "$project_user_setup" "$user_setup"
        print_status "TFT_eSPI configured for GC9A01 (ESP32-C6)"
        print_info "Original config backed up as: User_Setup.h.original"
    else
        print_warning "Custom User_Setup.h not found in project"
    fi
}

# Download ESP32 board support files
install_esp32_boards() {
    print_info "Installing ESP32 board support..."
    echo ""
    print_info "This requires Arduino IDE Board Manager"
    print_info "Please complete these steps manually in Arduino IDE:"
    echo ""
    echo "  1. Open Arduino IDE"
    echo "  2. Go to Tools → Board → Boards Manager"
    echo "  3. Search for 'esp32'"
    echo "  4. Install 'esp32 by Espressif Systems' (version 3.0.0+)"
    echo "  5. Wait for installation to complete (~5-10 minutes)"
    echo ""
    print_warning "Board support must be installed through Arduino IDE"
    echo ""
}

# Install RP2040 board support
install_rp2040_boards() {
    print_info "Installing RP2040 board support..."
    echo ""
    print_info "RP2040 board support for Raspberry Pi Pico:"
    echo ""
    print_info "Please complete these steps manually in Arduino IDE:"
    echo ""
    echo "  1. Open Arduino IDE"
    echo "  2. Go to File → Preferences"
    echo "  3. Add this URL to 'Additional Board Manager URLs':"
    echo "     https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json"
    echo "  4. Go to Tools → Board → Boards Manager"
    echo "  5. Search for 'pico'"
    echo "  6. Install 'Raspberry Pi Pico/RP2040' by Earle F. Philhower"
    echo ""
    print_warning "Board support must be installed through Arduino IDE"
    echo ""
}

# Create helpful README in Arduino libraries folder
create_library_readme() {
    local readme="$ARDUINO_LIBRARIES/FRAMEWORK_BADGE_LIBRARIES.txt"

    cat > "$readme" << 'EOF'
Framework Badge 2025 - Installed Libraries
===========================================

This folder contains libraries for the Framework Badge dual-MCU project.

INSTALLED LIBRARIES:
-------------------
✓ Adafruit_NeoPixel      - LED matrix control (WS2812B)
✓ QRCode                 - QR code generation
✓ ArduinoJson            - JSON parsing for web API
✓ TFT_eSPI               - LCD display driver (GC9A01) - for RP2040
✓ Arduino_GFX            - LCD display driver - for ESP32-C6
✓ FastLED                - Advanced LED control (optional)
✓ Seeed_GFX              - Seeed graphics library
✓ Seeed_Arduino_RoundDisplay - Round display support

CONFIGURATION:
-------------
- TFT_eSPI has been configured for GC9A01 displays on ESP32-C6
- Original User_Setup.h backed up as User_Setup.h.original

BOARD SUPPORT:
-------------
Required boards (install via Arduino IDE Board Manager):
- ESP32 by Espressif Systems (for ESP32-C6)
- Raspberry Pi Pico/RP2040 by Earle F. Philhower (for RP2040)

PROJECT STRUCTURE:
-----------------
RP2040 (Badge):
- Controls LED matrix, buttons, battery monitoring
- I2C master for ESP32-C6 communication

ESP32-C6 (Coprocessor):
- WiFi 6 access point
- Web server for LED control
- Dual round LCD displays (cat eyes)
- I2C slave

DOCUMENTATION:
-------------
See project folder for detailed documentation:
~/Documents/GitHub/Framework-2025-McMillan/

Questions? Check:
- CLAUDE.md (project overview)
- ESP32_C6_SETUP.md (board setup)
- Dual_MCU_Integration/README.md (system architecture)
- ESP32_C6_Dual_LCD/README.md (cat eyes display)

Generated by: install_arduino_deps.sh
Date: $(date)
EOF

    print_status "Created library reference: FRAMEWORK_BADGE_LIBRARIES.txt"
}

# Print summary of what needs to be done manually
print_manual_steps() {
    echo ""
    echo -e "${BLUE}╔════════════════════════════════════════════════════════════════╗${NC}"
    echo -e "${BLUE}║           MANUAL STEPS REQUIRED IN ARDUINO IDE                 ║${NC}"
    echo -e "${BLUE}╚════════════════════════════════════════════════════════════════╝${NC}"
    echo ""

    print_info "Step 1: Install ESP32 Board Support"
    echo "  → Open Arduino IDE"
    echo "  → Tools → Board → Boards Manager"
    echo "  → Search 'esp32' → Install 'esp32 by Espressif Systems'"
    echo ""

    print_info "Step 2: Install RP2040 Board Support"
    echo "  → File → Preferences → Additional Board Manager URLs"
    echo "  → Add: https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json"
    echo "  → Tools → Board → Boards Manager"
    echo "  → Search 'pico' → Install 'Raspberry Pi Pico/RP2040'"
    echo ""

    print_info "Step 3: Select Board for Your Project"
    echo "  For ESP32-C6 projects:"
    echo "    → Tools → Board → ESP32 Arduino → XIAO_ESP32C6"
    echo ""
    echo "  For RP2040 projects:"
    echo "    → Tools → Board → Raspberry Pi RP2040 Boards → Raspberry Pi Pico"
    echo ""

    print_status "All automatic installation steps completed!"
    echo ""
}

# Print final summary
print_summary() {
    echo ""
    echo -e "${GREEN}╔════════════════════════════════════════════════════════════════╗${NC}"
    echo -e "${GREEN}║              INSTALLATION SUMMARY                              ║${NC}"
    echo -e "${GREEN}╚════════════════════════════════════════════════════════════════╝${NC}"
    echo ""

    print_status "Libraries installed in: $ARDUINO_LIBRARIES"
    print_status "Board manager configured in: $ARDUINO15_DIR"
    print_status "TFT_eSPI configured for GC9A01 displays"
    echo ""

    print_info "Installed Libraries:"
    echo "  - Adafruit_NeoPixel (LED matrix)"
    echo "  - QRCode (QR generation)"
    echo "  - ArduinoJson (JSON parsing)"
    echo "  - TFT_eSPI (LCD displays for RP2040)"
    echo "  - Arduino_GFX (LCD displays for ESP32-C6)"
    echo "  - FastLED (advanced LEDs)"
    echo "  - Seeed_GFX (graphics)"
    echo "  - Seeed_Arduino_RoundDisplay (round LCDs)"
    echo ""

    print_info "Next Steps:"
    echo "  1. Complete manual steps above (board support installation)"
    echo "  2. Restart Arduino IDE to load new libraries"
    echo "  3. Open project files from: $PROJECT_DIR"
    echo "  4. Select appropriate board in Tools → Board"
    echo "  5. Upload firmware and enjoy!"
    echo ""

    print_info "Documentation:"
    echo "  - Setup guide: $PROJECT_DIR/ESP32_C6_SETUP.md"
    echo "  - Dual LCD guide: $PROJECT_DIR/Dual_MCU_Integration/ESP32_C6_Dual_LCD/README.md"
    echo "  - Project overview: $PROJECT_DIR/CLAUDE.md"
    echo ""

    echo -e "${GREEN}╔════════════════════════════════════════════════════════════════╗${NC}"
    echo -e "${GREEN}║                    🎉 SETUP COMPLETE! 🎉                       ║${NC}"
    echo -e "${GREEN}╚════════════════════════════════════════════════════════════════╝${NC}"
    echo ""
}

# Main installation flow
main() {
    echo ""

    # Check prerequisites
    check_arduino_ide

    # Setup directories
    setup_arduino_directories

    # Configure board manager
    setup_board_manager_urls

    # Install libraries
    install_arduino_libraries

    # Configure TFT_eSPI
    configure_tft_espi

    # Create reference documentation
    create_library_readme

    # Print manual steps
    print_manual_steps

    # Print summary
    print_summary
}

# Run main installation
main
