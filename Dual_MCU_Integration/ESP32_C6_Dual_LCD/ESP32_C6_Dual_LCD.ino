/*
 * XIAO ESP32-C6 - Dual Round LCD with Cat Eyes Animation
 *
 * Controls two GC9A01 round LCD displays (240x240) with cat eye animations.
 * Features WiFi 6 AP with web interface for controlling eye movements.
 *
 * Hardware:
 * - Seeed XIAO ESP32-C6
 * - 2× GC9A01 240×240 round LCD displays
 * - Connected to badge via SAO port for power/I2C (optional)
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
 * Features:
 * - Animated cat eyes that look left, right, center, blink
 * - WiFi 6 Access Point
 * - Web interface to control eye direction
 * - Smooth eye movement animations
 * - Configurable eye colors and pupil sizes
 */

#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <SPI.h>
#include <TFT_eSPI.h>

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

// Eye colors
#define EYE_WHITE       0xFFFF  // White
#define EYE_IRIS_COLOR  0x07E0  // Green (can be changed)
#define EYE_PUPIL_COLOR 0x0000  // Black
#define EYE_OUTLINE     0x0000  // Black

// Eye sizes (for 240x240 display)
#define EYE_RADIUS      100     // Outer eye circle
#define IRIS_RADIUS     50      // Iris circle
#define PUPIL_RADIUS    25      // Pupil circle
#define HIGHLIGHT_RADIUS 8      // Light reflection

// Eye positions on screen
#define EYE_CENTER_X    120
#define EYE_CENTER_Y    120

// Pupil movement limits
#define MAX_PUPIL_OFFSET_X  30
#define MAX_PUPIL_OFFSET_Y  20

// ============================================================================
// GLOBAL VARIABLES
// ============================================================================

// TFT displays (we'll manually handle CS)
TFT_eSPI tft = TFT_eSPI();

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
unsigned long blinkInterval = 5000;  // Blink every 5 seconds
int blinkDuration = 150;             // Blink lasts 150ms

// Custom iris color (can be changed via web)
uint16_t irisColor = EYE_IRIS_COLOR;

// ============================================================================
// SETUP
// ============================================================================

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n\n=== XIAO ESP32-C6 Dual Cat Eyes ===");
  Serial.println("Initializing dual round LCD displays...");

  // Generate unique SSID
  uint32_t chipId = 0;
  for(int i=0; i<17; i=i+8) {
    chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  }
  snprintf(apSSID, sizeof(apSSID), "%s-%04X", AP_SSID_PREFIX, (uint16_t)chipId);

  // Initialize SPI
  SPI.begin(TFT_SCK, -1, TFT_MOSI, -1);  // SCK, MISO(-1), MOSI, SS(-1)

  // Initialize display chip select and DC pins
  pinMode(TFT1_CS, OUTPUT);
  pinMode(TFT1_DC, OUTPUT);
  pinMode(TFT1_RST, OUTPUT);
  pinMode(TFT2_CS, OUTPUT);
  pinMode(TFT2_DC, OUTPUT);
  pinMode(TFT2_RST, OUTPUT);

  // Deselect both displays
  digitalWrite(TFT1_CS, HIGH);
  digitalWrite(TFT2_CS, HIGH);

  // Initialize both displays
  Serial.println("Initializing Display 1 (Left Eye)...");
  initDisplay1();

  Serial.println("Initializing Display 2 (Right Eye)...");
  initDisplay2();

  // Draw initial cat eyes
  drawBothEyes();

  // Start WiFi AP
  Serial.println("Starting WiFi Access Point...");
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, gateway, subnet);
  WiFi.softAP(apSSID, apPassword.c_str(), 6, 0, 4);

  delay(500);

  Serial.print("✓ AP Started - SSID: ");
  Serial.println(apSSID);
  Serial.print("  IP: ");
  Serial.println(WiFi.softAPIP());

  // Start DNS server (captive portal)
  dnsServer.start(DNS_PORT, "*", apIP);

  // Setup web server routes
  setupWebServer();
  server.begin();
  Serial.println("✓ Web server started");

  Serial.println("\n=== Cat Eyes Ready! ===");
  Serial.println("Connect to WiFi and browse to:");
  Serial.print("  http://");
  Serial.println(WiFi.softAPIP());
  Serial.println();

  lastBlinkTime = millis();
}

// ============================================================================
// MAIN LOOP
// ============================================================================

void loop() {
  // Handle DNS requests (captive portal)
  dnsServer.processNextRequest();

  // Handle web server requests
  server.handleClient();

  // Update eye animation
  updateEyeAnimation();

  // Handle automatic blinking
  unsigned long currentTime = millis();
  if (currentTime - lastBlinkTime > blinkInterval) {
    blink();
    lastBlinkTime = currentTime;
  }

  delay(20);  // ~50 FPS
}

// ============================================================================
// DISPLAY INITIALIZATION
// ============================================================================

void initDisplay1() {
  // Select display 1
  digitalWrite(TFT1_CS, LOW);

  // Reset display
  digitalWrite(TFT1_RST, LOW);
  delay(20);
  digitalWrite(TFT1_RST, HIGH);
  delay(150);

  // Initialize (basic GC9A01 init sequence)
  // In production, use TFT_eSPI library configuration
  // For now, we'll use manual init
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);

  digitalWrite(TFT1_CS, HIGH);
  Serial.println("  Display 1 initialized");
}

void initDisplay2() {
  // Select display 2
  digitalWrite(TFT2_CS, LOW);

  // Reset display
  digitalWrite(TFT2_RST, LOW);
  delay(20);
  digitalWrite(TFT2_RST, HIGH);
  delay(150);

  // Initialize
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);

  digitalWrite(TFT2_CS, HIGH);
  Serial.println("  Display 2 initialized");
}

// ============================================================================
// EYE DRAWING FUNCTIONS
// ============================================================================

void drawBothEyes() {
  drawEye1();
  drawEye2();
}

void drawEye1() {
  digitalWrite(TFT1_CS, LOW);
  drawEye(pupilOffsetX, pupilOffsetY);
  digitalWrite(TFT1_CS, HIGH);
}

void drawEye2() {
  digitalWrite(TFT2_CS, LOW);
  // Mirror the X offset for right eye to create natural movement
  drawEye(-pupilOffsetX, pupilOffsetY);
  digitalWrite(TFT2_CS, HIGH);
}

void drawEye(int offsetX, int offsetY) {
  // Clear screen
  tft.fillScreen(TFT_BLACK);

  // Draw outer eye (white)
  tft.fillCircle(EYE_CENTER_X, EYE_CENTER_Y, EYE_RADIUS, EYE_WHITE);
  tft.drawCircle(EYE_CENTER_X, EYE_CENTER_Y, EYE_RADIUS, EYE_OUTLINE);

  // Calculate pupil position
  int pupilX = EYE_CENTER_X + offsetX;
  int pupilY = EYE_CENTER_Y + offsetY;

  // Draw iris
  tft.fillCircle(pupilX, pupilY, IRIS_RADIUS, irisColor);

  // Draw pupil
  tft.fillCircle(pupilX, pupilY, PUPIL_RADIUS, EYE_PUPIL_COLOR);

  // Draw highlight (light reflection)
  int highlightX = pupilX - 10;
  int highlightY = pupilY - 10;
  tft.fillCircle(highlightX, highlightY, HIGHLIGHT_RADIUS, EYE_WHITE);
}

void drawClosedEye() {
  // Draw closed eyelid (horizontal ellipse)
  tft.fillScreen(TFT_BLACK);

  // Upper eyelid
  tft.fillEllipse(EYE_CENTER_X, EYE_CENTER_Y, EYE_RADIUS, 15, 0xFFC0);

  // Lower eyelid
  tft.fillEllipse(EYE_CENTER_X, EYE_CENTER_Y + 5, EYE_RADIUS, 10, 0xFFC0);

  // Eyelid crease
  tft.drawLine(EYE_CENTER_X - EYE_RADIUS, EYE_CENTER_Y,
               EYE_CENTER_X + EYE_RADIUS, EYE_CENTER_Y, EYE_OUTLINE);
}

// ============================================================================
// EYE ANIMATION
// ============================================================================

void updateEyeAnimation() {
  // Smooth interpolation toward target position
  if (pupilOffsetX != targetPupilX || pupilOffsetY != targetPupilY) {
    // Move 20% of the distance each frame (ease-out)
    pupilOffsetX += (targetPupilX - pupilOffsetX) * 0.2;
    pupilOffsetY += (targetPupilY - pupilOffsetY) * 0.2;

    // Snap to target if very close
    if (abs(targetPupilX - pupilOffsetX) < 1) pupilOffsetX = targetPupilX;
    if (abs(targetPupilY - pupilOffsetY) < 1) pupilOffsetY = targetPupilY;

    // Redraw eyes
    drawBothEyes();
  }
}

void setEyeDirection(EyeDirection dir) {
  currentDirection = dir;

  switch (dir) {
    case CENTER:
      targetPupilX = 0;
      targetPupilY = 0;
      break;
    case LEFT:
      targetPupilX = -MAX_PUPIL_OFFSET_X;
      targetPupilY = 0;
      break;
    case RIGHT:
      targetPupilX = MAX_PUPIL_OFFSET_X;
      targetPupilY = 0;
      break;
    case UP:
      targetPupilX = 0;
      targetPupilY = -MAX_PUPIL_OFFSET_Y;
      break;
    case DOWN:
      targetPupilX = 0;
      targetPupilY = MAX_PUPIL_OFFSET_Y;
      break;
    case UP_LEFT:
      targetPupilX = -MAX_PUPIL_OFFSET_X;
      targetPupilY = -MAX_PUPIL_OFFSET_Y;
      break;
    case UP_RIGHT:
      targetPupilX = MAX_PUPIL_OFFSET_X;
      targetPupilY = -MAX_PUPIL_OFFSET_Y;
      break;
    case DOWN_LEFT:
      targetPupilX = -MAX_PUPIL_OFFSET_X;
      targetPupilY = MAX_PUPIL_OFFSET_Y;
      break;
    case DOWN_RIGHT:
      targetPupilX = MAX_PUPIL_OFFSET_X;
      targetPupilY = MAX_PUPIL_OFFSET_Y;
      break;
  }

  Serial.print("Eye direction changed to: ");
  Serial.println(dir);
}

void blink() {
  Serial.println("Blinking...");

  // Draw closed eyes
  digitalWrite(TFT1_CS, LOW);
  drawClosedEye();
  digitalWrite(TFT1_CS, HIGH);

  digitalWrite(TFT2_CS, LOW);
  drawClosedEye();
  digitalWrite(TFT2_CS, HIGH);

  delay(blinkDuration);

  // Redraw open eyes
  drawBothEyes();
}

// ============================================================================
// WEB SERVER
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
* {margin:0; padding:0; box-sizing:border-box; -webkit-tap-highlight-color:transparent;}
body {
  font-family: -apple-system, BlinkMacSystemFont, sans-serif;
  background: linear-gradient(135deg, #1a1a2e, #16213e);
  color: #fff;
  padding: 20px;
  min-height: 100vh;
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
}
h1 {
  font-size: 32px;
  margin-bottom: 10px;
  text-align: center;
  background: linear-gradient(90deg, #00f5ff, #ff00ff);
  -webkit-background-clip: text;
  -webkit-text-fill-color: transparent;
}
.subtitle {
  font-size: 14px;
  color: #aaa;
  margin-bottom: 30px;
  text-align: center;
}
.control-grid {
  display: grid;
  grid-template-columns: repeat(3, 80px);
  gap: 10px;
  margin-bottom: 30px;
  padding: 20px;
  background: #222;
  border-radius: 15px;
}
.btn {
  width: 80px;
  height: 80px;
  border: none;
  border-radius: 10px;
  background: linear-gradient(135deg, #00f5ff, #0099ff);
  color: #000;
  font-size: 32px;
  cursor: pointer;
  transition: all 0.2s;
  display: flex;
  align-items: center;
  justify-content: center;
}
.btn:hover {
  transform: scale(1.05);
  box-shadow: 0 0 20px rgba(0, 245, 255, 0.5);
}
.btn:active {
  transform: scale(0.95);
}
.btn.center {
  background: linear-gradient(135deg, #ff00ff, #ff0080);
}
.color-section {
  background: #222;
  padding: 20px;
  border-radius: 15px;
  margin-bottom: 20px;
  text-align: center;
  width: 300px;
}
.color-buttons {
  display: flex;
  gap: 10px;
  justify-content: center;
  flex-wrap: wrap;
  margin-top: 15px;
}
.color-btn {
  width: 50px;
  height: 50px;
  border-radius: 50%;
  border: 3px solid #444;
  cursor: pointer;
  transition: all 0.2s;
}
.color-btn:hover {
  transform: scale(1.1);
  border-color: #fff;
}
.action-btn {
  padding: 15px 40px;
  font-size: 18px;
  font-weight: bold;
  border: none;
  border-radius: 10px;
  background: linear-gradient(135deg, #ffaa00, #ff6600);
  color: #000;
  cursor: pointer;
  margin: 10px;
  transition: all 0.2s;
}
.action-btn:hover {
  transform: scale(1.05);
  box-shadow: 0 0 20px rgba(255, 170, 0, 0.5);
}
.status {
  background: #1a1a1a;
  padding: 10px 20px;
  border-radius: 10px;
  margin-top: 20px;
  font-size: 14px;
  color: #aaa;
}
</style>
</head>
<body>
<h1>🐱 CAT EYES CONTROLLER</h1>
<div class="subtitle">Control the direction of the cat's gaze</div>

<div class="control-grid">
  <button class="btn" onclick="moveEyes('up-left')">↖️</button>
  <button class="btn" onclick="moveEyes('up')">⬆️</button>
  <button class="btn" onclick="moveEyes('up-right')">↗️</button>

  <button class="btn" onclick="moveEyes('left')">⬅️</button>
  <button class="btn center" onclick="moveEyes('center')">⚫</button>
  <button class="btn" onclick="moveEyes('right')">➡️</button>

  <button class="btn" onclick="moveEyes('down-left')">↙️</button>
  <button class="btn" onclick="moveEyes('down')">⬇️</button>
  <button class="btn" onclick="moveEyes('down-right')">↘️</button>
</div>

<div class="color-section">
  <h3>Eye Color</h3>
  <div class="color-buttons">
    <div class="color-btn" style="background:#07E0" onclick="setColor('green')"></div>
    <div class="color-btn" style="background:#001F" onclick="setColor('blue')"></div>
    <div class="color-btn" style="background:#F800" onclick="setColor('red')"></div>
    <div class="color-btn" style="background:#FFE0" onclick="setColor('yellow')"></div>
    <div class="color-btn" style="background:#F81F" onclick="setColor('magenta')"></div>
    <div class="color-btn" style="background:#07FF" onclick="setColor('cyan')"></div>
  </div>
</div>

<button class="action-btn" onclick="triggerBlink()">😉 BLINK</button>

<div class="status" id="status">Ready</div>

<script>
async function moveEyes(direction) {
  document.getElementById('status').textContent = `Looking ${direction}...`;
  try {
    const response = await fetch('/api/eyes', {
      method: 'POST',
      headers: {'Content-Type': 'application/json'},
      body: JSON.stringify({direction: direction})
    });
    if (response.ok) {
      document.getElementById('status').textContent = `✓ Eyes looking ${direction}`;
    }
  } catch (e) {
    document.getElementById('status').textContent = '✗ Error';
  }
  setTimeout(() => document.getElementById('status').textContent = 'Ready', 2000);
}

async function setColor(color) {
  document.getElementById('status').textContent = `Changing to ${color}...`;
  try {
    const response = await fetch('/api/color', {
      method: 'POST',
      headers: {'Content-Type': 'application/json'},
      body: JSON.stringify({color: color})
    });
    if (response.ok) {
      document.getElementById('status').textContent = `✓ Color changed to ${color}`;
    }
  } catch (e) {
    document.getElementById('status').textContent = '✗ Error';
  }
  setTimeout(() => document.getElementById('status').textContent = 'Ready', 2000);
}

async function triggerBlink() {
  document.getElementById('status').textContent = 'Blinking...';
  try {
    await fetch('/api/blink');
    document.getElementById('status').textContent = '✓ Blinked!';
  } catch (e) {
    document.getElementById('status').textContent = '✗ Error';
  }
  setTimeout(() => document.getElementById('status').textContent = 'Ready', 2000);
}
</script>
</body>
</html>
)rawliteral";

  server.send(200, "text/html", html);
}

void handleEyeControl() {
  if (!server.hasArg("plain")) {
    server.send(400, "application/json", "{\"error\":\"No data\"}");
    return;
  }

  String body = server.arg("plain");
  Serial.print("Eye control: ");
  Serial.println(body);

  // Parse direction (simple string parsing)
  if (body.indexOf("center") >= 0) {
    setEyeDirection(CENTER);
  } else if (body.indexOf("up-left") >= 0) {
    setEyeDirection(UP_LEFT);
  } else if (body.indexOf("up-right") >= 0) {
    setEyeDirection(UP_RIGHT);
  } else if (body.indexOf("down-left") >= 0) {
    setEyeDirection(DOWN_LEFT);
  } else if (body.indexOf("down-right") >= 0) {
    setEyeDirection(DOWN_RIGHT);
  } else if (body.indexOf("left") >= 0) {
    setEyeDirection(LEFT);
  } else if (body.indexOf("right") >= 0) {
    setEyeDirection(RIGHT);
  } else if (body.indexOf("up") >= 0) {
    setEyeDirection(UP);
  } else if (body.indexOf("down") >= 0) {
    setEyeDirection(DOWN);
  }

  server.send(200, "application/json", "{\"success\":true}");
}

void handleColorChange() {
  if (!server.hasArg("plain")) {
    server.send(400, "application/json", "{\"error\":\"No data\"}");
    return;
  }

  String body = server.arg("plain");
  Serial.print("Color change: ");
  Serial.println(body);

  // Parse color
  if (body.indexOf("green") >= 0) {
    irisColor = 0x07E0;  // Green
  } else if (body.indexOf("blue") >= 0) {
    irisColor = 0x001F;  // Blue
  } else if (body.indexOf("red") >= 0) {
    irisColor = 0xF800;  // Red
  } else if (body.indexOf("yellow") >= 0) {
    irisColor = 0xFFE0;  // Yellow
  } else if (body.indexOf("magenta") >= 0) {
    irisColor = 0xF81F;  // Magenta
  } else if (body.indexOf("cyan") >= 0) {
    irisColor = 0x07FF;  // Cyan
  }

  // Redraw eyes with new color
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
  json += "\"ap_ip\":\"" + WiFi.softAPIP().toString() + "\",";
  json += "\"clients\":" + String(WiFi.softAPgetStationNum()) + ",";
  json += "\"direction\":\"" + String(currentDirection) + "\",";
  json += "\"uptime\":" + String(millis() / 1000);
  json += "}";

  server.send(200, "application/json", json);
}

void handleNotFound() {
  // Redirect to main page (captive portal)
  server.sendHeader("Location", "/", true);
  server.send(302, "text/plain", "");
}
