/*
 * Framework Badge - MPU6050 Tilt-Controlled LED Matrix
 *
 * Uses accelerometer data to create gravity-responsive LED effects:
 * - "Liquid" effect: LEDs pool at the lowest edge based on tilt
 * - Color changes based on tilt angle
 * - Safe power management for battery operation
 *
 * Hardware:
 * - RP2040-based Framework Badge
 * - 15x7 WS2812B LED matrix (IO#4)
 * - MPU6050 IMU via Whisker Port I2C (shared with ESP32 ProS3)
 * - Three buttons (IO#5, #6, #7)
 *
 * Wiring (Shared I2C Bus):
 * The MPU6050 shares the I2C bus with the ESP32 ProS3 coprocessor.
 * Both devices connect to the same SDA/SCL pins (parallel connection).
 *
 * Physical wiring options:
 * 1. Breadboard: Connect both MPU6050 and ESP32 to same pins
 * 2. Daisy-chain: MPU6050 → ESP32 → Badge SAO port
 * 3. Y-cable: Badge SAO → splits to MPU6050 and ESP32
 *
 * Pin connections:
 *   Badge Whisker/SAO Port     MPU6050         ESP32 ProS3
 *   ----------------------     -------         -----------
 *   +3.3V               ───┬──→ VCC      ───┬──→ 3V3 (GPIO1/2 bus)
 *   GND                 ───┼──→ GND      ───┼──→ GND
 *   IO#23 (SDA)         ───┼──→ SDA      ───┼──→ GPIO1 (SDA)
 *   IO#24 (SCL)         ───┴──→ SCL      ───┴──→ GPIO2 (SCL)
 *
 * I2C Addresses:
 * - MPU6050: 0x68 (default, or 0x69 if AD0 pulled high)
 * - ESP32 ProS3: 0x42
 *
 * Required Libraries:
 * - Adafruit_NeoPixel
 * - Adafruit MPU6050
 * - Adafruit Unified Sensor
 */

#include <Adafruit_NeoPixel.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

// Hardware pin definitions
#define LED_PIN 4
#define BUTTON_LEFT 5
#define BUTTON_CENTER 6
#define BUTTON_RIGHT 7

// I2C pins for MPU6050
#define SDA_PIN 23
#define SCL_PIN 24

// LED matrix configuration
#define LED_COUNT 105
#define LED_COLS 15
#define LED_ROWS 7
#define MAX_BRIGHTNESS 64  // 25% max for power safety

// Animation modes
enum AnimationMode {
  MODE_LIQUID,      // Liquid pooling effect
  MODE_BUBBLE,      // Rising bubbles
  MODE_TILT_METER   // Tilt angle visualization
};

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_MPU6050 mpu;

AnimationMode currentMode = MODE_LIQUID;
unsigned long lastModeChange = 0;
const unsigned long MODE_DEBOUNCE = 300;

// Liquid simulation state
float liquid[LED_COLS][LED_ROWS] = {0};  // Amount of "liquid" at each position

void setup() {
  Serial.begin(115200);

  // Initialize buttons
  pinMode(BUTTON_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_CENTER, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);

  // Initialize I2C with custom pins
  Wire.setSDA(SDA_PIN);
  Wire.setSCL(SCL_PIN);
  Wire.begin();

  // Initialize LED matrix
  strip.begin();
  strip.setBrightness(MAX_BRIGHTNESS);
  strip.clear();
  strip.show();

  // Initialize MPU6050 (I2C address 0x68, shared bus with ESP32)
  Serial.println("Initializing MPU6050 on shared I2C bus...");
  Serial.println("Note: ESP32 ProS3 is at 0x42, MPU6050 at 0x68");

  if (!mpu.begin(0x68, &Wire)) {
    Serial.println("Failed to find MPU6050 chip at address 0x68!");
    Serial.println("Check wiring and ensure I2C bus is shared correctly.");
    Serial.println("If MPU6050 AD0 pin is HIGH, try address 0x69.");

    // Show error pattern on LEDs
    for (int i = 0; i < LED_COUNT; i++) {
      strip.setPixelColor(i, strip.Color(255, 0, 0));
    }
    strip.show();
    while (1) {
      delay(10);
    }
  }

  Serial.println("MPU6050 Found on shared I2C bus!");

  // Configure MPU6050
  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  // Success animation
  for (int brightness = 0; brightness < MAX_BRIGHTNESS; brightness += 4) {
    strip.setBrightness(brightness);
    for (int i = 0; i < LED_COUNT; i++) {
      strip.setPixelColor(i, strip.Color(0, 255, 0));
    }
    strip.show();
    delay(10);
  }
  delay(500);
  strip.clear();
  strip.show();

  Serial.println("Ready! Use buttons to change modes:");
  Serial.println("LEFT: Liquid mode");
  Serial.println("CENTER: Bubble mode");
  Serial.println("RIGHT: Tilt meter mode");
}

void loop() {
  // Check for mode changes
  handleButtons();

  // Get sensor readings
  sensors_event_t accel, gyro, temp;
  mpu.getEvent(&accel, &gyro, &temp);

  // Normalize accelerometer values (-1 to 1)
  float accelX = constrain(accel.acceleration.x / 10.0, -1.0, 1.0);
  float accelY = constrain(accel.acceleration.y / 10.0, -1.0, 1.0);
  float accelZ = constrain(accel.acceleration.z / 10.0, -1.0, 1.0);

  // Run current animation mode
  switch (currentMode) {
    case MODE_LIQUID:
      liquidEffect(accelX, accelY);
      break;
    case MODE_BUBBLE:
      bubbleEffect(accelX, accelY);
      break;
    case MODE_TILT_METER:
      tiltMeterEffect(accelX, accelY);
      break;
  }

  delay(30);  // ~33 FPS
}

void handleButtons() {
  unsigned long now = millis();

  if (now - lastModeChange < MODE_DEBOUNCE) return;

  if (digitalRead(BUTTON_LEFT) == LOW) {
    currentMode = MODE_LIQUID;
    lastModeChange = now;
    resetLiquid();
    Serial.println("Mode: LIQUID");
  } else if (digitalRead(BUTTON_CENTER) == LOW) {
    currentMode = MODE_BUBBLE;
    lastModeChange = now;
    resetLiquid();
    Serial.println("Mode: BUBBLE");
  } else if (digitalRead(BUTTON_RIGHT) == LOW) {
    currentMode = MODE_TILT_METER;
    lastModeChange = now;
    Serial.println("Mode: TILT METER");
  }
}

// Convert X,Y coordinates to LED index (column-based addressing)
int xyToIndex(int x, int y) {
  if (x < 0 || x >= LED_COLS || y < 0 || y >= LED_ROWS) return -1;
  return x * LED_ROWS + y;
}

// Liquid pooling effect - LEDs act like water responding to gravity
void liquidEffect(float accelX, float accelY) {
  // Decay existing liquid slightly
  for (int x = 0; x < LED_COLS; x++) {
    for (int y = 0; y < LED_ROWS; y++) {
      liquid[x][y] *= 0.95;
    }
  }

  // Add new liquid at the top/sides based on tilt
  for (int x = 0; x < LED_COLS; x++) {
    liquid[x][0] += 0.3;  // Constant drip from top
  }

  // Simulate gravity - liquid moves toward the "down" direction
  for (int iter = 0; iter < 3; iter++) {  // Multiple iterations for smoother flow
    for (int x = 0; x < LED_COLS; x++) {
      for (int y = 0; y < LED_ROWS; y++) {
        if (liquid[x][y] < 0.1) continue;

        // Calculate where gravity is pointing
        int nextX = x + (accelX > 0.2 ? 1 : (accelX < -0.2 ? -1 : 0));
        int nextY = y + (accelY > 0.2 ? 1 : (accelY < -0.2 ? -1 : 0));

        // Transfer liquid to lower position
        if (nextX >= 0 && nextX < LED_COLS && nextY >= 0 && nextY < LED_ROWS) {
          float transfer = liquid[x][y] * 0.3;
          liquid[x][y] -= transfer;
          liquid[nextX][nextY] += transfer;
        }
      }
    }
  }

  // Render liquid to LEDs
  strip.clear();
  for (int x = 0; x < LED_COLS; x++) {
    for (int y = 0; y < LED_ROWS; y++) {
      float amount = constrain(liquid[x][y], 0, 1);
      if (amount > 0.05) {
        // Color based on amount: blue -> cyan -> white
        uint8_t blue = 255;
        uint8_t green = amount * 200;
        uint8_t red = amount > 0.7 ? (amount - 0.7) * 850 : 0;

        int idx = xyToIndex(x, y);
        if (idx >= 0) {
          strip.setPixelColor(idx, strip.Color(red, green, blue));
        }
      }
    }
  }
  strip.show();
}

// Bubble effect - bubbles rise against gravity
void bubbleEffect(float accelX, float accelY) {
  static unsigned long lastBubble = 0;
  unsigned long now = millis();

  // Decay existing bubbles
  for (int x = 0; x < LED_COLS; x++) {
    for (int y = 0; y < LED_ROWS; y++) {
      liquid[x][y] *= 0.92;
    }
  }

  // Spawn new bubbles at the "bottom" edge
  if (now - lastBubble > 200) {
    // Determine bottom edge based on tilt
    if (abs(accelY) > abs(accelX)) {
      // Gravity mostly vertical
      int spawnY = accelY > 0 ? LED_ROWS - 1 : 0;
      liquid[random(LED_COLS)][spawnY] = 1.0;
    } else {
      // Gravity mostly horizontal
      int spawnX = accelX > 0 ? LED_COLS - 1 : 0;
      liquid[spawnX][random(LED_ROWS)] = 1.0;
    }
    lastBubble = now;
  }

  // Move bubbles against gravity
  for (int x = LED_COLS - 1; x >= 0; x--) {
    for (int y = LED_ROWS - 1; y >= 0; y--) {
      if (liquid[x][y] < 0.1) continue;

      // Move opposite to gravity direction
      int nextX = x - (accelX > 0.2 ? 1 : (accelX < -0.2 ? -1 : 0));
      int nextY = y - (accelY > 0.2 ? 1 : (accelY < -0.2 ? -1 : 0));

      if (nextX >= 0 && nextX < LED_COLS && nextY >= 0 && nextY < LED_ROWS) {
        float transfer = liquid[x][y] * 0.5;
        liquid[x][y] -= transfer;
        liquid[nextX][nextY] += transfer;
      }
    }
  }

  // Render bubbles
  strip.clear();
  for (int x = 0; x < LED_COLS; x++) {
    for (int y = 0; y < LED_ROWS; y++) {
      float amount = constrain(liquid[x][y], 0, 1);
      if (amount > 0.05) {
        // Color: yellow/orange bubbles
        uint8_t red = 255;
        uint8_t green = 200;
        uint8_t blue = amount > 0.5 ? (1.0 - amount) * 255 : amount * 255;

        int idx = xyToIndex(x, y);
        if (idx >= 0) {
          strip.setPixelColor(idx, strip.Color(red, green, blue));
        }
      }
    }
  }
  strip.show();
}

// Tilt meter - visualize the tilt angle and direction
void tiltMeterEffect(float accelX, float accelY) {
  strip.clear();

  // Calculate tilt vector
  int centerX = LED_COLS / 2;
  int centerY = LED_ROWS / 2;

  // Draw center dot
  strip.setPixelColor(xyToIndex(centerX, centerY), strip.Color(100, 100, 100));

  // Draw tilt direction line
  for (float t = 0; t <= 1.0; t += 0.2) {
    int x = centerX + (int)(accelX * 6 * t);
    int y = centerY + (int)(accelY * 3 * t);

    int idx = xyToIndex(x, y);
    if (idx >= 0) {
      // Color based on tilt magnitude
      float magnitude = sqrt(accelX * accelX + accelY * accelY);
      uint8_t red = magnitude * 255;
      uint8_t green = (1.0 - magnitude) * 255;
      strip.setPixelColor(idx, strip.Color(red, green, 0));
    }
  }

  // Draw arrow at tip
  int tipX = centerX + (int)(accelX * 6);
  int tipY = centerY + (int)(accelY * 3);

  // Arrow head
  for (int dx = -1; dx <= 1; dx++) {
    for (int dy = -1; dy <= 1; dy++) {
      int idx = xyToIndex(tipX + dx, tipY + dy);
      if (idx >= 0) {
        strip.setPixelColor(idx, strip.Color(255, 0, 0));
      }
    }
  }

  strip.show();
}

void resetLiquid() {
  for (int x = 0; x < LED_COLS; x++) {
    for (int y = 0; y < LED_ROWS; y++) {
      liquid[x][y] = 0;
    }
  }
  strip.clear();
  strip.show();
}
