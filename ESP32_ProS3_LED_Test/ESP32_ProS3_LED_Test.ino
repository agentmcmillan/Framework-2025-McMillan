/*
 * Unexpected Maker ESP32 ProS3 - RGB LED Test
 *
 * Simple sketch to blink the onboard WS2812B RGB LED.
 * The ProS3 has a single addressable RGB LED on GPIO48.
 *
 * Hardware:
 * - Unexpected Maker ESP32-S3 ProS3
 * - Onboard WS2812B RGB LED on GPIO48
 *
 * LED Patterns:
 * - Red → Green → Blue → White → Off (cycle)
 * - 500ms per color
 *
 * Board Selection:
 * - Board: ESP32S3 Dev Module (or UM ProS3 if board package has it)
 * - USB CDC On Boot: Enabled
 * - USB DFU On Boot: Disabled
 * - Flash Size: 16MB
 * - PSRAM: OPI PSRAM
 *
 * Required Library:
 * - Adafruit_NeoPixel (install via Library Manager)
 */

#include <Adafruit_NeoPixel.h>

// UM ProS3 RGB LED is on GPIO48
#define RGB_LED_PIN 48
#define NUM_LEDS 1

// Create NeoPixel object
Adafruit_NeoPixel led(NUM_LEDS, RGB_LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);
  delay(1000);  // Wait for serial connection

  Serial.println("\n╔════════════════════════════════════════╗");
  Serial.println("║  UM ESP32 ProS3 - RGB LED Test         ║");
  Serial.println("╚════════════════════════════════════════╝\n");

  // Initialize the RGB LED
  led.begin();
  led.setBrightness(50);  // 20% brightness (0-255)
  led.clear();
  led.show();

  Serial.println("RGB LED initialized on GPIO48");
  Serial.println("Starting color cycle...\n");
}

void loop() {
  // Red
  Serial.println("🔴 Red");
  led.setPixelColor(0, led.Color(255, 0, 0));
  led.show();
  delay(500);

  // Green
  Serial.println("🟢 Green");
  led.setPixelColor(0, led.Color(0, 255, 0));
  led.show();
  delay(500);

  // Blue
  Serial.println("🔵 Blue");
  led.setPixelColor(0, led.Color(0, 0, 255));
  led.show();
  delay(500);

  // Cyan
  Serial.println("🔵 Cyan");
  led.setPixelColor(0, led.Color(0, 255, 255));
  led.show();
  delay(500);

  // Magenta
  Serial.println("🟣 Magenta");
  led.setPixelColor(0, led.Color(255, 0, 255));
  led.show();
  delay(500);

  // Yellow
  Serial.println("🟡 Yellow");
  led.setPixelColor(0, led.Color(255, 255, 0));
  led.show();
  delay(500);

  // White
  Serial.println("⚪ White");
  led.setPixelColor(0, led.Color(255, 255, 255));
  led.show();
  delay(500);

  // Off
  Serial.println("⚫ Off\n");
  led.clear();
  led.show();
  delay(500);
}
