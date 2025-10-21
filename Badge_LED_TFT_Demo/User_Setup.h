// User_Setup.h for Framework-2025-Badge with GC9A01 Round Display
// Place this file in your Seeed_GFX library folder or configure via platformio.ini

#define USER_SETUP_ID 2025

// ============================================================================
// DISPLAY DRIVER - GC9A01 Round Display (240x240)
// ============================================================================
#define GC9A01_DRIVER

// ============================================================================
// DISPLAY DIMENSIONS
// ============================================================================
#define TFT_WIDTH  240
#define TFT_HEIGHT 240

// ============================================================================
// PIN CONFIGURATION - Framework Badge + Whisker Port
// ============================================================================

// IMPORTANT: The RP2040 SPI pins are flexible via PIO, but we use these
// specific pins to match the Whisker port connections

#define TFT_MOSI 23    // Whisker port IO#23 (SPI MOSI)
#define TFT_SCLK 24    // Whisker port IO#24 (SPI SCK)
#define TFT_CS   25    // Whisker port IO#25 (Chip Select)
#define TFT_DC   20    // Data/Command (use available GPIO, not on whisker)
#define TFT_RST  21    // Reset (use available GPIO, not on whisker)

// Backlight control (optional)
// Connect TFT backlight to +3.3V or +5V on Whisker port for always-on
// Or define a GPIO pin for PWM brightness control:
// #define TFT_BL   22  // Backlight control pin

// ============================================================================
// FONT CONFIGURATION
// ============================================================================

// Standard fonts
#define LOAD_GLCD   // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
#define LOAD_FONT2  // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
#define LOAD_FONT4  // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
#define LOAD_FONT6  // Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm
#define LOAD_FONT7  // Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH, only characters 1234567890:.
#define LOAD_FONT8  // Font 8. Large 75 pixel font needs ~3256 bytes in FLASH, only characters 1234567890:-.
#define LOAD_GFXFF  // FreeFonts. Include access to the 48 Adafruit_GFX free fonts FF1 to FF48

// Smooth fonts
#define SMOOTH_FONT

// ============================================================================
// SPI CONFIGURATION
// ============================================================================

// SPI frequency for writing to TFT
#define SPI_FREQUENCY  27000000   // 27MHz - safe for RP2040
// #define SPI_FREQUENCY  40000000   // 40MHz - try if 27MHz works well

// Optional reduced SPI frequency for reading TFT (not typically used)
#define SPI_READ_FREQUENCY  5000000

// RP2040 specific settings
#define RP2040_PIO_SPI  // Use PIO for SPI on RP2040 (better performance)

// ============================================================================
// COLOR DEPTH
// ============================================================================

// 16-bit color (RGB565) - standard for most TFT displays
// This is the default and most efficient option
// #define TFT_RGB_ORDER TFT_BGR  // Uncomment if colors are inverted

// ============================================================================
// OTHER OPTIONS
// ============================================================================

// Define to enable support for smooth fonts
#define SMOOTH_FONT

// Enable if you want to use DMA (RP2040 supports this)
// #define RP2040_DMA

// ============================================================================
// NOTES FOR FRAMEWORK BADGE
// ============================================================================
/*
 * Wiring Guide - Whisker Port to GC9A01:
 *
 * Whisker Port (1x5 header on bottom of badge):
 * Pin 1: +5V        -> Connect to VCC or LED+ on display
 * Pin 2: +3.3V      -> Connect to VCC on display (if 3.3V only)
 * Pin 3: GND        -> Connect to GND on display
 * Pin 4: IO#23      -> Connect to SDA/MOSI on display
 * Pin 5: IO#24      -> Connect to SCL/SCK on display
 *
 * Additional connections needed (use nearby GPIOs or solder to RP2040 pads):
 * IO#25 -> CS (Chip Select)
 * IO#20 -> DC (Data/Command)
 * IO#21 -> RST (Reset) - or connect to 3.3V with 10K resistor
 *
 * Display Backlight:
 * - If always on: connect BL/LED to +3.3V or +5V
 * - If PWM control: connect to TFT_BL GPIO (requires additional wire)
 *
 * Current Consumption:
 * - TFT display: ~20-40mA typical, ~100mA max (white screen, full brightness)
 * - Total with LED matrix: Monitor battery voltage (IO#26) to track consumption
 * - Consider using lower brightness on TFT if running on batteries
 *
 * Alternative Pin Mappings:
 * If IO#23-25 are needed for other purposes, you can use:
 * - Any available GPIO for SPI (RP2040 has flexible PIO SPI)
 * - Update TFT_MOSI, TFT_SCLK, TFT_CS definitions above
 * - Common alternatives: IO#16-19 (if not using DVI port)
 */

// ============================================================================
// COMPATIBILITY CHECK
// ============================================================================

#ifndef RP2040
  #warning "This setup is configured for RP2040. Other platforms may need adjustments."
#endif
