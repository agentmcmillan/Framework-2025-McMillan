/*
 * Framework-2025-Badge LED Matrix + TFT Display Demo
 *
 * Combines WS2812B LED matrix (15x7) with Seeed Round Display (GC9A01)
 * via Whisker expansion port.
 *
 * Hardware:
 * - RP2040 microcontroller
 * - 105 WS2812B LEDs on GPIO 4
 * - 3 buttons on GPIO 5, 6, 7
 * - GC9A01 240x240 round TFT on Whisker port (SPI)
 * - Battery voltage monitor on GPIO 26
 *
 * Features:
 * - LED matrix rainbow ripple animation (triggered by center button)
 * - TFT display shows: battery level, button states, system info
 * - Real-time status updates on round display
 *
 * Required Libraries:
 * - Adafruit_NeoPixel (for LED matrix)
 * - Seeed_GFX (TFT_eSPI fork, included in repo)
 */

#include <Adafruit_NeoPixel.h>
#include <TFT_eSPI.h>  // Seeed_GFX library

// ============================================================================
// PIN DEFINITIONS - Framework Badge
// ============================================================================

// LED Matrix
#define LED_PIN         4      // WS2812B data line
#define NUM_LEDS        105    // 15 columns × 7 rows
#define LED_COLS        15
#define LED_ROWS        7

// Buttons (active low with pull-up)
#define BTN_LEFT        5      // GPIO 5
#define BTN_CENTER      6      // GPIO 6 - "MEOW" button
#define BTN_RIGHT       7      // GPIO 7

// Battery Monitor
#define BATTERY_PIN     26     // ADC0, voltage divider (0.357x scaling)

// Status LED
#define STATUS_LED      11     // Red LED

// TFT Display on Whisker Port
// Note: Configure these in User_Setup.h or create Setup file
#define TFT_MOSI        23     // Whisker port IO#23 (can also use IO#24)
#define TFT_SCLK        24     // Whisker port IO#24 (can also use IO#25)
#define TFT_CS          25     // Whisker port IO#25
#define TFT_DC          20     // Available GPIO (not on whisker, use nearby pin)
#define TFT_RST         21     // Available GPIO (not on whisker, use nearby pin)
// TFT_BL (backlight) can connect to +5V or +3.3V on Whisker port

// ============================================================================
// HARDWARE OBJECTS
// ============================================================================

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
TFT_eSPI tft = TFT_eSPI();  // Uses pins defined in User_Setup.h

// ============================================================================
// CONSTANTS & GLOBALS
// ============================================================================

// Safety: Maximum brightness for LED matrix (15% to prevent over-current)
#define MAX_LED_BRIGHTNESS 0.15

// Colors for TFT (RGB565 format)
#define TFT_BG          0x0000  // Black background
#define TFT_TITLE       0xFFFF  // White text
#define TFT_ACCENT      0x07FF  // Cyan accent
#define TFT_GOOD        0x07E0  // Green (good battery)
#define TFT_WARN        0xFFE0  // Yellow (medium battery)
#define TFT_BAD         0xF800  // Red (low battery)

// State tracking
bool btnLeftState = false;
bool btnCenterState = false;
bool btnRightState = false;
float batteryVoltage = 0.0;
unsigned long lastUpdate = 0;
unsigned long lastRipple = 0;

// ============================================================================
// LED MATRIX HELPER FUNCTIONS
// ============================================================================

// Convert x,y grid coordinates to LED index
// LED matrix addressing: by columns, top to bottom, left to right
int xyToIndex(int x, int y) {
  if (x < 0 || x >= LED_COLS || y < 0 || y >= LED_ROWS) {
    return -1;
  }
  return x * LED_ROWS + y;
}

// Calculate distance between two points
float distance(float x1, float y1, float x2, float y2) {
  return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

// HSV to RGB conversion
uint32_t hsvToRgb(float h, float s, float v) {
  float c = v * s;
  float x = c * (1.0 - fabs(fmod(h / 60.0, 2.0) - 1.0));
  float m = v - c;
  float r, g, b;

  if (h < 60) {
    r = c; g = x; b = 0;
  } else if (h < 120) {
    r = x; g = c; b = 0;
  } else if (h < 180) {
    r = 0; g = c; b = x;
  } else if (h < 240) {
    r = 0; g = x; b = c;
  } else if (h < 300) {
    r = x; g = 0; b = c;
  } else {
    r = c; g = 0; b = x;
  }

  return strip.Color(
    (uint8_t)((r + m) * 255),
    (uint8_t)((g + m) * 255),
    (uint8_t)((b + m) * 255)
  );
}

// ============================================================================
// LED ANIMATIONS
// ============================================================================

void rainbowRipple() {
  const float centerX = LED_COLS / 2.0;
  const float centerY = LED_ROWS / 2.0;
  const unsigned long duration = 2500; // 2.5 seconds
  unsigned long startTime = millis();

  while (millis() - startTime < duration) {
    float elapsed = (millis() - startTime) / 1000.0;
    float wavePos = elapsed * 5.0;

    for (int x = 0; x < LED_COLS; x++) {
      for (int y = 0; y < LED_ROWS; y++) {
        float dist = distance(centerX, centerY, x, y);
        float waveOffset = fabs(dist - wavePos);

        if (waveOffset < 2.0) {
          // Main wave front
          float intensity = (2.0 - waveOffset) / 2.0;
          float hue = fmod(dist * 40.0 + elapsed * 60.0, 360.0);

          uint32_t color = hsvToRgb(hue, 1.0, intensity * MAX_LED_BRIGHTNESS);
          int index = xyToIndex(x, y);
          if (index >= 0) {
            strip.setPixelColor(index, color);
          }
        } else if (dist < wavePos) {
          // Trailing fade
          float fadeIntensity = max(0.0, 1.0 - (wavePos - dist) / 5.0);
          float hue = fmod(dist * 40.0, 360.0);

          uint32_t color = hsvToRgb(hue, 0.8, fadeIntensity * MAX_LED_BRIGHTNESS * 0.3);
          int index = xyToIndex(x, y);
          if (index >= 0) {
            strip.setPixelColor(index, color);
          }
        } else {
          // Not yet reached
          int index = xyToIndex(x, y);
          if (index >= 0) {
            strip.setPixelColor(index, 0);
          }
        }
      }
    }

    strip.show();
    delay(40);  // ~25 FPS
  }

  // Clear after animation
  strip.clear();
  strip.show();
}

void idleAnimation() {
  // Gentle breathing effect on center pixel
  int centerIndex = xyToIndex(LED_COLS / 2, LED_ROWS / 2);

  static float phase = 0;
  phase += 0.1;

  float intensity = (sin(phase) + 1.0) / 2.0;  // 0 to 1
  uint32_t color = hsvToRgb(200, 0.7, intensity * MAX_LED_BRIGHTNESS * 0.5);

  if (centerIndex >= 0) {
    strip.setPixelColor(centerIndex, color);
  }

  strip.show();
}

// ============================================================================
// BATTERY MONITORING
// ============================================================================

float readBatteryVoltage() {
  // Read ADC (0-4095 for 0-3.3V)
  int adcValue = analogRead(BATTERY_PIN);

  // Convert to actual voltage
  // ADC reading represents voltage after 0.357x divider
  // Actual battery voltage = ADC_voltage / 0.357
  float adcVoltage = (adcValue / 4095.0) * 3.3;
  float batteryV = adcVoltage / 0.357;

  return batteryV;
}

// ============================================================================
// TFT DISPLAY FUNCTIONS
// ============================================================================

void initTFT() {
  tft.init();
  tft.setRotation(0);  // 0-3 depending on your mounting
  tft.fillScreen(TFT_BG);

  // Draw initial UI
  drawStaticUI();
}

void drawStaticUI() {
  tft.fillScreen(TFT_BG);

  // Title
  tft.setTextColor(TFT_TITLE, TFT_BG);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("PIXEL KITTY", 120, 20, 4);

  // Draw separators
  tft.drawFastHLine(20, 45, 200, TFT_ACCENT);
  tft.drawFastHLine(20, 195, 200, TFT_ACCENT);

  // Labels
  tft.setTextColor(TFT_ACCENT, TFT_BG);
  tft.setTextDatum(ML_DATUM);
  tft.drawString("Battery:", 30, 70, 2);
  tft.drawString("Buttons:", 30, 110, 2);
  tft.drawString("Left:", 50, 135, 2);
  tft.drawString("Center:", 50, 155, 2);
  tft.drawString("Right:", 50, 175, 2);

  // Footer
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_ACCENT, TFT_BG);
  tft.drawString("Press CENTER for ripple!", 120, 215, 2);
}

void updateTFTStatus() {
  // Update battery voltage display
  tft.setTextDatum(ML_DATUM);

  // Determine battery color based on voltage
  uint16_t battColor;
  if (batteryVoltage > 2.7) {
    battColor = TFT_GOOD;  // > 2.7V = good
  } else if (batteryVoltage > 2.4) {
    battColor = TFT_WARN;  // 2.4-2.7V = medium
  } else {
    battColor = TFT_BAD;   // < 2.4V = low
  }

  // Clear previous value
  tft.fillRect(130, 60, 80, 20, TFT_BG);

  // Draw new value
  tft.setTextColor(battColor, TFT_BG);
  char battStr[20];
  sprintf(battStr, "%.2fV", batteryVoltage);
  tft.drawString(battStr, 130, 70, 2);

  // Update button states
  tft.setTextDatum(MR_DATUM);

  // Left button
  tft.fillCircle(210, 135, 10, btnLeftState ? TFT_GOOD : TFT_BAD);

  // Center button
  tft.fillCircle(210, 155, 10, btnCenterState ? TFT_GOOD : TFT_BAD);

  // Right button
  tft.fillCircle(210, 175, 10, btnRightState ? TFT_GOOD : TFT_BAD);
}

void drawRippleIndicator() {
  // Visual feedback on TFT when ripple is triggered
  tft.fillCircle(120, 120, 30, TFT_ACCENT);
  delay(100);
  tft.fillCircle(120, 120, 30, TFT_BG);
}

// ============================================================================
// SETUP
// ============================================================================

void setup() {
  Serial.begin(115200);
  Serial.println("Framework Badge - LED Matrix + TFT Demo");

  // Initialize pins
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_CENTER, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);
  pinMode(STATUS_LED, OUTPUT);
  pinMode(BATTERY_PIN, INPUT);

  // Initialize LED strip
  strip.begin();
  strip.setBrightness(255);  // Max brightness, we control via color values
  strip.clear();
  strip.show();

  // Initialize TFT display
  Serial.println("Initializing TFT display...");
  initTFT();

  // Initial battery reading
  batteryVoltage = readBatteryVoltage();
  updateTFTStatus();

  Serial.println("Setup complete!");
  Serial.println("Press CENTER button for rainbow ripple!");
}

// ============================================================================
// MAIN LOOP
// ============================================================================

void loop() {
  unsigned long currentTime = millis();

  // Read button states (active low)
  btnLeftState = !digitalRead(BTN_LEFT);
  btnCenterState = !digitalRead(BTN_CENTER);
  btnRightState = !digitalRead(BTN_RIGHT);

  // Check for center button press (rainbow ripple trigger)
  if (btnCenterState && (currentTime - lastRipple > 500)) {
    Serial.println("MEOW! Rainbow ripple triggered!");
    lastRipple = currentTime;

    // Visual feedback on TFT
    drawRippleIndicator();

    // Run rainbow ripple animation
    rainbowRipple();

    // Wait for button release
    while (!digitalRead(BTN_CENTER) == LOW) {
      delay(10);
    }
  } else {
    // Idle animation when not triggering ripple
    idleAnimation();
  }

  // Update TFT status display periodically (every 500ms)
  if (currentTime - lastUpdate > 500) {
    lastUpdate = currentTime;

    // Read battery voltage (average over multiple readings)
    batteryVoltage = 0;
    for (int i = 0; i < 10; i++) {
      batteryVoltage += readBatteryVoltage();
      delay(1);
    }
    batteryVoltage /= 10.0;

    // Update TFT display
    updateTFTStatus();
  }

  delay(20);  // ~50 FPS update rate
}
