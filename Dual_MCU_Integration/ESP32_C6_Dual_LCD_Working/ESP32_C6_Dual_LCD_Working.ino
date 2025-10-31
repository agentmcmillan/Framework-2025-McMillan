/*
 * XIAO ESP32-C6 - Dual Round LCD with Cat Eyes Animation
 * Using Arduino GFX + Standard SPI (ESP32-C6 Compatible)
 *
 * Controls two GC9A01 round LCD displays (240x240) with cat eye animations.
 * Features WiFi 6 AP with web interface for controlling eye movements.
 *
 * Hardware:
 * - Seeed XIAO ESP32-C6
 * - 2× GC9A01 240×240 round LCD displays
 *
 * Pin Assignments:
 * Display 1 (Left Eye):
 *   - CS:   GPIO2 (D0)
 *   - DC:   GPIO3 (D1)
 *   - RST:  GPIO4 (D2)
 *   - MOSI: GPIO21 (shared SPI)
 *   - SCK:  GPIO20 (shared SPI)
 *
 * Display 2 (Right Eye):
 *   - CS:   GPIO5 (D3)
 *   - DC:   GPIO6 (D4)
 *   - RST:  GPIO7 (D5)
 *   - MOSI: GPIO21 (shared SPI, same as Display 1)
 *   - SCK:  GPIO20 (shared SPI, same as Display 1)
 *
 * Required Libraries:
 * - Adafruit GFX Library
 * - Adafruit BusIO
 * - Adafruit ST7735 and ST7789 Library (for GC9A01 support)
 */

#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>  // GC9A01 uses similar protocol

// ============================================================================
// DISPLAY CONFIGURATION
// ============================================================================

// Display pins
#define TFT1_CS     2   // Left eye chip select
#define TFT1_DC     3   // Left eye data/command
#define TFT1_RST    4   // Left eye reset

#define TFT2_CS     5   // Right eye chip select
#define TFT2_DC     6   // Right eye data/command
#define TFT2_RST    7   // Right eye reset

#define TFT_MOSI    21  // Shared SPI MOSI
#define TFT_SCK     20  // Shared SPI SCK

#define SCREEN_WIDTH  240
#define SCREEN_HEIGHT 240

// Create display objects using Adafruit ST7789 (compatible with GC9A01)
Adafruit_ST7789 display1 = Adafruit_ST7789(TFT1_CS, TFT1_DC, TFT1_RST);
Adafruit_ST7789 display2 = Adafruit_ST7789(TFT2_CS, TFT2_DC, TFT2_RST);

// ============================================================================
// WIFI CONFIGURATION
// ============================================================================

#define AP_SSID_PREFIX  "CatEyes"
#define AP_PASSWORD     "meow1234"
#define WEB_SERVER_PORT 80
#define DNS_PORT        53

WebServer server(WEB_SERVER_PORT);
DNSServer dnsServer;

String ap_ssid;

// ============================================================================
// CAT EYE PARAMETERS
// ============================================================================

// Eye colors (RGB565 format)
#define EYE_WHITE       0xFFFF  // White
#define EYE_IRIS_COLOR  0x07E0  // Green
#define EYE_PUPIL_COLOR 0x0000  // Black
#define EYE_OUTLINE     0x0000  // Black

// Eye sizes
#define EYE_RADIUS      100
#define IRIS_RADIUS     50
#define PUPIL_RADIUS    25
#define HIGHLIGHT_RADIUS 8

// Eye positions
#define EYE_CENTER_X    120
#define EYE_CENTER_Y    120

// Pupil movement limits
#define MAX_PUPIL_OFFSET_X  30
#define MAX_PUPIL_OFFSET_Y  20

// ============================================================================
// ANIMATION PARAMETERS
// ============================================================================

#define BLINK_DURATION 150       // Blink animation duration (ms)
#define BLINK_INTERVAL_MIN 3000  // Minimum time between blinks (ms)
#define BLINK_INTERVAL_MAX 7000  // Maximum time between blinks (ms)
#define ANIMATION_STEPS 10       // Number of steps in eye movement animation

// ============================================================================
// GLOBAL STATE
// ============================================================================

// Current pupil positions
int leftPupilX = 0;
int leftPupilY = 0;
int rightPupilX = 0;
int rightPupilY = 0;

// Target pupil positions
int targetLeftX = 0;
int targetLeftY = 0;
int targetRightX = 0;
int targetRightY = 0;

// Current iris color
uint16_t currentIrisColor = EYE_IRIS_COLOR;

// Auto-blink timing
unsigned long lastBlinkTime = 0;
unsigned long nextBlinkInterval = 5000;

// Function forward declarations
void setupWebServer();
void handleRoot();
void handleEyeControl();
void handleColorChange();
void handleBlinkCommand();
void handleStatus();
void handleNotFound();

// ============================================================================
// SETUP
// ============================================================================

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("\n\n=== ESP32-C6 Dual Cat Eyes ===\n");

  // Initialize SPI
  SPI.begin(TFT_SCK, -1, TFT_MOSI, -1);  // SCK, MISO, MOSI, SS

  // Initialize displays
  Serial.println("Initializing displays...");

  display1.init(SCREEN_WIDTH, SCREEN_HEIGHT, SPI_MODE0);
  display1.setRotation(0);
  Serial.println("Display 1 initialized");

  display2.init(SCREEN_WIDTH, SCREEN_HEIGHT, SPI_MODE0);
  display2.setRotation(0);
  Serial.println("Display 2 initialized");

  // Draw initial eyes
  Serial.println("Drawing initial eyes...");
  drawEye(&display1, leftPupilX, leftPupilY);
  drawEye(&display2, rightPupilX, rightPupilY);
  Serial.println("Done!\n");

  // Setup WiFi AP
  setupWiFiAP();

  // Setup web server
  setupWebServer();

  // Initialize random seed for blinking
  randomSeed(analogRead(0));
  nextBlinkInterval = random(BLINK_INTERVAL_MIN, BLINK_INTERVAL_MAX);
  lastBlinkTime = millis();

  Serial.println("Ready! Connect to WiFi and go to http://192.168.4.1\n");
}

// ============================================================================
// MAIN LOOP
// ============================================================================

void loop() {
  // Handle DNS requests (for captive portal)
  dnsServer.processNextRequest();

  // Handle web server requests
  server.handleClient();

  // Auto-blink
  if (millis() - lastBlinkTime > nextBlinkInterval) {
    blinkEyes();
    lastBlinkTime = millis();
    nextBlinkInterval = random(BLINK_INTERVAL_MIN, BLINK_INTERVAL_MAX);
  }
}

// ============================================================================
// WIFI SETUP
// ============================================================================

void setupWiFiAP() {
  Serial.println("Starting WiFi AP...");

  // Generate unique SSID using MAC address
  uint8_t mac[6];
  WiFi.macAddress(mac);
  ap_ssid = String(AP_SSID_PREFIX) + "-" +
            String(mac[4], HEX) + String(mac[5], HEX);
  ap_ssid.toUpperCase();

  // Start AP
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ap_ssid.c_str(), AP_PASSWORD);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP SSID: ");
  Serial.println(ap_ssid);
  Serial.print("AP Password: ");
  Serial.println(AP_PASSWORD);
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Start DNS server for captive portal
  dnsServer.start(DNS_PORT, "*", IP);

  Serial.println("WiFi AP started!\n");
}

// ============================================================================
// DRAWING FUNCTIONS
// ============================================================================

void drawEye(Adafruit_ST7789 *display, int pupilOffsetX, int pupilOffsetY) {
  // Clear screen
  display->fillScreen(0x0000);

  // Draw eye white (outer circle)
  display->fillCircle(EYE_CENTER_X, EYE_CENTER_Y, EYE_RADIUS, EYE_WHITE);

  // Calculate pupil position
  int pupilX = EYE_CENTER_X + pupilOffsetX;
  int pupilY = EYE_CENTER_Y + pupilOffsetY;

  // Draw iris
  display->fillCircle(pupilX, pupilY, IRIS_RADIUS, currentIrisColor);

  // Draw pupil
  display->fillCircle(pupilX, pupilY, PUPIL_RADIUS, EYE_PUPIL_COLOR);

  // Draw highlight (makes it look more realistic)
  int highlightX = pupilX - 10;
  int highlightY = pupilY - 10;
  display->fillCircle(highlightX, highlightY, HIGHLIGHT_RADIUS, EYE_WHITE);

  // Draw outline
  display->drawCircle(EYE_CENTER_X, EYE_CENTER_Y, EYE_RADIUS, EYE_OUTLINE);
}

void animateEyeMovement() {
  // Smoothly animate from current to target position
  for (int step = 0; step <= ANIMATION_STEPS; step++) {
    float progress = (float)step / ANIMATION_STEPS;

    // Ease in-out cubic
    float ease = progress < 0.5
      ? 4 * progress * progress * progress
      : 1 - pow(-2 * progress + 2, 3) / 2;

    // Interpolate positions
    int newLeftX = leftPupilX + (targetLeftX - leftPupilX) * ease;
    int newLeftY = leftPupilY + (targetLeftY - leftPupilY) * ease;
    int newRightX = rightPupilX + (targetRightX - rightPupilX) * ease;
    int newRightY = rightPupilY + (targetRightY - rightPupilY) * ease;

    // Draw both eyes
    drawEye(&display1, newLeftX, newLeftY);
    drawEye(&display2, newRightX, newRightY);

    delay(20);  // Small delay for smooth animation
  }

  // Update current positions
  leftPupilX = targetLeftX;
  leftPupilY = targetLeftY;
  rightPupilX = targetRightX;
  rightPupilY = targetRightY;
}

void lookDirection(String direction) {
  Serial.print("Looking: ");
  Serial.println(direction);

  // Set target positions based on direction
  if (direction == "center") {
    targetLeftX = 0;
    targetLeftY = 0;
    targetRightX = 0;
    targetRightY = 0;
  }
  else if (direction == "left") {
    targetLeftX = -MAX_PUPIL_OFFSET_X;
    targetLeftY = 0;
    targetRightX = -MAX_PUPIL_OFFSET_X;
    targetRightY = 0;
  }
  else if (direction == "right") {
    targetLeftX = MAX_PUPIL_OFFSET_X;
    targetLeftY = 0;
    targetRightX = MAX_PUPIL_OFFSET_X;
    targetRightY = 0;
  }
  else if (direction == "up") {
    targetLeftX = 0;
    targetLeftY = -MAX_PUPIL_OFFSET_Y;
    targetRightX = 0;
    targetRightY = -MAX_PUPIL_OFFSET_Y;
  }
  else if (direction == "down") {
    targetLeftX = 0;
    targetLeftY = MAX_PUPIL_OFFSET_Y;
    targetRightX = 0;
    targetRightY = MAX_PUPIL_OFFSET_Y;
  }
  else if (direction == "up-left") {
    targetLeftX = -MAX_PUPIL_OFFSET_X * 0.7;
    targetLeftY = -MAX_PUPIL_OFFSET_Y * 0.7;
    targetRightX = -MAX_PUPIL_OFFSET_X * 0.7;
    targetRightY = -MAX_PUPIL_OFFSET_Y * 0.7;
  }
  else if (direction == "up-right") {
    targetLeftX = MAX_PUPIL_OFFSET_X * 0.7;
    targetLeftY = -MAX_PUPIL_OFFSET_Y * 0.7;
    targetRightX = MAX_PUPIL_OFFSET_X * 0.7;
    targetRightY = -MAX_PUPIL_OFFSET_Y * 0.7;
  }
  else if (direction == "down-left") {
    targetLeftX = -MAX_PUPIL_OFFSET_X * 0.7;
    targetLeftY = MAX_PUPIL_OFFSET_Y * 0.7;
    targetRightX = -MAX_PUPIL_OFFSET_X * 0.7;
    targetRightY = MAX_PUPIL_OFFSET_Y * 0.7;
  }
  else if (direction == "down-right") {
    targetLeftX = MAX_PUPIL_OFFSET_X * 0.7;
    targetLeftY = MAX_PUPIL_OFFSET_Y * 0.7;
    targetRightX = MAX_PUPIL_OFFSET_X * 0.7;
    targetRightY = MAX_PUPIL_OFFSET_Y * 0.7;
  }

  animateEyeMovement();
}

void blinkEyes() {
  Serial.println("Blinking...");

  // Blink is simulated by drawing progressively smaller vertical eyes
  for (int i = 0; i < 5; i++) {
    int closeAmount = (i < 3) ? i * 30 : (5 - i) * 30;

    // Draw partially closed eyes (simplified - just redraw with less vertical space)
    drawEye(&display1, leftPupilX, leftPupilY);
    drawEye(&display2, rightPupilX, rightPupilY);

    // Draw black bars at top and bottom to simulate eyelid
    display1.fillRect(0, 0, SCREEN_WIDTH, closeAmount, 0x0000);
    display1.fillRect(0, SCREEN_HEIGHT - closeAmount, SCREEN_WIDTH, closeAmount, 0x0000);
    display2.fillRect(0, 0, SCREEN_WIDTH, closeAmount, 0x0000);
    display2.fillRect(0, SCREEN_HEIGHT - closeAmount, SCREEN_WIDTH, closeAmount, 0x0000);

    delay(30);
  }

  // Redraw full eyes
  drawEye(&display1, leftPupilX, leftPupilY);
  drawEye(&display2, rightPupilX, rightPupilY);
}

// ============================================================================
// WEB SERVER SETUP
// ============================================================================

void setupWebServer() {
  server.on("/", handleRoot);
  server.on("/control", handleEyeControl);
  server.on("/color", handleColorChange);
  server.on("/blink", handleBlinkCommand);
  server.on("/status", handleStatus);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("Web server started");
}

// ============================================================================
// WEB SERVER HANDLERS
// ============================================================================

void handleRoot() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Cat Eyes Control</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            max-width: 600px;
            margin: 0 auto;
            padding: 20px;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            color: white;
        }
        h1 {
            text-align: center;
            margin-bottom: 10px;
        }
        .subtitle {
            text-align: center;
            opacity: 0.9;
            margin-bottom: 30px;
        }
        .control-grid {
            display: grid;
            grid-template-columns: repeat(3, 1fr);
            gap: 10px;
            margin-bottom: 20px;
            background: rgba(255,255,255,0.1);
            padding: 20px;
            border-radius: 15px;
        }
        button {
            padding: 20px;
            font-size: 24px;
            border: none;
            border-radius: 10px;
            cursor: pointer;
            background: white;
            transition: all 0.2s;
        }
        button:hover {
            transform: scale(1.05);
            box-shadow: 0 5px 15px rgba(0,0,0,0.3);
        }
        button:active {
            transform: scale(0.95);
        }
        .center-btn {
            grid-column: 2;
            grid-row: 2;
            background: #4CAF50;
            color: white;
        }
        .controls {
            background: rgba(255,255,255,0.1);
            padding: 20px;
            border-radius: 15px;
            margin-bottom: 20px;
        }
        select, .blink-btn {
            width: 100%;
            padding: 15px;
            font-size: 16px;
            border: none;
            border-radius: 10px;
            margin-bottom: 10px;
        }
        .blink-btn {
            background: #FF6B6B;
            color: white;
            cursor: pointer;
        }
        .status {
            background: rgba(255,255,255,0.1);
            padding: 15px;
            border-radius: 10px;
            text-align: center;
        }
    </style>
</head>
<body>
    <h1>👁️ Cat Eyes Control 👁️</h1>
    <p class="subtitle">ESP32-C6 Dual Display Controller</p>

    <div class="control-grid">
        <button onclick="look('up-left')">↖️</button>
        <button onclick="look('up')">⬆️</button>
        <button onclick="look('up-right')">↗️</button>
        <button onclick="look('left')">⬅️</button>
        <button onclick="look('center')" class="center-btn">👁️</button>
        <button onclick="look('right')">➡️</button>
        <button onclick="look('down-left')">↙️</button>
        <button onclick="look('down')">⬇️</button>
        <button onclick="look('down-right')">↘️</button>
    </div>

    <div class="controls">
        <select id="colorSelect" onchange="changeColor()">
            <option value="green">🟢 Green Eyes</option>
            <option value="blue">🔵 Blue Eyes</option>
            <option value="red">🔴 Red Eyes</option>
            <option value="yellow">🟡 Yellow Eyes</option>
            <option value="magenta">🟣 Magenta Eyes</option>
            <option value="cyan">🔷 Cyan Eyes</option>
        </select>
        <button class="blink-btn" onclick="blink()">😉 Blink</button>
    </div>

    <div class="status" id="status">Ready</div>

    <script>
        function look(direction) {
            fetch('/control?dir=' + direction)
                .then(r => r.text())
                .then(t => document.getElementById('status').innerText = t);
        }

        function changeColor() {
            const color = document.getElementById('colorSelect').value;
            fetch('/color?c=' + color)
                .then(r => r.text())
                .then(t => document.getElementById('status').innerText = t);
        }

        function blink() {
            fetch('/blink')
                .then(r => r.text())
                .then(t => document.getElementById('status').innerText = t);
        }
    </script>
</body>
</html>
)rawliteral";

  server.send(200, "text/html", html);
}

void handleEyeControl() {
  if (server.hasArg("dir")) {
    String direction = server.arg("dir");
    lookDirection(direction);
    server.send(200, "text/plain", "Looking " + direction);
  } else {
    server.send(400, "text/plain", "Missing direction parameter");
  }
}

void handleColorChange() {
  if (server.hasArg("c")) {
    String color = server.arg("c");

    if (color == "green") currentIrisColor = 0x07E0;
    else if (color == "blue") currentIrisColor = 0x001F;
    else if (color == "red") currentIrisColor = 0xF800;
    else if (color == "yellow") currentIrisColor = 0xFFE0;
    else if (color == "magenta") currentIrisColor = 0xF81F;
    else if (color == "cyan") currentIrisColor = 0x07FF;

    // Redraw eyes with new color
    drawEye(&display1, leftPupilX, leftPupilY);
    drawEye(&display2, rightPupilX, rightPupilY);

    server.send(200, "text/plain", "Changed to " + color + " eyes");
  } else {
    server.send(400, "text/plain", "Missing color parameter");
  }
}

void handleBlinkCommand() {
  blinkEyes();
  server.send(200, "text/plain", "Blinked!");
}

void handleStatus() {
  String status = "OK";
  server.send(200, "text/plain", status);
}

void handleNotFound() {
  // Redirect all unknown requests to root (captive portal behavior)
  server.sendHeader("Location", "/", true);
  server.send(302, "text/plain", "");
}
