/*
 * Framework-2025-Badge LED Ripple Effect
 * Arduino sketch for creating a rainbow ripple animation on the 15x7 LED matrix
 *
 * This sketch runs continuously on the RP2040, displaying a breathing idle animation
 * and triggering a rainbow ripple when the center "MEOW" button is pressed.
 *
 * IMPORTANT: LED matrix power management is critical!
 * - Never use full brightness white (hardware damage risk)
 * - This sketch uses low brightness (15% max) for safety
 * - Limits number of lit pixels to prevent over-current
 *
 * Hardware:
 * - RP2040 microcontroller (Raspberry Pi Pico or compatible)
 * - 15x7 WS2812B-2020 LED matrix (105 LEDs)
 * - Three buttons on GPIO 5, 6, 7 (active low with pull-ups)
 *
 * Required Library:
 * - Adafruit_NeoPixel (install via Arduino Library Manager)
 */

#include <Adafruit_NeoPixel.h>

// LED Matrix Configuration
#define LED_PIN        4      // GPIO pin for LED matrix data line
#define NUM_LEDS       105    // 15 columns × 7 rows
#define COLS           15
#define ROWS           7
#define MAX_BRIGHTNESS 0.15   // Safety limit: 15% maximum brightness

// Button Configuration
#define BUTTON_LEFT    5      // GPIO #5
#define BUTTON_CENTER  6      // GPIO #6 - "MEOW" button
#define BUTTON_RIGHT   7      // GPIO #7

// Animation Timing
#define RIPPLE_DURATION  2500  // Ripple duration in milliseconds
#define RIPPLE_FPS       25    // Frames per second for ripple animation
#define IDLE_FPS         20    // Frames per second for idle animation
#define DEBOUNCE_DELAY   300   // Button debounce delay in milliseconds

// Initialize NeoPixel strip
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// Debounce variables
unsigned long lastPressTime = 0;

// Animation state
float idlePhase = 0.0;

/*
 * Convert x,y coordinates to LED index
 * LED matrix is addressed by columns (top to bottom, left to right)
 * Column 0: LEDs 0-6, Column 1: LEDs 7-13, etc.
 */
int xyToIndex(int x, int y) {
  if (x < 0 || x >= COLS || y < 0 || y >= ROWS) {
    return -1;  // Invalid position
  }
  return x * ROWS + y;
}

/*
 * Convert LED index back to x,y coordinates
 */
void indexToXY(int index, int &x, int &y) {
  x = index / ROWS;
  y = index % ROWS;
}

/*
 * Calculate Euclidean distance between two points
 */
float distance(float x1, float y1, float x2, float y2) {
  float dx = x2 - x1;
  float dy = y2 - y1;
  return sqrt(dx * dx + dy * dy);
}

/*
 * Convert HSV color to RGB (0-255 range)
 * h: 0-360, s: 0.0-1.0, v: 0.0-1.0
 */
uint32_t hsvToRgb(float h, float s, float v) {
  // Normalize hue to 0-360 range
  while (h < 0) h += 360;
  while (h >= 360) h -= 360;

  float c = v * s;
  float x = c * (1.0 - abs(fmod(h / 60.0, 2.0) - 1.0));
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

  // Convert to 0-255 range
  uint8_t red = (uint8_t)((r + m) * 255);
  uint8_t green = (uint8_t)((g + m) * 255);
  uint8_t blue = (uint8_t)((b + m) * 255);

  return strip.Color(red, green, blue);
}

/*
 * Clear all LEDs (turn them off)
 */
void clearDisplay() {
  strip.clear();
  strip.show();
}

/*
 * Rainbow ripple effect emanating from center of display
 * Triggered by the MEOW button press
 */
void rainbowRippleFromCenter() {
  // Center of display
  float centerX = COLS / 2.0;
  float centerY = ROWS / 2.0;

  unsigned long startTime = millis();
  unsigned long frameDelay = 1000 / RIPPLE_FPS;

  while (millis() - startTime < RIPPLE_DURATION) {
    unsigned long frameStart = millis();
    float elapsed = (millis() - startTime) / 1000.0;  // Convert to seconds

    // Wave propagation speed
    float wavePos = elapsed * 0.5 * 10.0;

    for (int x = 0; x < COLS; x++) {
      for (int y = 0; y < ROWS; y++) {
        // Calculate distance from center
        float dist = distance(centerX, centerY, x, y);

        // Calculate wave intensity
        float waveOffset = abs(dist - wavePos);

        uint32_t color;

        // Create expanding wave with trailing fade
        if (waveOffset < 2.0) {
          // Main wave front
          float intensity = (2.0 - waveOffset) / 2.0;
          intensity = constrain(intensity, 0.0, 1.0);

          // Rainbow colors based on distance (creates rainbow bands)
          float hue = fmod(dist * 40.0 + elapsed * 60.0, 360.0);
          color = hsvToRgb(hue, 1.0, intensity * MAX_BRIGHTNESS);

        } else if (dist < wavePos) {
          // Trailing sparkle/fade effect
          float fadeIntensity = max(0.0, 1.0 - (wavePos - dist) / 5.0);
          float hue = fmod(dist * 40.0, 360.0);
          color = hsvToRgb(hue, 0.8, fadeIntensity * MAX_BRIGHTNESS * 0.3);

        } else {
          // Not yet reached by wave
          color = strip.Color(0, 0, 0);
        }

        int index = xyToIndex(x, y);
        if (index >= 0) {
          strip.setPixelColor(index, color);
        }
      }
    }

    strip.show();

    // Frame rate limiting
    unsigned long frameTime = millis() - frameStart;
    if (frameTime < frameDelay) {
      delay(frameDelay - frameTime);
    }
  }
}

/*
 * Gentle pulsing animation when waiting for button press
 * Returns true if button was pressed during animation
 */
bool idleAnimation() {
  // Simple breathing effect on center pixel
  int centerIndex = xyToIndex(COLS / 2, ROWS / 2);

  unsigned long frameDelay = 1000 / IDLE_FPS;

  for (int i = 0; i < 20; i++) {  // Short animation cycle
    unsigned long frameStart = millis();

    // Calculate breathing intensity (0 to 1)
    float intensity = (sin(idlePhase) + 1.0) / 2.0;
    idlePhase += 0.3;

    // Soft blue/cyan breathing color
    uint32_t color = hsvToRgb(200, 0.7, intensity * MAX_BRIGHTNESS * 0.5);

    if (centerIndex >= 0) {
      strip.setPixelColor(centerIndex, color);
    }

    strip.show();

    // Check for button press during idle
    if (digitalRead(BUTTON_CENTER) == LOW) {  // Button pressed (active low)
      return true;
    }

    // Frame rate limiting
    unsigned long frameTime = millis() - frameStart;
    if (frameTime < frameDelay) {
      delay(frameDelay - frameTime);
    }
  }

  return false;
}

/*
 * Wait for button release with timeout
 */
void waitForButtonRelease(unsigned long timeout = 1000) {
  unsigned long startTime = millis();
  while (digitalRead(BUTTON_CENTER) == LOW) {
    if (millis() - startTime > timeout) {
      break;  // Timeout to prevent infinite loop
    }
    delay(10);
  }
}

/*
 * Arduino setup function - runs once at startup
 */
void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  delay(1000);  // Wait for serial to initialize

  Serial.println("Framework Badge - LED Ripple Animation");
  Serial.println("Press CENTER button for rainbow ripple effect!");
  Serial.println("Initializing LED matrix...");

  // Initialize buttons (active low with internal pull-ups)
  pinMode(BUTTON_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_CENTER, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);

  // Initialize LED strip
  strip.begin();
  strip.setBrightness(255);  // Use full brightness, we control intensity per-pixel
  clearDisplay();

  Serial.println("Initialization complete!");
  Serial.println("Ready for button presses...");
}

/*
 * Arduino main loop - runs continuously
 */
void loop() {
  // Check if center button is pressed (active low)
  if (digitalRead(BUTTON_CENTER) == LOW) {
    unsigned long currentTime = millis();

    // Debounce check
    if (currentTime - lastPressTime > DEBOUNCE_DELAY) {
      Serial.println("MEOW! Rainbow ripple triggered!");
      lastPressTime = currentTime;

      // Trigger rainbow ripple
      rainbowRippleFromCenter();

      // Clear display after ripple
      clearDisplay();

      // Wait for button release
      waitForButtonRelease();
    }
  }

  // Gentle idle animation while waiting
  bool buttonPressed = idleAnimation();

  if (buttonPressed) {
    // Button was pressed during idle, trigger immediately
    unsigned long currentTime = millis();
    if (currentTime - lastPressTime > DEBOUNCE_DELAY) {
      Serial.println("MEOW! Rainbow ripple triggered!");
      lastPressTime = currentTime;

      rainbowRippleFromCenter();
      clearDisplay();

      // Wait for button release
      waitForButtonRelease();
    }
  }
}
