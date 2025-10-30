/*
 * XIAO ESP32-C6 - Dual Round LCD with Cat Eyes Animation
 * Using Adafruit GFX Library (ESP32-C6 Compatible)
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
 * - Adafruit GC9A01A (or use Arduino_GFX)
 */

#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Arduino_GFX_Library.h>  // Better GC9A01 support

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
#define TFT_MISO    -1  // Not used for displays

#define SCREEN_WIDTH  240
#define SCREEN_HEIGHT 240

// Create display objects using Arduino_GFX
Arduino_DataBus *bus1 = new Arduino_ESP32SPI(TFT1_DC, TFT1_CS, TFT_SCK, TFT_MOSI, TFT_MISO);
Arduino_DataBus *bus2 = new Arduino_ESP32SPI(TFT2_DC, TFT2_CS, TFT_SCK, TFT_MOSI, TFT_MISO);

Arduino_GFX *display1 = new Arduino_GC9A01(bus1, TFT1_RST, 0 /* rotation */, true /* IPS */);
Arduino_GFX *display2 = new Arduino_GC9A01(bus2, TFT2_RST, 0 /* rotation */, true /* IPS */);

// ============================================================================
// WIFI CONFIGURATION
// ============================================================================

#define AP_SSID_PREFIX  "CatEyes"
#define AP_PASSWORD     "meow1234"
#define WEB_SERVER_PORT 80
#define DNS_PORT        53

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
// GLOBAL VARIABLES
// ============================================================================

// WiFi
char apSSID[32];
String apPassword = AP_PASSWORD;
IPAddress apIP(192, 168, 4, 1);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

WebServer server(WEB_SERVER_PORT);
DNSServer dnsServer;

// Eye animation state
enum EyeDirection {
  CENTER,
  LEFT,
  RIGHT,
  UP,
  DOWN,
  UP_LEFT,
  UP_RIGHT,
  DOWN_LEFT,
  DOWN_RIGHT
};

EyeDirection currentDirection = CENTER;
int pupilOffsetX = 0;
int pupilOffsetY = 0;
int targetPupilX = 0;
int targetPupilY = 0;

// Eye animation parameters
bool isBlinking = false;
unsigned long lastBlinkTime = 0;
unsigned long blinkInterval = 5000;
int blinkDuration = 150;

// Custom iris color
uint16_t irisColor = EYE_IRIS_COLOR;

// ============================================================================
// FUNCTION FORWARD DECLARATIONS
// ============================================================================

void drawBothEyes();
void drawEye(Arduino_GFX *display, int offsetX, int offsetY);
void drawClosedEye(Arduino_GFX *display);
void updateEyeAnimation();
void setEyeDirection(EyeDirection dir);
void blink();
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
  delay(1000);
  Serial.println("\n\n=== XIAO ESP32-C6 Dual Cat Eyes ===");
  Serial.println("Using Adafruit GFX Library");

  // Generate unique SSID
  uint32_t chipId = 0;
  for(int i=0; i<17; i=i+8) {
    chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  }
  snprintf(apSSID, sizeof(apSSID), "%s-%04X", AP_SSID_PREFIX, (uint16_t)chipId);

  Serial.print("AP SSID: ");
  Serial.println(apSSID);

  // Initialize displays
  Serial.println("Initializing Display 1 (Left Eye)...");
  if (!display1->begin()) {
    Serial.println("Display 1 init failed!");
  } else {
    Serial.println("  Display 1 OK");
  }

  Serial.println("Initializing Display 2 (Right Eye)...");
  if (!display2->begin()) {
    Serial.println("Display 2 init failed!");
  } else {
    Serial.println("  Display 2 OK");
  }

  // Draw initial eyes
  drawBothEyes();

  // Start WiFi AP
  Serial.println("Starting WiFi Access Point...");
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, gateway, subnet);
  WiFi.softAP(apSSID, apPassword.c_str(), 6, 0, 4);

  delay(500);

  Serial.print("✓ AP Started - IP: ");
  Serial.println(WiFi.softAPIP());

  // Start DNS server
  dnsServer.start(DNS_PORT, "*", apIP);

  // Setup web server
  setupWebServer();
  server.begin();
  Serial.println("✓ Web server started");

  Serial.println("\n=== Cat Eyes Ready! ===");
  Serial.print("  http://");
  Serial.println(WiFi.softAPIP());

  lastBlinkTime = millis();
}

// ============================================================================
// MAIN LOOP
// ============================================================================

void loop() {
  dnsServer.processNextRequest();
  server.handleClient();
  updateEyeAnimation();

  // Auto blink
  unsigned long currentTime = millis();
  if (currentTime - lastBlinkTime > blinkInterval) {
    blink();
    lastBlinkTime = currentTime;
  }

  delay(20);
}

// ============================================================================
// EYE DRAWING FUNCTIONS
// ============================================================================

void drawBothEyes() {
  drawEye(display1, pupilOffsetX, pupilOffsetY);
  drawEye(display2, -pupilOffsetX, pupilOffsetY);  // Mirror X for right eye
}

void drawEye(Arduino_GFX *display, int offsetX, int offsetY) {
  // Clear screen
  display->fillScreen(0x0000);

  // Draw outer eye (white)
  display->fillCircle(EYE_CENTER_X, EYE_CENTER_Y, EYE_RADIUS, EYE_WHITE);
  display->drawCircle(EYE_CENTER_X, EYE_CENTER_Y, EYE_RADIUS, EYE_OUTLINE);

  // Calculate pupil position
  int pupilX = EYE_CENTER_X + offsetX;
  int pupilY = EYE_CENTER_Y + offsetY;

  // Draw iris
  display->fillCircle(pupilX, pupilY, IRIS_RADIUS, irisColor);

  // Draw pupil
  display->fillCircle(pupilX, pupilY, PUPIL_RADIUS, EYE_PUPIL_COLOR);

  // Draw highlight
  int highlightX = pupilX - 10;
  int highlightY = pupilY - 10;
  display->fillCircle(highlightX, highlightY, HIGHLIGHT_RADIUS, EYE_WHITE);
}

void drawClosedEye(Arduino_GFX *display) {
  display->fillScreen(0x0000);

  // Upper eyelid
  display->fillEllipse(EYE_CENTER_X, EYE_CENTER_Y, EYE_RADIUS, 15, 0xFFC0);

  // Lower eyelid
  display->fillEllipse(EYE_CENTER_X, EYE_CENTER_Y + 5, EYE_RADIUS, 10, 0xFFC0);

  // Eyelid crease
  display->drawLine(EYE_CENTER_X - EYE_RADIUS, EYE_CENTER_Y,
                    EYE_CENTER_X + EYE_RADIUS, EYE_CENTER_Y, EYE_OUTLINE);
}

// ============================================================================
// EYE ANIMATION
// ============================================================================

void updateEyeAnimation() {
  if (pupilOffsetX != targetPupilX || pupilOffsetY != targetPupilY) {
    pupilOffsetX += (targetPupilX - pupilOffsetX) * 0.2;
    pupilOffsetY += (targetPupilY - pupilOffsetY) * 0.2;

    if (abs(targetPupilX - pupilOffsetX) < 1) pupilOffsetX = targetPupilX;
    if (abs(targetPupilY - pupilOffsetY) < 1) pupilOffsetY = targetPupilY;

    drawBothEyes();
  }
}

void setEyeDirection(EyeDirection dir) {
  currentDirection = dir;

  switch (dir) {
    case CENTER:      targetPupilX = 0;                   targetPupilY = 0;                  break;
    case LEFT:        targetPupilX = -MAX_PUPIL_OFFSET_X; targetPupilY = 0;                  break;
    case RIGHT:       targetPupilX = MAX_PUPIL_OFFSET_X;  targetPupilY = 0;                  break;
    case UP:          targetPupilX = 0;                   targetPupilY = -MAX_PUPIL_OFFSET_Y; break;
    case DOWN:        targetPupilX = 0;                   targetPupilY = MAX_PUPIL_OFFSET_Y;  break;
    case UP_LEFT:     targetPupilX = -MAX_PUPIL_OFFSET_X; targetPupilY = -MAX_PUPIL_OFFSET_Y; break;
    case UP_RIGHT:    targetPupilX = MAX_PUPIL_OFFSET_X;  targetPupilY = -MAX_PUPIL_OFFSET_Y; break;
    case DOWN_LEFT:   targetPupilX = -MAX_PUPIL_OFFSET_X; targetPupilY = MAX_PUPIL_OFFSET_Y;  break;
    case DOWN_RIGHT:  targetPupilX = MAX_PUPIL_OFFSET_X;  targetPupilY = MAX_PUPIL_OFFSET_Y;  break;
  }

  Serial.print("Eye direction: ");
  Serial.println(dir);
}

void blink() {
  Serial.println("Blinking...");

  drawClosedEye(display1);
  drawClosedEye(display2);

  delay(blinkDuration);

  drawBothEyes();
}

// ============================================================================
// WEB SERVER - (Same as original, keeping it compact)
// ============================================================================

void setupWebServer() {
  server.on("/", HTTP_GET, handleRoot);
  server.on("/api/eyes", HTTP_POST, handleEyeControl);
  server.on("/api/color", HTTP_POST, handleColorChange);
  server.on("/api/blink", HTTP_GET, handleBlinkCommand);
  server.on("/api/status", HTTP_GET, handleStatus);
  server.onNotFound(handleNotFound);
}

void handleRoot() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1.0">
<title>🐱 Cat Eyes Controller</title>
<style>
* {margin:0; padding:0; box-sizing:border-box;}
body {font-family: -apple-system, sans-serif; background: linear-gradient(135deg, #1a1a2e, #16213e); color: #fff; padding: 20px; min-height: 100vh; display: flex; flex-direction: column; align-items: center; justify-content: center;}
h1 {font-size: 32px; margin-bottom: 10px; text-align: center; background: linear-gradient(90deg, #00f5ff, #ff00ff); -webkit-background-clip: text; -webkit-text-fill-color: transparent;}
.control-grid {display: grid; grid-template-columns: repeat(3, 80px); gap: 10px; margin-bottom: 30px; padding: 20px; background: #222; border-radius: 15px;}
.btn {width: 80px; height: 80px; border: none; border-radius: 10px; background: linear-gradient(135deg, #00f5ff, #0099ff); color: #000; font-size: 32px; cursor: pointer;}
.btn:hover {transform: scale(1.05);}
.btn.center {background: linear-gradient(135deg, #ff00ff, #ff0080);}
.color-section {background: #222; padding: 20px; border-radius: 15px; margin-bottom: 20px; text-align: center; width: 300px;}
.color-buttons {display: flex; gap: 10px; justify-content: center; flex-wrap: wrap; margin-top: 15px;}
.color-btn {width: 50px; height: 50px; border-radius: 50%; border: 3px solid #444; cursor: pointer;}
.action-btn {padding: 15px 40px; font-size: 18px; font-weight: bold; border: none; border-radius: 10px; background: linear-gradient(135deg, #ffaa00, #ff6600); color: #000; cursor: pointer; margin: 10px;}
.status {background: #1a1a1a; padding: 10px 20px; border-radius: 10px; margin-top: 20px; font-size: 14px;}
</style>
</head>
<body>
<h1>🐱 CAT EYES CONTROLLER</h1>
<div class="control-grid">
  <button class="btn" onclick="move('up-left')">↖️</button>
  <button class="btn" onclick="move('up')">⬆️</button>
  <button class="btn" onclick="move('up-right')">↗️</button>
  <button class="btn" onclick="move('left')">⬅️</button>
  <button class="btn center" onclick="move('center')">⚫</button>
  <button class="btn" onclick="move('right')">➡️</button>
  <button class="btn" onclick="move('down-left')">↙️</button>
  <button class="btn" onclick="move('down')">⬇️</button>
  <button class="btn" onclick="move('down-right')">↘️</button>
</div>
<div class="color-section">
  <h3>Eye Color</h3>
  <div class="color-buttons">
    <div class="color-btn" style="background:#0f0" onclick="setColor('green')"></div>
    <div class="color-btn" style="background:#00f" onclick="setColor('blue')"></div>
    <div class="color-btn" style="background:#f00" onclick="setColor('red')"></div>
    <div class="color-btn" style="background:#ff0" onclick="setColor('yellow')"></div>
    <div class="color-btn" style="background:#f0f" onclick="setColor('magenta')"></div>
    <div class="color-btn" style="background:#0ff" onclick="setColor('cyan')"></div>
  </div>
</div>
<button class="action-btn" onclick="fetch('/api/blink')">😉 BLINK</button>
<div class="status" id="s">Ready</div>
<script>
async function move(d) {
  document.getElementById('s').textContent = 'Looking ' + d;
  await fetch('/api/eyes', {method: 'POST', body: JSON.stringify({direction: d})});
  setTimeout(() => document.getElementById('s').textContent = 'Ready', 2000);
}
async function setColor(c) {
  document.getElementById('s').textContent = 'Color: ' + c;
  await fetch('/api/color', {method: 'POST', body: JSON.stringify({color: c})});
  setTimeout(() => document.getElementById('s').textContent = 'Ready', 2000);
}
</script>
</body>
</html>
)rawliteral";

  server.send(200, "text/html", html);
}

void handleEyeControl() {
  String body = server.arg("plain");
  Serial.println("Eye control: " + body);

  if (body.indexOf("center") >= 0)         setEyeDirection(CENTER);
  else if (body.indexOf("up-left") >= 0)   setEyeDirection(UP_LEFT);
  else if (body.indexOf("up-right") >= 0)  setEyeDirection(UP_RIGHT);
  else if (body.indexOf("down-left") >= 0) setEyeDirection(DOWN_LEFT);
  else if (body.indexOf("down-right") >= 0) setEyeDirection(DOWN_RIGHT);
  else if (body.indexOf("left") >= 0)      setEyeDirection(LEFT);
  else if (body.indexOf("right") >= 0)     setEyeDirection(RIGHT);
  else if (body.indexOf("up") >= 0)        setEyeDirection(UP);
  else if (body.indexOf("down") >= 0)      setEyeDirection(DOWN);

  server.send(200, "application/json", "{\"success\":true}");
}

void handleColorChange() {
  String body = server.arg("plain");

  if (body.indexOf("green") >= 0)        irisColor = 0x07E0;
  else if (body.indexOf("blue") >= 0)    irisColor = 0x001F;
  else if (body.indexOf("red") >= 0)     irisColor = 0xF800;
  else if (body.indexOf("yellow") >= 0)  irisColor = 0xFFE0;
  else if (body.indexOf("magenta") >= 0) irisColor = 0xF81F;
  else if (body.indexOf("cyan") >= 0)    irisColor = 0x07FF;

  drawBothEyes();
  server.send(200, "application/json", "{\"success\":true}");
}

void handleBlinkCommand() {
  blink();
  server.send(200, "application/json", "{\"success\":true}");
}

void handleStatus() {
  String json = "{";
  json += "\"ap_ssid\":\"" + String(apSSID) + "\",";
  json += "\"clients\":" + String(WiFi.softAPgetStationNum());
  json += "}";
  server.send(200, "application/json", json);
}

void handleNotFound() {
  server.sendHeader("Location", "/", true);
  server.send(302, "text/plain", "");
}
