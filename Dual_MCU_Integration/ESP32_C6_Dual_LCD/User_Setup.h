// User_Setup.h for ESP32-C6 Dual GC9A01 Round Displays
// Place this file in the same folder as ESP32_C6_Dual_LCD.ino
// OR copy to Arduino/libraries/TFT_eSPI/ (overwriting existing)

// Driver selection
#define GC9A01_DRIVER     // GC9A01 240x240 round display

// Display dimensions
#define TFT_WIDTH  240
#define TFT_HEIGHT 240

// Pin definitions for ESP32-C6
// Note: CS, DC, RST are managed manually in code for dual displays
#define TFT_MOSI 21
#define TFT_SCLK 20
#define TFT_CS   2   // Default CS (overridden in code)
#define TFT_DC   3   // Default DC (overridden in code)
#define TFT_RST  4   // Default RST (overridden in code)

// Fonts to be loaded
#define LOAD_GLCD   // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
#define LOAD_FONT2  // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH
#define LOAD_FONT4  // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH
#define LOAD_FONT6  // Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH
#define LOAD_FONT7  // Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH
#define LOAD_FONT8  // Font 8. Large 75 pixel font needs ~3256 bytes in FLASH

// Optional: Load smooth fonts from SPIFFS (not needed for this project)
// #define SMOOTH_FONT

// SPI frequency
#define SPI_FREQUENCY  40000000  // 40MHz - works reliably with GC9A01
#define SPI_READ_FREQUENCY  20000000
#define SPI_TOUCH_FREQUENCY  2500000

// Optional: Enable DMA for faster screen updates (ESP32 only)
// #define DMA_CHANNEL 1  // Uncomment for better performance
