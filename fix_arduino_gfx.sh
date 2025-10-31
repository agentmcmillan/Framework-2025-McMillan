#!/bin/bash
# Fix Arduino_GFX library for ESP32 Arduino Core 3.x compatibility
# Issue: CONFIG_IDF_TARGET_* macros need defined() wrapper

ARDUINO_GFX_DIR="$HOME/Documents/Arduino/libraries/Arduino_GFX"

echo "Fixing Arduino_GFX library for ESP32 Core 3.x..."

# Fix pattern: Change (CONFIG_IDF_TARGET_ESP32 || CONFIG...) to (defined(CONFIG_IDF_TARGET_ESP32) || defined(CONFIG...)|| ...)
find "$ARDUINO_GFX_DIR/src" -name "*.h" -type f -exec sed -i.bak \
  's/CONFIG_IDF_TARGET_ESP32 ||/defined(CONFIG_IDF_TARGET_ESP32) ||/g; \
   s/|| CONFIG_IDF_TARGET_ESP32S2/|| defined(CONFIG_IDF_TARGET_ESP32S2)/g; \
   s/|| CONFIG_IDF_TARGET_ESP32S3/|| defined(CONFIG_IDF_TARGET_ESP32S3)/g; \
   s/|| CONFIG_IDF_TARGET_ESP32C2/|| defined(CONFIG_IDF_TARGET_ESP32C2)/g; \
   s/|| CONFIG_IDF_TARGET_ESP32C3/|| defined(CONFIG_IDF_TARGET_ESP32C3)/g; \
   s/|| CONFIG_IDF_TARGET_ESP32C6/|| defined(CONFIG_IDF_TARGET_ESP32C6)/g; \
   s/|| CONFIG_IDF_TARGET_ESP32H2/|| defined(CONFIG_IDF_TARGET_ESP32H2)/g; \
   s/|| CONFIG_IDF_TARGET_ESP32P4/|| defined(CONFIG_IDF_TARGET_ESP32P4)/g; \
   s/|| CONFIG_IDF_TARGET_ESP32C5/|| defined(CONFIG_IDF_TARGET_ESP32C5)/g; \
   s/#if CONFIG_IDF_TARGET/#if defined(CONFIG_IDF_TARGET/g; \
   s/#elif CONFIG_IDF_TARGET/#elif defined(CONFIG_IDF_TARGET/g; \
   s/#ifndef CONFIG_IDF_TARGET/#ifndef CONFIG_IDF_TARGET/g' {} \;

echo "Done! Arduino_GFX library patched for ESP32 Core 3.x"
echo "Backup files saved with .bak extension"
