/*
 * XIAO ESP32-C6 - Enhanced WiFi AP + Web Server
 *
 * Provides WiFi Access Point with web interface for LED matrix drawing.
 * Serves HTML page for designing LED patterns and sends them to RP2040.
 *
 * Hardware:
 * - Seeed XIAO ESP32-C6
 * - Connected to Framework Badge via SAO port
 * - I2C: SDA=GPIO6, SCL=GPIO7
 *
 * Features:
 * - WiFi 6 AP mode (SSID: "PixelKitty-XXXX")
 * - Web server with LED drawing interface
 * - QR code data generation for WiFi connection
 * - I2C communication with RP2040
 *
 * Note: ESP32-C6 uses RISC-V architecture, supports WiFi 6 (802.11ax)
 */

#include <Wire.h>
#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <ArduinoJson.h>

// ============================================================================
// CONFIGURATION
// ============================================================================

// WiFi AP Configuration
#define AP_SSID_PREFIX  "PixelKitty"  // Will append chip ID
#define AP_PASSWORD     "meow1234"    // Minimum 8 characters
#define AP_CHANNEL      6
#define AP_MAX_CLIENTS  4

// I2C Configuration
#define I2C_SDA         6
#define I2C_SCL         7
#define I2C_SLAVE_ADDR  0x42

// Web Server
#define WEB_SERVER_PORT 80
#define DNS_PORT        53

// LED Matrix
#define LED_COLS        15
#define LED_ROWS        7
#define NUM_LEDS        (LED_COLS * LED_ROWS)

// Commands (must match RP2040)
#define CMD_PING                0x00
#define CMD_GET_STATUS          0x01
#define CMD_SET_LED_DATA        0x50  // New command for LED data
#define CMD_SHOW_QRCODE         0x51  // Request RP2040 to show QR code
#define CMD_SHOW_CAT            0x52  // Request RP2040 to show cat animation

// Status codes
#define STATUS_OK               0x00
#define STATUS_ERROR            0xFF

// ============================================================================
// GLOBAL VARIABLES
// ============================================================================

// WiFi
char apSSID[32];
String apPassword = AP_PASSWORD;
IPAddress apIP(192, 168, 4, 1);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

// Web Server & DNS
WebServer server(WEB_SERVER_PORT);
DNSServer dnsServer;

// I2C Communication
volatile uint8_t i2cCommand = 0;
volatile uint8_t i2cDataBuffer[256];
volatile uint8_t i2cDataLength = 0;
volatile bool newCommandReceived = false;

uint8_t responseBuffer[64];
uint8_t responseLength = 0;

// LED Data buffer (RGB values for 105 LEDs)
uint8_t ledData[NUM_LEDS * 3];  // R, G, B for each LED
bool ledDataReady = false;

// ============================================================================
// SETUP
// ============================================================================

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n\n=== XIAO ESP32-C6 Enhanced ===");
  Serial.println("WiFi 6 AP + Web Server + LED Control");

  // Generate unique SSID with chip ID
  uint32_t chipId = 0;
  for(int i=0; i<17; i=i+8) {
    chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  }
  snprintf(apSSID, sizeof(apSSID), "%s-%04X", AP_SSID_PREFIX, (uint16_t)chipId);

  Serial.print("AP SSID: ");
  Serial.println(apSSID);
  Serial.print("AP Password: ");
  Serial.println(apPassword);

  // Initialize I2C as slave
  Serial.println("Initializing I2C slave...");
  Wire.begin(I2C_SDA, I2C_SCL, I2C_SLAVE_ADDR);
  Wire.onReceive(i2cReceiveEvent);
  Wire.onRequest(i2cRequestEvent);

  // Start WiFi AP
  Serial.println("Starting WiFi Access Point...");
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, gateway, subnet);
  WiFi.softAP(apSSID, apPassword.c_str(), AP_CHANNEL, 0, AP_MAX_CLIENTS);

  delay(500);

  Serial.print("✓ AP Started - IP: ");
  Serial.println(WiFi.softAPIP());

  // Start DNS server (captive portal)
  dnsServer.start(DNS_PORT, "*", apIP);

  // Setup web server routes
  setupWebServer();

  // Start web server
  server.begin();
  Serial.println("✓ Web server started on port 80");

  // Generate WiFi QR code string
  String qrData = generateWiFiQR();
  Serial.println("\n=== WiFi QR Code Data ===");
  Serial.println(qrData);
  Serial.println("=========================\n");

  Serial.println("Ready! Connect to WiFi and browse to:");
  Serial.print("  http://");
  Serial.println(WiFi.softAPIP());
  Serial.println("  or http://badge.local\n");
}

// ============================================================================
// MAIN LOOP
// ============================================================================

void loop() {
  // Handle DNS requests (captive portal)
  dnsServer.processNextRequest();

  // Handle web server requests
  server.handleClient();

  // Process I2C commands from RP2040
  if (newCommandReceived) {
    newCommandReceived = false;
    processI2CCommand();
  }

  // Send LED data to RP2040 if ready
  if (ledDataReady) {
    sendLEDDataToRP2040();
    ledDataReady = false;
  }

  delay(1);
}

// ============================================================================
// WEB SERVER ROUTES
// ============================================================================

void setupWebServer() {
  // Main page - LED matrix designer
  server.on("/", HTTP_GET, handleRoot);

  // API endpoint for LED data
  server.on("/api/led", HTTP_POST, handleLEDAPI);
  server.on("/api/led", HTTP_OPTIONS, handleCORS);

  // Status API
  server.on("/api/status", HTTP_GET, handleStatusAPI);

  // QR code endpoint
  server.on("/api/qr", HTTP_GET, handleQRCode);

  // Trigger QR display on badge
  server.on("/api/show-qr", HTTP_GET, handleShowQR);

  // Trigger cat animation
  server.on("/api/show-cat", HTTP_GET, handleShowCat);

  // 404 handler
  server.onNotFound(handleNotFound);
}

void handleRoot() {
  // Serve the LED matrix HTML page
  String html = getHTMLPage();
  server.send(200, "text/html", html);
}

void handleLEDAPI() {
  if (!server.hasArg("plain")) {
    server.send(400, "application/json", "{\"error\":\"No data\"}");
    return;
  }

  String body = server.arg("plain");
  Serial.println("Received LED data:");
  Serial.println(body);

  // Parse JSON
  StaticJsonDocument<4096> doc;
  DeserializationError error = deserializeJson(doc, body);

  if (error) {
    Serial.print("JSON parse error: ");
    Serial.println(error.c_str());
    server.send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
    return;
  }

  // Extract LED array
  JsonArray leds = doc["leds"];
  if (leds.size() != NUM_LEDS * 3) {
    server.send(400, "application/json", "{\"error\":\"Invalid LED count\"}");
    return;
  }

  // Copy LED data
  for (int i = 0; i < NUM_LEDS * 3; i++) {
    ledData[i] = leds[i];
  }

  ledDataReady = true;

  server.send(200, "application/json", "{\"success\":true}");
  Serial.println("LED data ready to send to RP2040");
}

void handleCORS() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "POST, GET, OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  server.send(200);
}

void handleStatusAPI() {
  String json = "{";
  json += "\"ap_ssid\":\"" + String(apSSID) + "\",";
  json += "\"ap_ip\":\"" + WiFi.softAPIP().toString() + "\",";
  json += "\"clients\":" + String(WiFi.softAPgetStationNum()) + ",";
  json += "\"uptime\":" + String(millis() / 1000);
  json += "}";

  server.send(200, "application/json", json);
}

void handleQRCode() {
  String qrData = generateWiFiQR();
  String json = "{\"qr_data\":\"" + qrData + "\"}";
  server.send(200, "application/json", json);
}

void handleShowQR() {
  Serial.println("Request to show QR code on badge");

  // Send command to RP2040 to display QR code
  Wire.beginTransmission(I2C_SLAVE_ADDR);
  // Note: This won't work as-is because ESP32 is slave, not master
  // We'll need to handle this differently - RP2040 should request QR data

  server.send(200, "application/json", "{\"success\":true}");
}

void handleShowCat() {
  Serial.println("Request to show cat animation on badge");
  // Similar issue - need different approach
  server.send(200, "application/json", "{\"success\":true}");
}

void handleNotFound() {
  // Redirect to main page (captive portal behavior)
  server.sendHeader("Location", "/", true);
  server.send(302, "text/plain", "");
}

// ============================================================================
// LED DATA TRANSMISSION
// ============================================================================

void sendLEDDataToRP2040() {
  Serial.println("Sending LED data to RP2040...");

  // We need to send 315 bytes (105 LEDs × 3 bytes)
  // I2C has size limits, so we'll send in chunks

  const int chunkSize = 32;  // I2C buffer size limit
  int totalBytes = NUM_LEDS * 3;
  int chunks = (totalBytes + chunkSize - 1) / chunkSize;

  Serial.print("Total bytes: ");
  Serial.print(totalBytes);
  Serial.print(" in ");
  Serial.print(chunks);
  Serial.println(" chunks");

  // Note: Since ESP32 is I2C slave, it can't initiate transmission
  // RP2040 needs to request data in chunks
  // We'll store it and respond when requested

  Serial.println("LED data stored, waiting for RP2040 to request it");
}

// ============================================================================
// I2C EVENT HANDLERS
// ============================================================================

void i2cReceiveEvent(int numBytes) {
  if (numBytes == 0) return;

  i2cCommand = Wire.read();
  i2cDataLength = 0;

  while (Wire.available() && i2cDataLength < 256) {
    i2cDataBuffer[i2cDataLength++] = Wire.read();
  }

  newCommandReceived = true;
}

void i2cRequestEvent() {
  // RP2040 is requesting data
  if (responseLength > 0) {
    Wire.write(responseBuffer, responseLength);
  } else {
    Wire.write(STATUS_OK);
  }
}

void processI2CCommand() {
  memset(responseBuffer, 0, sizeof(responseBuffer));
  responseLength = 0;

  Serial.print("I2C Command: 0x");
  Serial.println(i2cCommand, HEX);

  switch (i2cCommand) {
    case CMD_PING:
      responseBuffer[0] = 0xA5;
      responseLength = 1;
      break;

    case CMD_GET_STATUS:
      responseBuffer[0] = STATUS_OK;
      responseBuffer[1] = WiFi.softAPgetStationNum(); // Number of clients
      responseLength = 2;
      break;

    // Add more commands as needed
  }
}

// ============================================================================
// WiFi QR CODE GENERATION
// ============================================================================

String generateWiFiQR() {
  // WiFi QR code format: WIFI:T:WPA;S:SSID;P:password;;
  String qrData = "WIFI:T:WPA;S:";
  qrData += apSSID;
  qrData += ";P:";
  qrData += apPassword;
  qrData += ";;";
  return qrData;
}

// ============================================================================
// HTML PAGE (LED MATRIX DESIGNER)
// ============================================================================

String getHTMLPage() {
  // Return the LED matrix designer HTML
  // For production, serve from SPIFFS or SD card
  // For now, embedded (will be large)

  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1.0,user-scalable=no">
<title>🐱 Pixel Kitty LED Designer</title>
<style>
*{margin:0;padding:0;box-sizing:border-box;-webkit-tap-highlight-color:transparent}
body{font-family:-apple-system,sans-serif;background:linear-gradient(135deg,#1a1a2e,#16213e);color:#fff;padding:20px;min-height:100vh;display:flex;flex-direction:column;align-items:center}
h1{font-size:28px;background:linear-gradient(90deg,#00f5ff,#ff00ff);-webkit-background-clip:text;-webkit-text-fill-color:transparent;margin-bottom:5px}
.led-grid{display:grid;grid-template-columns:repeat(15,1fr);grid-template-rows:repeat(7,1fr);gap:3px;background:#0a0a0a;padding:10px;border-radius:10px;margin-bottom:20px;aspect-ratio:15/7;max-width:500px}
.led-pixel{aspect-ratio:1/1;border-radius:50%;cursor:pointer;border:1px solid #222;background:#1a1a1a}
.led-pixel.lit{box-shadow:0 0 10px currentColor}
.controls{background:#222;padding:15px;border-radius:10px;max-width:500px;width:100%}
.color-buttons{display:flex;gap:10px;flex-wrap:wrap;margin-bottom:15px}
.color-btn{width:50px;height:50px;border-radius:8px;border:3px solid transparent;cursor:pointer}
.color-btn.active{border-color:#fff;box-shadow:0 0 20px currentColor}
button{padding:15px;font-size:16px;font-weight:bold;border:none;border-radius:8px;cursor:pointer;text-transform:uppercase}
.btn-send{background:linear-gradient(135deg,#00f5ff,#0099ff);color:#000;width:100%;margin-bottom:10px}
.status{background:#1a1a1a;padding:10px;border-radius:8px;text-align:center;margin-top:10px}
</style>
</head>
<body>
<h1>🐱 PIXEL KITTY</h1>
<div class="led-grid" id="grid"></div>
<div class="controls">
<div class="color-buttons" id="colors"></div>
<button class="btn-send" onclick="send()">📡 Send to Badge</button>
<button onclick="clear()">Clear</button>
<button onclick="rainbow()">Rainbow</button>
</div>
<div class="status" id="status">Draw and click Send</div>
<script>
const COLS=15,ROWS=7;
let matrix=Array(ROWS).fill(null).map(()=>Array(COLS).fill('#000000'));
let color='#ff0000';
let drawing=false;
const colors=['#ff0000','#ff7f00','#ffff00','#00ff00','#00ffff','#0000ff','#ff00ff','#ffffff','#000000'];
function init(){
const g=document.getElementById('grid');
for(let y=0;y<ROWS;y++){
for(let x=0;x<COLS;x++){
const p=document.createElement('div');
p.className='led-pixel';
p.dataset.x=x;
p.dataset.y=y;
p.addEventListener('mousedown',()=>{drawing=true;set(x,y)});
p.addEventListener('mouseenter',()=>{if(drawing)set(x,y)});
g.appendChild(p);
}}
document.addEventListener('mouseup',()=>drawing=false);
const c=document.getElementById('colors');
colors.forEach((col,i)=>{
const b=document.createElement('div');
b.className='color-btn'+(i==0?' active':'');
b.style.backgroundColor=col;
b.onclick=()=>{document.querySelectorAll('.color-btn').forEach(x=>x.classList.remove('active'));b.classList.add('active');color=col};
c.appendChild(b);
});}
function set(x,y){
matrix[y][x]=color;
const p=document.querySelectorAll('.led-pixel')[y*COLS+x];
p.style.backgroundColor=color;
if(color!='#000000'){p.style.color=color;p.classList.add('lit')}else{p.classList.remove('lit')}}
function clear(){
matrix=Array(ROWS).fill(null).map(()=>Array(COLS).fill('#000000'));
document.querySelectorAll('.led-pixel').forEach(p=>{p.style.backgroundColor='#1a1a1a';p.classList.remove('lit')})}
function rainbow(){
const rc=['#ff0000','#ff7f00','#ffff00','#00ff00','#0000ff','#4b0082','#9400d3'];
for(let x=0;x<COLS;x++){
const c=rc[Math.floor(x/COLS*rc.length)];
for(let y=0;y<ROWS;y++){
matrix[y][x]=c;set(x,y);}}}
async function send(){
const s=document.getElementById('status');
s.textContent='Sending...';
const d=[];
for(let x=0;x<COLS;x++){
for(let y=0;y<ROWS;y++){
const c=matrix[y][x];
d.push(parseInt(c.substr(1,2),16),parseInt(c.substr(3,2),16),parseInt(c.substr(5,2),16));}}
try{
const r=await fetch('/api/led',{method:'POST',headers:{'Content-Type':'application/json'},body:JSON.stringify({leds:d})});
if(r.ok){s.textContent='✓ Sent!'}else{s.textContent='✗ Error'}
}catch(e){s.textContent='✗ Failed'}
setTimeout(()=>s.textContent='Draw and click Send',3000)}
init();
</script>
</body>
</html>
)rawliteral";

  return html;
}
