/*
 * Framework Badge - RP2040 I2C Master Firmware
 *
 * Controls LED matrix, buttons, and communicates with ESP32-S3 coprocessor
 * via I2C for WiFi/Bluetooth capabilities.
 *
 * Hardware:
 * - RP2040 microcontroller (master)
 * - 105 WS2812B LEDs (15x7 matrix) on GPIO 4
 * - 3 buttons on GPIO 5, 6, 7
 * - Battery monitor on GPIO 26
 * - I2C to ESP32-S3: SDA=GPIO 23 (or 24), SCL=GPIO 24 (or 25)
 *
 * ESP32-S3 Connection:
 * - Connected via SAO port (4-pin: VCC, GND, SDA, SCL)
 * - I2C address: 0x42
 * - Provides WiFi and Bluetooth services
 */

#include <Wire.h>
#include <Adafruit_NeoPixel.h>

// ============================================================================
// PIN DEFINITIONS
// ============================================================================

#define LED_PIN         4      // WS2812B data line
#define NUM_LEDS        105    // 15 columns × 7 rows
#define LED_COLS        15
#define LED_ROWS        7

#define BTN_LEFT        5      // Left button
#define BTN_CENTER      6      // Center button (MEOW)
#define BTN_RIGHT       7      // Right button

#define BATTERY_PIN     26     // ADC0 - battery voltage monitor
#define STATUS_LED      11     // Red status LED

// I2C pins for ESP32-S3 communication
#define I2C_SDA         23     // Or use pin 24
#define I2C_SCL         24     // Or use pin 25

// ============================================================================
// ESP32-S3 I2C CONFIGURATION
// ============================================================================

#define ESP32_I2C_ADDR  0x42   // ESP32-S3 slave address

// Command codes (must match ESP32-S3 firmware)
#define CMD_PING                0x00
#define CMD_GET_STATUS          0x01
#define CMD_WIFI_CONNECT        0x10
#define CMD_WIFI_DISCONNECT     0x11
#define CMD_WIFI_SCAN           0x12
#define CMD_BLE_BEACON          0x20
#define CMD_BLE_STOP            0x21
#define CMD_WEB_SERVER          0x30
#define CMD_HTTP_GET            0x31
#define CMD_LED_NOTIFY          0x40
#define CMD_BUTTON_EVENT        0x41
#define CMD_BATTERY_LEVEL       0x42

// Status codes
#define STATUS_OK               0x00
#define STATUS_ERROR            0xFF
#define STATUS_BUSY             0x01
#define STATUS_INVALID_CMD      0x02
#define STATUS_WIFI_DISCONNECTED 0x10
#define STATUS_WIFI_CONNECTED   0x11
#define STATUS_BLE_ACTIVE       0x20

// ============================================================================
// GLOBAL VARIABLES
// ============================================================================

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// Button states
bool btnLeftState = false;
bool btnCenterState = false;
bool btnRightState = false;
bool btnLeftPrev = false;
bool btnCenterPrev = false;
bool btnRightPrev = false;

// Battery monitoring
float batteryVoltage = 0.0;
unsigned long lastBatteryUpdate = 0;
const unsigned long BATTERY_UPDATE_INTERVAL = 5000; // 5 seconds

// ESP32-S3 status
bool esp32Connected = false;
bool wifiConnected = false;
bool bleActive = false;
int8_t wifiRSSI = 0;
unsigned long lastESP32Check = 0;
const unsigned long ESP32_CHECK_INTERVAL = 10000; // 10 seconds

// LED animation state
#define MAX_LED_BRIGHTNESS 0.15  // Safety limit
float animationPhase = 0;

// ============================================================================
// SETUP
// ============================================================================

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n\n=== Framework Badge - RP2040 Master ===");
  Serial.println("Dual-MCU System with ESP32-S3 Coprocessor");

  // Initialize pins
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_CENTER, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);
  pinMode(STATUS_LED, OUTPUT);
  pinMode(BATTERY_PIN, INPUT);

  // Initialize LED strip
  Serial.println("Initializing LED matrix...");
  strip.begin();
  strip.setBrightness(255);
  strip.clear();
  strip.show();

  // Startup animation
  startupAnimation();

  // Initialize I2C as master
  Serial.print("Initializing I2C (SDA=");
  Serial.print(I2C_SDA);
  Serial.print(", SCL=");
  Serial.print(I2C_SCL);
  Serial.println(")...");

  Wire.setSDA(I2C_SDA);
  Wire.setSCL(I2C_SCL);
  Wire.begin();
  Wire.setClock(100000); // 100kHz (standard I2C speed)

  delay(500); // Give ESP32-S3 time to boot

  // Test ESP32-S3 connection
  Serial.println("Checking ESP32-S3 connection...");
  esp32Connected = pingESP32();

  if (esp32Connected) {
    Serial.println("✓ ESP32-S3 connected!");
    digitalWrite(STATUS_LED, HIGH);
    delay(100);
    digitalWrite(STATUS_LED, LOW);

    // Get initial status
    getESP32Status();
  } else {
    Serial.println("✗ ESP32-S3 not responding");
    Serial.println("  Check SAO port connections");
    blinkStatusLED(3); // Error indication
  }

  // Read initial battery voltage
  batteryVoltage = readBatteryVoltage();
  Serial.print("Battery voltage: ");
  Serial.print(batteryVoltage);
  Serial.println("V");

  Serial.println("\n=== Setup complete ===\n");
  Serial.println("Controls:");
  Serial.println("  LEFT button:   WiFi scan");
  Serial.println("  CENTER button: Rainbow ripple + BLE beacon");
  Serial.println("  RIGHT button:  Get ESP32 status");
  Serial.println();
}

// ============================================================================
// MAIN LOOP
// ============================================================================

void loop() {
  unsigned long currentTime = millis();

  // Update button states
  updateButtons();

  // Handle button press events
  if (btnLeftState && !btnLeftPrev) {
    Serial.println("LEFT button pressed - WiFi scan");
    handleWiFiScan();
  }

  if (btnCenterState && !btnCenterPrev) {
    Serial.println("CENTER button pressed - Rainbow ripple + BLE beacon");
    rainbowRipple();
    startBLEBeacon();
  }

  if (btnRightState && !btnRightPrev) {
    Serial.println("RIGHT button pressed - Get ESP32 status");
    getESP32Status();
  }

  // Save previous button states
  btnLeftPrev = btnLeftState;
  btnCenterPrev = btnCenterState;
  btnRightPrev = btnRightState;

  // Idle LED animation
  if (!btnCenterState) {
    idleAnimation();
  }

  // Periodic battery monitoring
  if (currentTime - lastBatteryUpdate >= BATTERY_UPDATE_INTERVAL) {
    lastBatteryUpdate = currentTime;
    batteryVoltage = readBatteryVoltage();

    // Send battery level to ESP32-S3
    if (esp32Connected) {
      sendBatteryLevel(batteryVoltage);
    }

    // Low battery warning
    if (batteryVoltage < 2.3 && batteryVoltage > 0.5) {
      Serial.println("⚠ WARNING: Low battery!");
      blinkStatusLED(2);
    }
  }

  // Periodic ESP32-S3 status check
  if (currentTime - lastESP32Check >= ESP32_CHECK_INTERVAL) {
    lastESP32Check = currentTime;
    if (esp32Connected) {
      getESP32Status();
    }
  }

  delay(20); // ~50 FPS
}

// ============================================================================
// BUTTON HANDLING
// ============================================================================

void updateButtons() {
  btnLeftState = !digitalRead(BTN_LEFT);     // Active low
  btnCenterState = !digitalRead(BTN_CENTER);
  btnRightState = !digitalRead(BTN_RIGHT);
}

// ============================================================================
// BATTERY MONITORING
// ============================================================================

float readBatteryVoltage() {
  // Average multiple readings for stability
  float sum = 0;
  for (int i = 0; i < 10; i++) {
    int adcValue = analogRead(BATTERY_PIN);
    float adcVoltage = (adcValue / 4095.0) * 3.3;
    sum += adcVoltage / 0.357; // Voltage divider scaling
    delay(1);
  }
  return sum / 10.0;
}

// ============================================================================
// LED ANIMATIONS
// ============================================================================

int xyToIndex(int x, int y) {
  if (x < 0 || x >= LED_COLS || y < 0 || y >= LED_ROWS) return -1;
  return x * LED_ROWS + y;
}

float distance(float x1, float y1, float x2, float y2) {
  return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

uint32_t hsvToRgb(float h, float s, float v) {
  float c = v * s;
  float x = c * (1.0 - fabs(fmod(h / 60.0, 2.0) - 1.0));
  float m = v - c;
  float r, g, b;

  if (h < 60) { r = c; g = x; b = 0; }
  else if (h < 120) { r = x; g = c; b = 0; }
  else if (h < 180) { r = 0; g = c; b = x; }
  else if (h < 240) { r = 0; g = x; b = c; }
  else if (h < 300) { r = x; g = 0; b = c; }
  else { r = c; g = 0; b = x; }

  return strip.Color(
    (uint8_t)((r + m) * 255),
    (uint8_t)((g + m) * 255),
    (uint8_t)((b + m) * 255)
  );
}

void idleAnimation() {
  int centerIndex = xyToIndex(LED_COLS / 2, LED_ROWS / 2);
  animationPhase += 0.1;
  float intensity = (sin(animationPhase) + 1.0) / 2.0;
  uint32_t color = hsvToRgb(200, 0.7, intensity * MAX_LED_BRIGHTNESS * 0.5);

  if (centerIndex >= 0) {
    strip.setPixelColor(centerIndex, color);
  }
  strip.show();
}

void rainbowRipple() {
  const float centerX = LED_COLS / 2.0;
  const float centerY = LED_ROWS / 2.0;
  const unsigned long duration = 2500;
  unsigned long startTime = millis();

  while (millis() - startTime < duration) {
    float elapsed = (millis() - startTime) / 1000.0;
    float wavePos = elapsed * 5.0;

    for (int x = 0; x < LED_COLS; x++) {
      for (int y = 0; y < LED_ROWS; y++) {
        float dist = distance(centerX, centerY, x, y);
        float waveOffset = fabs(dist - wavePos);

        if (waveOffset < 2.0) {
          float intensity = (2.0 - waveOffset) / 2.0;
          float hue = fmod(dist * 40.0 + elapsed * 60.0, 360.0);
          uint32_t color = hsvToRgb(hue, 1.0, intensity * MAX_LED_BRIGHTNESS);
          int index = xyToIndex(x, y);
          if (index >= 0) strip.setPixelColor(index, color);
        } else {
          int index = xyToIndex(x, y);
          if (index >= 0) strip.setPixelColor(index, 0);
        }
      }
    }
    strip.show();
    delay(40);
  }

  strip.clear();
  strip.show();
}

void startupAnimation() {
  // Quick LED test
  for (int i = 0; i < 3; i++) {
    strip.fill(strip.Color(10, 0, 0));
    strip.show();
    delay(100);
    strip.clear();
    strip.show();
    delay(100);
  }
}

void blinkStatusLED(int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(STATUS_LED, HIGH);
    delay(100);
    digitalWrite(STATUS_LED, LOW);
    delay(100);
  }
}

// ============================================================================
// I2C COMMUNICATION WITH ESP32-S3
// ============================================================================

bool pingESP32() {
  Wire.beginTransmission(ESP32_I2C_ADDR);
  Wire.write(CMD_PING);
  uint8_t error = Wire.endTransmission();

  if (error != 0) {
    Serial.print("I2C error: ");
    Serial.println(error);
    return false;
  }

  // Request response
  Wire.requestFrom(ESP32_I2C_ADDR, 1);
  if (Wire.available()) {
    uint8_t response = Wire.read();
    return (response == 0xA5); // Expected ping response
  }

  return false;
}

void getESP32Status() {
  Serial.println("Getting ESP32-S3 status...");

  Wire.beginTransmission(ESP32_I2C_ADDR);
  Wire.write(CMD_GET_STATUS);
  uint8_t error = Wire.endTransmission();

  if (error != 0) {
    Serial.println("✗ Failed to send status request");
    esp32Connected = false;
    return;
  }

  // Request 4 bytes: [status, wifi_status, ble_status, rssi]
  Wire.requestFrom(ESP32_I2C_ADDR, 4);
  delay(10);

  if (Wire.available() >= 4) {
    uint8_t status = Wire.read();
    uint8_t wifi_status = Wire.read();
    uint8_t ble_status = Wire.read();
    int8_t rssi = (int8_t)Wire.read();

    wifiConnected = (wifi_status == STATUS_WIFI_CONNECTED);
    bleActive = (ble_status == STATUS_BLE_ACTIVE);
    wifiRSSI = rssi;

    Serial.println("ESP32-S3 Status:");
    Serial.print("  WiFi: ");
    Serial.println(wifiConnected ? "Connected" : "Disconnected");
    if (wifiConnected) {
      Serial.print("  RSSI: ");
      Serial.print(rssi);
      Serial.println(" dBm");
    }
    Serial.print("  BLE: ");
    Serial.println(bleActive ? "Active" : "Inactive");
  } else {
    Serial.println("✗ No response from ESP32-S3");
  }
}

void handleWiFiScan() {
  if (!esp32Connected) {
    Serial.println("✗ ESP32-S3 not connected");
    return;
  }

  Serial.println("Requesting WiFi scan...");

  Wire.beginTransmission(ESP32_I2C_ADDR);
  Wire.write(CMD_WIFI_SCAN);
  Wire.endTransmission();

  // WiFi scan takes time, wait then check status
  delay(3000);

  Serial.println("Scan complete (results available via Serial on ESP32-S3)");
  blinkStatusLED(1);
}

void startBLEBeacon() {
  if (!esp32Connected) {
    Serial.println("✗ ESP32-S3 not connected");
    return;
  }

  Serial.println("Starting BLE beacon...");

  // Send beacon command with badge ID
  Wire.beginTransmission(ESP32_I2C_ADDR);
  Wire.write(CMD_BLE_BEACON);
  Wire.write(4); // Data length
  Wire.write('M'); // Badge ID: "MEOW"
  Wire.write('E');
  Wire.write('O');
  Wire.write('W');
  Wire.endTransmission();

  Serial.println("BLE beacon started");
  digitalWrite(STATUS_LED, HIGH);
  delay(50);
  digitalWrite(STATUS_LED, LOW);
}

void sendBatteryLevel(float voltage) {
  if (!esp32Connected) return;

  // Convert voltage to 2 bytes (voltage * 100)
  uint16_t voltageInt = (uint16_t)(voltage * 100);

  Wire.beginTransmission(ESP32_I2C_ADDR);
  Wire.write(CMD_BATTERY_LEVEL);
  Wire.write(2); // Data length
  Wire.write(voltageInt >> 8);   // High byte
  Wire.write(voltageInt & 0xFF);  // Low byte
  Wire.endTransmission();
}

// ============================================================================
// SERIAL COMMAND INTERFACE (FOR TESTING)
// ============================================================================

void serialEvent() {
  while (Serial.available()) {
    char cmd = Serial.read();

    switch (cmd) {
      case 'p':
        Serial.println("Pinging ESP32-S3...");
        esp32Connected = pingESP32();
        Serial.println(esp32Connected ? "✓ Connected" : "✗ Not responding");
        break;

      case 's':
        getESP32Status();
        break;

      case 'w':
        handleWiFiScan();
        break;

      case 'b':
        startBLEBeacon();
        break;

      case 'r':
        rainbowRipple();
        break;

      case 'h':
        printHelp();
        break;
    }
  }
}

void printHelp() {
  Serial.println("\n=== Serial Commands ===");
  Serial.println("p - Ping ESP32-S3");
  Serial.println("s - Get ESP32-S3 status");
  Serial.println("w - WiFi scan");
  Serial.println("b - Start BLE beacon");
  Serial.println("r - Rainbow ripple");
  Serial.println("h - This help");
  Serial.println();
}
