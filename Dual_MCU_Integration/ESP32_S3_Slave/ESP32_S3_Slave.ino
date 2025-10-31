/*
 * ESP32 ProS3 - I2C Slave Coprocessor Firmware
 *
 * Provides WiFi and Bluetooth services to Framework Badge (RP2040) via I2C.
 * Acts as I2C slave, responding to commands from RP2040 master.
 *
 * Hardware:
 * - Unexpected Maker ESP32 ProS3
 * - 16MB Flash, 8MB PSRAM
 * - STEMMA QT connector (GPIO8/GPIO9 for I2C)
 *
 * Connection: SAO port (4-pin) or STEMMA QT connector
 * Power: 3.3V from badge via SAO port
 *
 * Features:
 * - WiFi 802.11b/g/n client/AP mode
 * - BLE 5.0 beacon broadcasting
 * - Web server for badge control
 * - HTTP client for cloud integration
 * - Battery level reporting from RP2040
 * - RGB LED status indicator (GPIO48)
 */

#include <Wire.h>
#include <WiFi.h>
#include <WebServer.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLEBeacon.h>

// ============================================================================
// HARDWARE CONFIGURATION - ESP32 ProS3
// ============================================================================

#define BOARD_NAME      "ESP32 ProS3"

// I2C Configuration (STEMMA QT connector)
#define I2C_SDA         8      // GPIO 8 (STEMMA QT SDA)
#define I2C_SCL         9      // GPIO 9 (STEMMA QT SCL)
#define I2C_SLAVE_ADDR  0x42   // I2C slave address (must match RP2040)

// ProS3-specific hardware
#define RGB_LED_PIN     48     // Built-in RGB LED
#define RGB_BRIGHTNESS  20     // 0-255, keep low for power savings
#define VBAT_PIN        4      // Battery voltage monitor (optional)

// Command codes (must match RP2040 firmware)
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

// I2C communication buffers
volatile uint8_t i2cCommand = 0;
volatile uint8_t i2cDataBuffer[64];
volatile uint8_t i2cDataLength = 0;
volatile bool newCommandReceived = false;

uint8_t responseBuffer[64];
uint8_t responseLength = 0;

// WiFi state
bool wifiConnected = false;
char wifiSSID[32] = "";
char wifiPassword[64] = "";
int8_t wifiRSSI = 0;

// BLE state
bool bleBeaconActive = false;
BLEAdvertising *pAdvertising = nullptr;
char bleBeaconData[16] = "BADGE";

// Web server
WebServer *webServer = nullptr;
bool webServerRunning = false;
uint16_t webServerPort = 80;

// Battery monitoring (from RP2040)
float batteryVoltage = 0.0;

// Status LED (use built-in LED if available)
#define STATUS_LED LED_BUILTIN

// ============================================================================
// SETUP
// ============================================================================

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n\n=== ESP32 ProS3 I2C Slave ===");
  Serial.println("WiFi / BLE Coprocessor for Framework Badge");
  Serial.println("Hardware: 16MB Flash, 8MB PSRAM");
  Serial.println("I2C: STEMMA QT connector (GPIO8/GPIO9)");

  // Initialize status LED
  pinMode(STATUS_LED, OUTPUT);
  digitalWrite(STATUS_LED, LOW);

  // Initialize I2C as slave
  Serial.print("Initializing I2C slave (address 0x");
  Serial.print(I2C_SLAVE_ADDR, HEX);
  Serial.println(")...");

  Wire.begin(I2C_SDA, I2C_SCL, I2C_SLAVE_ADDR);
  Wire.onReceive(i2cReceiveEvent);
  Wire.onRequest(i2cRequestEvent);

  Serial.println("✓ I2C slave initialized");

  // Startup blink
  for (int i = 0; i < 3; i++) {
    digitalWrite(STATUS_LED, HIGH);
    delay(100);
    digitalWrite(STATUS_LED, LOW);
    delay(100);
  }

  Serial.println("\n=== Ready for commands ===\n");
  printHelp();
}

// ============================================================================
// MAIN LOOP
// ============================================================================

void loop() {
  // Process I2C commands
  if (newCommandReceived) {
    newCommandReceived = false;
    processCommand();
  }

  // Handle web server requests
  if (webServerRunning && webServer != nullptr) {
    webServer->handleClient();
  }

  // Update WiFi status
  if (WiFi.status() == WL_CONNECTED) {
    if (!wifiConnected) {
      wifiConnected = true;
      wifiRSSI = WiFi.RSSI();
      Serial.println("✓ WiFi connected");
      Serial.print("  IP: ");
      Serial.println(WiFi.localIP());
      Serial.print("  RSSI: ");
      Serial.print(wifiRSSI);
      Serial.println(" dBm");
    }
    wifiRSSI = WiFi.RSSI();
  } else {
    if (wifiConnected) {
      wifiConnected = false;
      Serial.println("✗ WiFi disconnected");
    }
  }

  delay(10);
}

// ============================================================================
// I2C EVENT HANDLERS
// ============================================================================

void i2cReceiveEvent(int numBytes) {
  if (numBytes == 0) return;

  // First byte is command
  i2cCommand = Wire.read();
  i2cDataLength = 0;

  // Read remaining bytes as data
  while (Wire.available() && i2cDataLength < 64) {
    i2cDataBuffer[i2cDataLength++] = Wire.read();
  }

  newCommandReceived = true;

  // Debug
  Serial.print("I2C RX: CMD=0x");
  Serial.print(i2cCommand, HEX);
  Serial.print(" LEN=");
  Serial.println(i2cDataLength);
}

void i2cRequestEvent() {
  // Send response to master
  if (responseLength > 0) {
    Wire.write(responseBuffer, responseLength);

    // Debug
    Serial.print("I2C TX: ");
    Serial.print(responseLength);
    Serial.println(" bytes");
  } else {
    // No response ready, send status OK
    Wire.write(STATUS_OK);
  }
}

// ============================================================================
// COMMAND PROCESSING
// ============================================================================

void processCommand() {
  // Clear previous response
  memset(responseBuffer, 0, sizeof(responseBuffer));
  responseLength = 0;

  Serial.print("Processing command: 0x");
  Serial.println(i2cCommand, HEX);

  switch (i2cCommand) {
    case CMD_PING:
      handlePing();
      break;

    case CMD_GET_STATUS:
      handleGetStatus();
      break;

    case CMD_WIFI_CONNECT:
      handleWiFiConnect();
      break;

    case CMD_WIFI_DISCONNECT:
      handleWiFiDisconnect();
      break;

    case CMD_WIFI_SCAN:
      handleWiFiScan();
      break;

    case CMD_BLE_BEACON:
      handleBLEBeacon();
      break;

    case CMD_BLE_STOP:
      handleBLEStop();
      break;

    case CMD_WEB_SERVER:
      handleWebServer();
      break;

    case CMD_BATTERY_LEVEL:
      handleBatteryLevel();
      break;

    default:
      Serial.println("✗ Unknown command");
      responseBuffer[0] = STATUS_INVALID_CMD;
      responseLength = 1;
      break;
  }
}

// ============================================================================
// COMMAND HANDLERS
// ============================================================================

void handlePing() {
  Serial.println("PING");
  responseBuffer[0] = 0xA5; // Ping response
  responseLength = 1;
}

void handleGetStatus() {
  Serial.println("GET_STATUS");

  responseBuffer[0] = STATUS_OK;
  responseBuffer[1] = wifiConnected ? STATUS_WIFI_CONNECTED : STATUS_WIFI_DISCONNECTED;
  responseBuffer[2] = bleBeaconActive ? STATUS_BLE_ACTIVE : 0x00;
  responseBuffer[3] = (uint8_t)wifiRSSI;
  responseLength = 4;

  Serial.print("  WiFi: ");
  Serial.println(wifiConnected ? "Connected" : "Disconnected");
  Serial.print("  BLE: ");
  Serial.println(bleBeaconActive ? "Active" : "Inactive");
}

void handleWiFiConnect() {
  Serial.println("WIFI_CONNECT");

  // Parse SSID and password from data buffer
  // Format: [ssid_len, ssid..., pwd_len, pwd...]
  if (i2cDataLength < 2) {
    responseBuffer[0] = STATUS_ERROR;
    responseLength = 1;
    return;
  }

  uint8_t ssidLen = i2cDataBuffer[0];
  if (ssidLen == 0 || ssidLen > 31) {
    responseBuffer[0] = STATUS_ERROR;
    responseLength = 1;
    return;
  }

  memcpy(wifiSSID, &i2cDataBuffer[1], ssidLen);
  wifiSSID[ssidLen] = '\0';

  uint8_t pwdLen = i2cDataBuffer[1 + ssidLen];
  if (pwdLen > 0 && pwdLen < 64) {
    memcpy(wifiPassword, &i2cDataBuffer[2 + ssidLen], pwdLen);
    wifiPassword[pwdLen] = '\0';
  } else {
    wifiPassword[0] = '\0';
  }

  Serial.print("  SSID: ");
  Serial.println(wifiSSID);

  // Connect to WiFi
  WiFi.begin(wifiSSID, wifiPassword);

  responseBuffer[0] = STATUS_OK;
  responseLength = 1;
}

void handleWiFiDisconnect() {
  Serial.println("WIFI_DISCONNECT");
  WiFi.disconnect();
  wifiConnected = false;

  responseBuffer[0] = STATUS_OK;
  responseLength = 1;
}

void handleWiFiScan() {
  Serial.println("WIFI_SCAN");

  int n = WiFi.scanNetworks();
  Serial.print("Networks found: ");
  Serial.println(n);

  for (int i = 0; i < n && i < 10; i++) {
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.print(WiFi.SSID(i));
    Serial.print(" (");
    Serial.print(WiFi.RSSI(i));
    Serial.print(" dBm) ");
    Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? "Open" : "Encrypted");
  }

  responseBuffer[0] = STATUS_OK;
  responseBuffer[1] = (uint8_t)n; // Number of networks
  responseLength = 2;
}

void handleBLEBeacon() {
  Serial.println("BLE_BEACON");

  // Parse beacon data from buffer
  if (i2cDataLength > 0) {
    uint8_t dataLen = i2cDataBuffer[0];
    if (dataLen > 0 && dataLen <= 15) {
      memcpy(bleBeaconData, &i2cDataBuffer[1], dataLen);
      bleBeaconData[dataLen] = '\0';
    }
  }

  Serial.print("  Beacon data: ");
  Serial.println(bleBeaconData);

  // Initialize BLE if not already done
  if (!bleBeaconActive) {
    BLEDevice::init(bleBeaconData);

    // Create BLE server
    BLEServer *pServer = BLEDevice::createServer();

    // Get advertising instance
    pAdvertising = BLEDevice::getAdvertising();

    // Set advertising data
    BLEAdvertisementData advertisementData;
    advertisementData.setName(bleBeaconData);
    advertisementData.setManufacturerData(bleBeaconData);

    pAdvertising->setAdvertisementData(advertisementData);
    pAdvertising->start();

    bleBeaconActive = true;
    Serial.println("  BLE beacon started");
  }

  responseBuffer[0] = STATUS_OK;
  responseLength = 1;
}

void handleBLEStop() {
  Serial.println("BLE_STOP");

  if (bleBeaconActive && pAdvertising != nullptr) {
    pAdvertising->stop();
    BLEDevice::deinit(false);
    bleBeaconActive = false;
    Serial.println("  BLE beacon stopped");
  }

  responseBuffer[0] = STATUS_OK;
  responseLength = 1;
}

void handleWebServer() {
  Serial.println("WEB_SERVER");

  if (i2cDataLength >= 2) {
    webServerPort = (i2cDataBuffer[0] << 8) | i2cDataBuffer[1];
  } else {
    webServerPort = 80;
  }

  if (!webServerRunning) {
    startWebServer();
  }

  responseBuffer[0] = STATUS_OK;
  responseLength = 1;
}

void handleBatteryLevel() {
  // Receive battery voltage from RP2040
  if (i2cDataLength >= 2) {
    uint16_t voltageInt = (i2cDataBuffer[0] << 8) | i2cDataBuffer[1];
    batteryVoltage = voltageInt / 100.0;

    Serial.print("Battery: ");
    Serial.print(batteryVoltage);
    Serial.println("V");
  }

  responseBuffer[0] = STATUS_OK;
  responseLength = 1;
}

// ============================================================================
// WEB SERVER
// ============================================================================

void startWebServer() {
  if (!wifiConnected) {
    Serial.println("✗ Cannot start web server: WiFi not connected");
    return;
  }

  webServer = new WebServer(webServerPort);

  // Define routes
  webServer->on("/", handleRoot);
  webServer->on("/status", handleStatus);
  webServer->on("/led", handleLED);
  webServer->onNotFound(handleNotFound);

  webServer->begin();
  webServerRunning = true;

  Serial.print("✓ Web server started on port ");
  Serial.println(webServerPort);
  Serial.print("  Access at: http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}

void handleRoot() {
  String html = "<!DOCTYPE html><html><head>";
  html += "<title>Framework Badge Control</title>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<style>";
  html += "body { font-family: Arial; margin: 20px; background: #1a1a1a; color: #fff; }";
  html += "h1 { color: #00ffff; }";
  html += ".btn { padding: 15px 30px; margin: 10px; font-size: 18px; ";
  html += "background: #00ffff; border: none; border-radius: 5px; cursor: pointer; }";
  html += ".btn:hover { background: #00cccc; }";
  html += ".status { background: #2a2a2a; padding: 15px; border-radius: 5px; margin: 10px 0; }";
  html += "</style></head><body>";
  html += "<h1>🐱 Framework Badge Control</h1>";

  html += "<div class='status'>";
  html += "<h2>Status</h2>";
  html += "<p>WiFi: Connected (" + String(wifiRSSI) + " dBm)</p>";
  html += "<p>Battery: " + String(batteryVoltage) + "V</p>";
  html += "<p>BLE Beacon: " + String(bleBeaconActive ? "Active" : "Inactive") + "</p>";
  html += "</div>";

  html += "<h2>LED Controls</h2>";
  html += "<button class='btn' onclick=\"fetch('/led?action=ripple')\">Rainbow Ripple</button><br>";
  html += "<button class='btn' onclick=\"fetch('/led?action=red')\">Red Flash</button><br>";
  html += "<button class='btn' onclick=\"fetch('/led?action=green')\">Green Flash</button><br>";
  html += "<button class='btn' onclick=\"fetch('/led?action=blue')\">Blue Flash</button><br>";

  html += "<p><a href='/status'>JSON Status</a></p>";
  html += "</body></html>";

  webServer->send(200, "text/html", html);
}

void handleStatus() {
  String json = "{";
  json += "\"wifi_connected\":" + String(wifiConnected ? "true" : "false") + ",";
  json += "\"rssi\":" + String(wifiRSSI) + ",";
  json += "\"battery\":" + String(batteryVoltage) + ",";
  json += "\"ble_beacon\":" + String(bleBeaconActive ? "true" : "false");
  json += "}";

  webServer->send(200, "application/json", json);
}

void handleLED() {
  String action = webServer->arg("action");

  Serial.print("Web command: LED ");
  Serial.println(action);

  // TODO: Send command to RP2040 to trigger LED action
  // (Requires RP2040 to also act as I2C master or use different communication)

  webServer->send(200, "text/plain", "OK: " + action);
}

void handleNotFound() {
  webServer->send(404, "text/plain", "Not Found");
}

// ============================================================================
// SERIAL COMMAND INTERFACE (FOR TESTING)
// ============================================================================

void printHelp() {
  Serial.println("\n=== Serial Commands ===");
  Serial.println("w - WiFi scan");
  Serial.println("b - Start BLE beacon");
  Serial.println("s - Show status");
  Serial.println("h - This help");
  Serial.println();
}

void serialEvent() {
  while (Serial.available()) {
    char cmd = Serial.read();

    switch (cmd) {
      case 'w':
        handleWiFiScan();
        break;

      case 'b':
        handleBLEBeacon();
        break;

      case 's':
        Serial.println("\n=== Status ===");
        Serial.print("WiFi: ");
        Serial.println(wifiConnected ? "Connected" : "Disconnected");
        if (wifiConnected) {
          Serial.print("  IP: ");
          Serial.println(WiFi.localIP());
          Serial.print("  RSSI: ");
          Serial.print(wifiRSSI);
          Serial.println(" dBm");
        }
        Serial.print("BLE: ");
        Serial.println(bleBeaconActive ? "Active" : "Inactive");
        Serial.print("Battery: ");
        Serial.print(batteryVoltage);
        Serial.println("V");
        Serial.println();
        break;

      case 'h':
        printHelp();
        break;
    }
  }
}
