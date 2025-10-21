/*
 * Framework Badge - RP2040 Enhanced Firmware
 *
 * Features:
 * - LED matrix control with web-designed patterns
 * - TFT display with QR code and animated cat
 * - I2C communication with ESP32-S3
 * - RIGHT button shows WiFi QR code for 30 seconds, then animated cat
 *
 * Hardware:
 * - RP2040 + 105 WS2812B LEDs + 3 buttons
 * - GC9A01 round TFT display (240x240)
 * - ESP32-S3 via SAO port (I2C)
 */

#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include <TFT_eSPI.h>
#include "qrcode.h"  // QR code library

// Pin definitions
#define LED_PIN         4
#define NUM_LEDS        105
#define LED_COLS        15
#define LED_ROWS        7

#define BTN_LEFT        5
#define BTN_CENTER      6
#define BTN_RIGHT       7

#define BATTERY_PIN     26
#define STATUS_LED      11

#define I2C_SDA         23
#define I2C_SCL         24
#define ESP32_I2C_ADDR  0x42

// Constants
#define MAX_LED_BRIGHTNESS 0.15

// Hardware objects
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
TFT_eSPI tft = TFT_eSPI();

// State variables
bool btnRightPrev = false;
bool showingQR = false;
unsigned long qrStartTime = 0;
const unsigned long QR_DISPLAY_TIME = 30000; // 30 seconds

// Cat animation
int catX = 0;
int catDirection = 1;
unsigned long lastCatUpdate = 0;
const int CAT_SPEED = 50; // milliseconds per frame

// LED data from web interface
uint8_t ledDataFromWeb[NUM_LEDS * 3];
bool hasWebData = false;

//============================================================================
// SETUP
//============================================================================

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n=== Framework Badge Enhanced ===");

  // Initialize pins
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_CENTER, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);
  pinMode(STATUS_LED, OUTPUT);

  // Initialize LED strip
  strip.begin();
  strip.setBrightness(255);
  strip.clear();
  strip.show();

  // Initialize TFT
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  drawWelcomeScreen();

  // Initialize I2C
  Wire.setSDA(I2C_SDA);
  Wire.setSCL(I2C_SCL);
  Wire.begin();
  Wire.setClock(100000);

  Serial.println("Setup complete!");
}

//============================================================================
// MAIN LOOP
//============================================================================

void loop() {
  unsigned long currentTime = millis();

  // Read buttons
  bool btnRight = !digitalRead(BTN_RIGHT);

  // RIGHT button: Show QR code
  if (btnRight && !btnRightPrev) {
    Serial.println("RIGHT button pressed - Showing QR code");
    showingQR = true;
    qrStartTime = currentTime;
    displayWiFiQR();
  }
  btnRightPrev = btnRight;

  // QR code timeout -> show cat animation
  if (showingQR && (currentTime - qrStartTime > QR_DISPLAY_TIME)) {
    Serial.println("QR timeout - Starting cat animation");
    showingQR = false;
    tft.fillScreen(TFT_BLACK);
  }

  // Update display
  if (showingQR) {
    // Keep showing QR code
  } else {
    // Show animated cat
    if (currentTime - lastCatUpdate > CAT_SPEED) {
      lastCatUpdate = currentTime;
      updateCatAnimation();
    }
  }

  // Check for LED data from ESP32
  checkForWebLEDData();

  // Display web data or idle animation on LEDs
  if (hasWebData) {
    displayWebLEDData();
  } else {
    idleAnimation();
  }

  delay(20);
}

//============================================================================
// TFT DISPLAY FUNCTIONS
//============================================================================

void drawWelcomeScreen() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("PIXEL KITTY", 120, 100, 4);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("Ready!", 120, 140, 2);
}

void displayWiFiQR() {
  Serial.println("Generating QR code...");

  // Get QR data from ESP32
  String qrData = getQRDataFromESP32();
  if (qrData.length() == 0) {
    qrData = "WIFI:T:WPA;S:PixelKitty;P:meow1234;;"; // Fallback
  }

  Serial.print("QR Data: ");
  Serial.println(qrData);

  // Create QR code
  QRCode qrcode;
  uint8_t qrcodeData[qrcode_getBufferSize(3)];
  qrcode_initText(&qrcode, qrcodeData, 3, 0, qrData.c_str());

  // Clear screen
  tft.fillScreen(TFT_WHITE);

  // Draw title
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("Scan to Connect", 120, 20, 2);

  // Calculate QR code position and size
  int qrSize = qrcode.size;
  int pixelSize = 5; // Size of each QR module
  int qrWidth = qrSize * pixelSize;
  int offsetX = (240 - qrWidth) / 2;
  int offsetY = 50;

  // Draw QR code
  for (int y = 0; y < qrSize; y++) {
    for (int x = 0; x < qrSize; x++) {
      uint16_t color = qrcode_getModule(&qrcode, x, y) ? TFT_BLACK : TFT_WHITE;
      tft.fillRect(offsetX + x * pixelSize, offsetY + y * pixelSize,
                   pixelSize, pixelSize, color);
    }
  }

  // Draw instructions
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.drawString("WiFi: PixelKitty", 120, 200, 2);
  tft.drawString("Pass: meow1234", 120, 220, 2);

  Serial.println("QR code displayed!");
}

void updateCatAnimation() {
  // Clear old cat position
  tft.fillCircle(catX, 120, 20, TFT_BLACK);

  // Update position
  catX += catDirection * 5;

  // Bounce at edges
  if (catX > 220 || catX < 20) {
    catDirection *= -1;
  }

  // Draw cat (simple pixel art)
  drawCat(catX, 120, catDirection > 0);
}

void drawCat(int x, int y, bool facingRight) {
  // Draw a simple cat sprite (16x16 pixels scaled up)

  // Body (circle)
  tft.fillCircle(x, y, 15, TFT_ORANGE);

  // Ears (triangles)
  int earOffset = facingRight ? 1 : -1;
  tft.fillTriangle(x - 10 * earOffset, y - 12,
                    x - 15 * earOffset, y - 20,
                    x - 5 * earOffset, y - 15, TFT_ORANGE);
  tft.fillTriangle(x + 5 * earOffset, y - 15,
                    x + 10 * earOffset, y - 12,
                    x + 15 * earOffset, y - 20, TFT_ORANGE);

  // Eyes
  int eyeX = facingRight ? x + 5 : x - 5;
  tft.fillCircle(eyeX, y - 3, 3, TFT_BLACK);
  tft.fillCircle(eyeX, y - 3, 1, TFT_GREEN); // Pupil

  // Nose
  tft.fillTriangle(x, y + 2, x - 2, y + 5, x + 2, y + 5, TFT_PINK);

  // Whiskers
  tft.drawLine(x + (facingRight ? 8 : -8), y, x + (facingRight ? 18 : -18), y - 2, TFT_WHITE);
  tft.drawLine(x + (facingRight ? 8 : -8), y + 3, x + (facingRight ? 18 : -18), y + 3, TFT_WHITE);
  tft.drawLine(x + (facingRight ? 8 : -8), y + 6, x + (facingRight ? 18 : -18), y + 8, TFT_WHITE);

  // Tail (curve)
  for (int i = 0; i < 5; i++) {
    int tailX = x - (facingRight ? 12 : -12) + i * (facingRight ? -2 : 2);
    int tailY = y + 10 + i;
    tft.drawCircle(tailX, tailY, 2, TFT_ORANGE);
  }

  // Paws (if you want to add walking animation)
  static int pawFrame = 0;
  pawFrame = (pawFrame + 1) % 4;

  int paw1Y = y + 15 + (pawFrame < 2 ? 0 : 2);
  int paw2Y = y + 15 + (pawFrame < 2 ? 2 : 0);

  tft.fillCircle(x - 5, paw1Y, 3, TFT_ORANGE);
  tft.fillCircle(x + 5, paw2Y, 3, TFT_ORANGE);
}

//============================================================================
// I2C COMMUNICATION
//============================================================================

String getQRDataFromESP32() {
  // Request QR code data from ESP32
  Wire.beginTransmission(ESP32_I2C_ADDR);
  Wire.write(0x60); // CMD_GET_QR_DATA (define this in ESP32)
  uint8_t error = Wire.endTransmission();

  if (error != 0) {
    Serial.println("Failed to request QR data");
    return "";
  }

  // Request up to 100 bytes
  Wire.requestFrom(ESP32_I2C_ADDR, 100);
  delay(10);

  String qrData = "";
  while (Wire.available()) {
    char c = Wire.read();
    if (c == 0) break; // Null terminator
    qrData += c;
  }

  return qrData;
}

void checkForWebLEDData() {
  // Check if ESP32 has new LED data from web interface
  // For simplicity, we'll poll periodically
  static unsigned long lastCheck = 0;
  if (millis() - lastCheck < 1000) return; // Check every second
  lastCheck = millis();

  Wire.beginTransmission(ESP32_I2C_ADDR);
  Wire.write(0x61); // CMD_GET_LED_DATA
  uint8_t error = Wire.endTransmission();

  if (error != 0) return;

  // Request status byte first
  Wire.requestFrom(ESP32_I2C_ADDR, 1);
  if (!Wire.available()) return;

  uint8_t status = Wire.read();
  if (status == 0x00) {
    // No new data
    return;
  }

  // New data available - request it in chunks
  Serial.println("New LED data available from web!");

  int totalBytes = NUM_LEDS * 3;
  int bytesReceived = 0;
  const int chunkSize = 32;

  while (bytesReceived < totalBytes) {
    int toRequest = min(chunkSize, totalBytes - bytesReceived);

    Wire.requestFrom(ESP32_I2C_ADDR, toRequest);
    delay(5);

    while (Wire.available() && bytesReceived < totalBytes) {
      ledDataFromWeb[bytesReceived++] = Wire.read();
    }
  }

  if (bytesReceived == totalBytes) {
    hasWebData = true;
    Serial.println("LED data received from web!");
  }
}

//============================================================================
// LED FUNCTIONS
//============================================================================

void displayWebLEDData() {
  // Display LED pattern from web interface
  for (int i = 0; i < NUM_LEDS; i++) {
    uint8_t r = ledDataFromWeb[i * 3];
    uint8_t g = ledDataFromWeb[i * 3 + 1];
    uint8_t b = ledDataFromWeb[i * 3 + 2];

    // Apply brightness limit
    r = (uint8_t)(r * MAX_LED_BRIGHTNESS);
    g = (uint8_t)(g * MAX_LED_BRIGHTNESS);
    b = (uint8_t)(b * MAX_LED_BRIGHTNESS);

    strip.setPixelColor(i, strip.Color(r, g, b));
  }
  strip.show();
}

void idleAnimation() {
  // Simple breathing animation
  static float phase = 0;
  phase += 0.1;

  int centerIndex = xyToIndex(LED_COLS / 2, LED_ROWS / 2);
  float intensity = (sin(phase) + 1.0) / 2.0;

  uint32_t color = strip.Color(
    (uint8_t)(intensity * 50 * MAX_LED_BRIGHTNESS),
    (uint8_t)(intensity * 150 * MAX_LED_BRIGHTNESS),
    (uint8_t)(intensity * 200 * MAX_LED_BRIGHTNESS)
  );

  if (centerIndex >= 0) {
    strip.setPixelColor(centerIndex, color);
  }
  strip.show();
}

int xyToIndex(int x, int y) {
  if (x < 0 || x >= LED_COLS || y < 0 || y >= LED_ROWS) return -1;
  return x * LED_ROWS + y;
}
