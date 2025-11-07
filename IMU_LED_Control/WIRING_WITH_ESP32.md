# Wiring MPU6050 + ESP32 ProS3 to Framework Badge

This guide shows how to connect **both** the MPU6050 IMU and ESP32 ProS3 coprocessor to the Framework Badge using a shared I2C bus.

## System Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                    Framework Badge (RP2040)                 │
│                                                             │
│  ┌──────────────┐    ┌──────────────┐    ┌──────────────┐  │
│  │  LED Matrix  │    │   Buttons    │    │   Battery    │  │
│  │   15×7 RGB   │    │   L / C / R  │    │   Monitor    │  │
│  └──────────────┘    └──────────────┘    └──────────────┘  │
│                                                             │
│         Whisker/SAO Port (I2C Bus)                          │
│         ┌───────────────────────────┐                       │
│         │ IO#23 (SDA)  IO#24 (SCL) │                       │
│         │  +3.3V         GND        │                       │
│         └──────────┬────────────────┘                       │
└────────────────────┼───────────────────────────────────────┘
                     │
          ┌──────────┴──────────┐
          │   Shared I2C Bus    │
          │   (100kHz Standard) │
          └──────────┬──────────┘
                     │
         ┌───────────┴───────────┐
         │                       │
         ▼                       ▼
┌─────────────────┐     ┌─────────────────┐
│   MPU6050 IMU   │     │  ESP32 ProS3    │
│                 │     │  Coprocessor    │
│  I2C Addr: 0x68 │     │  I2C Addr: 0x42 │
│                 │     │                 │
│  - Accel (±2G)  │     │  - WiFi 6 AP    │
│  - Gyro (±250°) │     │  - Web Server   │
│  - Temp Sensor  │     │  - BLE 5.0      │
└─────────────────┘     └─────────────────┘
```

## Why This Works: I2C Multi-Device Bus

I2C (Inter-Integrated Circuit) is designed for multiple devices on the same two wires:
- **SDA (Serial Data):** Bidirectional data line
- **SCL (Serial Clock):** Clock signal from master (RP2040)

Each device has a **unique address**:
- **MPU6050:** 0x68 (or 0x69 if AD0 pin is HIGH)
- **ESP32 ProS3:** 0x42

The RP2040 master sends commands to specific addresses, so devices don't interfere.

## Physical Wiring Options

### Option 1: Breadboard (Recommended for Testing)

Use a small breadboard to create the shared bus connections:

```
Framework Badge Whisker/SAO Port
        ↓ (4 wires)
    Breadboard
     ┌────────────────────────┐
     │  Power Rails:          │
     │  ================      │
     │  +3.3V ──────────────  │─┐
     │  GND   ──────────────  │─┼─ Shared power
     │                        │─┘
     │  I2C Bus Rails:        │
     │  ================      │
     │  SDA (IO#23) ────────  │─┐
     │  SCL (IO#24) ────────  │─┼─ Shared signals
     └────────────────────────┘─┘
            │        │
            │        └─────────────┐
            │                      │
            ▼                      ▼
      MPU6050 IMU            ESP32 ProS3
      (4 wires)              (4 wires)
```

**Breadboard Connections:**
1. Insert badge's Whisker/SAO wires into breadboard power/bus rails
2. Connect MPU6050 to same rails (parallel)
3. Connect ESP32 ProS3 to same rails (parallel)

**Pin Mapping:**
```
Badge Pin       Breadboard Rail    MPU6050 Pin    ESP32 ProS3 Pin
─────────────   ───────────────    ───────────    ───────────────
+3.3V      →    Red (+) rail   →   VCC        →   3V3
GND        →    Blue (-) rail  →   GND        →   GND
IO#23 (SDA)→    SDA bus rail   →   SDA        →   GPIO1 (SDA)
IO#24 (SCL)→    SCL bus rail   →   SCL        →   GPIO2 (SCL)
```

### Option 2: Daisy-Chain Wiring

Connect devices in series (works because I2C is a bus):

```
Badge SAO Port
    ↓ (4 wires)
MPU6050 IMU
    ├─ VCC ←─ Badge +3.3V
    ├─ GND ←─ Badge GND
    ├─ SDA ←─ Badge IO#23 ──┐
    └─ SCL ←─ Badge IO#24 ──┼─ Continue to ESP32
                             │
                             ↓ (4 wires)
ESP32 ProS3
    ├─ 3V3 ←─ MPU6050 VCC (or Badge +3.3V)
    ├─ GND ←─ MPU6050 GND (or Badge GND)
    ├─ GPIO1 (SDA) ←─ MPU6050 SDA
    └─ GPIO2 (SCL) ←─ MPU6050 SCL
```

**Note:** Some MPU6050 breakout boards have duplicate pins on both sides, making daisy-chaining easy.

### Option 3: Y-Cable or Custom Splitter

Create a custom cable that splits SAO port to two connectors:

```
Badge SAO Port (2×2 header)
        │
        ├─ 4-wire cable
        │
    Y-Splitter
        ├──→ MPU6050 connector (4 pins)
        └──→ ESP32 ProS3 connector (4 pins)
```

**Materials:**
- 1× male 2×2 header (for badge SAO port)
- 2× female 4-pin connectors (for MPU6050 and ESP32)
- 28AWG wire (4 colors: red, black, green/blue, yellow)

## Detailed Pin Connections

### Badge Whisker/SAO Port Pinout

```
Top view of badge (SAO port on top edge):

    ┌───┬───┐
    │ 1 │ 2 │  Pin 1: VCC (+3.3V)
    ├───┼───┤  Pin 2: GND
    │ 3 │ 4 │  Pin 3: SDA (IO#23)
    └───┴───┘  Pin 4: SCL (IO#24)
```

### MPU6050 Breakout Board Pinout

```
Most common MPU6050 boards (GY-521 or similar):

┌──────────────────┐
│   MPU6050 Board  │
│                  │
│  VCC  ──────── Pin 1 (3.3V input)
│  GND  ──────── Pin 2 (Ground)
│  SCL  ──────── Pin 3 (I2C clock)
│  SDA  ──────── Pin 4 (I2C data)
│  XDA  ──────── (Not used - auxiliary I2C)
│  XCL  ──────── (Not used - auxiliary I2C)
│  AD0  ──────── (Address select: LOW=0x68, HIGH=0x69)
│  INT  ──────── (Not used - interrupt output)
└──────────────────┘

**IMPORTANT:**
- MPU6050 works with 3.3V OR 5V power
- Use 3.3V to match badge power rail
- AD0 pin can be left floating (defaults to LOW = address 0x68)
```

### ESP32 ProS3 Pinout (Unexpected Maker)

```
ESP32 ProS3 Side View:

┌────────────────────────────────┐
│         [USB-C Port]           │
│     ESP32 ProS3 (Top Edge)     │
│                                │
│  Left Side Pins:               │
│  ─────────────────             │
│  GND   ← Pin 2 (SAO GND)       │
│  3V3   ← Pin 1 (SAO +3.3V)     │
│  IO0                           │
│  IO1   ← Pin 3 (SAO SDA)       │
│  IO2   ← Pin 4 (SAO SCL)       │
│  IO3                           │
│  IO4                           │
│  ...                           │
└────────────────────────────────┘

**Alternative: STEMMA QT Connector**
ProS3 has a JST STEMMA QT connector (4-pin) for I2C:
- Uses GPIO8 (SDA) and GPIO9 (SCL)
- NOT used for badge connection (uses GPIO1/2 instead)
- Available for other sensors/displays
```

## Wire Color Convention (Recommended)

Use consistent colors to avoid mistakes:

| Signal      | Color Recommendation | Wire Gauge |
|-------------|---------------------|------------|
| +3.3V (VCC) | **Red**             | 22-28 AWG  |
| GND         | **Black**           | 22-28 AWG  |
| SDA         | **Blue or Green**   | 22-28 AWG  |
| SCL         | **Yellow or White** | 22-28 AWG  |

## Pull-Up Resistors

I2C requires pull-up resistors on SDA and SCL lines. In most cases, you **don't need to add external resistors** because:

1. **MPU6050 boards:** Often have built-in 4.7kΩ pull-ups
2. **RP2040:** Can enable internal pull-ups (weak ~50kΩ)
3. **ESP32 ProS3:** Has internal pull-ups (~45kΩ)

### When to Add External Pull-Ups

Add 4.7kΩ pull-up resistors between SDA/SCL and +3.3V if:
- Wire length > 15cm (long cables increase capacitance)
- I2C communication is unreliable (timeouts, NAKs)
- Using multiple devices (>2) on the bus
- Serial monitor shows I2C errors

```
+3.3V ──┬─── 4.7kΩ ───┬─── SDA (all devices)
        │             │
        └─── 4.7kΩ ───┴─── SCL (all devices)
```

## Software Configuration

### Arduino Sketch Compatibility

**Good news:** The IMU_LED_Control sketch is already configured for shared I2C!

Key lines in `IMU_LED_Control.ino:73-118`:
```cpp
// Initialize I2C with custom pins
Wire.setSDA(23);  // Shared SDA
Wire.setCL(24);   // Shared SCL
Wire.begin();     // RP2040 is I2C master

// Initialize MPU6050 at address 0x68
mpu.begin(0x68, &Wire);  // Explicitly set address

// ESP32 ProS3 is at address 0x42 (handled by separate sketch)
```

### Testing the Shared Bus

Use this I2C scanner sketch to verify both devices are detected:

```cpp
#include <Wire.h>

void setup() {
  Serial.begin(115200);
  Wire.setSDA(23);
  Wire.setSCL(24);
  Wire.begin();

  Serial.println("I2C Scanner - Looking for devices...");
}

void loop() {
  int deviceCount = 0;

  for (byte addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0) {
      Serial.print("Device found at 0x");
      Serial.println(addr, HEX);
      deviceCount++;
    }
  }

  Serial.print("Total devices: ");
  Serial.println(deviceCount);
  Serial.println("Expected: 2 (0x42 = ESP32, 0x68 = MPU6050)");
  delay(5000);
}
```

**Expected Output:**
```
I2C Scanner - Looking for devices...
Device found at 0x42  ← ESP32 ProS3
Device found at 0x68  ← MPU6050 IMU
Total devices: 2
```

## Troubleshooting

### Problem: Only One Device Detected

**Symptoms:**
- I2C scanner finds 0x42 (ESP32) but not 0x68 (MPU6050)
- OR finds 0x68 (MPU6050) but not 0x42 (ESP32)

**Solutions:**
1. **Check power:** Measure voltage at each device's VCC pin (should be 3.3V)
2. **Check ground:** Continuity test between all GND pins (0Ω resistance)
3. **Check SDA/SCL:** Verify both devices are connected to same wires
4. **Swap devices:** Test MPU6050 alone first, then add ESP32

### Problem: Neither Device Detected

**Symptoms:**
- I2C scanner finds 0 devices
- Both devices show no activity

**Solutions:**
1. **Verify I2C pins:** Ensure Wire.setSDA(23) and Wire.setSCL(24) match wiring
2. **Add pull-ups:** Solder 4.7kΩ resistors on SDA/SCL if using long wires
3. **Check for shorts:** Use multimeter to verify SDA and SCL aren't shorted to GND or VCC
4. **Test with simpler device:** Try an I2C OLED display to verify RP2040 I2C works

### Problem: MPU6050 at Wrong Address

**Symptoms:**
- I2C scanner finds device at 0x69 instead of 0x68

**Solutions:**
1. **Check AD0 pin:** MPU6050's AD0 pin may be pulled HIGH (some boards have this by default)
2. **Update sketch:** Change `mpu.begin(0x68, &Wire)` to `mpu.begin(0x69, &Wire)`
3. **OR ground AD0:** Connect AD0 pin on MPU6050 to GND to force address 0x68

### Problem: I2C Bus Hangs or Freezes

**Symptoms:**
- Setup() never completes
- Serial monitor stops printing
- LEDs remain off

**Solutions:**
1. **Add timeout:** Modify sketch to skip MPU6050 if not found after 2 seconds
2. **Lower I2C speed:** Add `Wire.setClock(50000);` to reduce speed from 100kHz to 50kHz
3. **Reset both devices:** Power cycle badge completely (remove batteries + USB)
4. **Check for EMI:** Keep I2C wires away from LED matrix data line (IO#4)

### Problem: Communication Works But Data is Corrupted

**Symptoms:**
- Accelerometer readings are random/noisy
- LEDs show erratic behavior
- Serial monitor shows garbled data

**Solutions:**
1. **Add decoupling capacitors:**
   - 10µF electrolytic across +3.3V and GND near MPU6050
   - 0.1µF ceramic across +3.3V and GND near MPU6050
2. **Shorten wires:** Keep I2C wiring under 10cm if possible
3. **Add pull-ups:** External 4.7kΩ resistors improve signal integrity
4. **Check grounding:** Ensure badge GND is connected to both devices

## Power Budget Analysis

### Current Draw by Device

| Device           | Idle    | Active  | Peak    |
|------------------|---------|---------|---------|
| RP2040 (Badge)   | 40 mA   | 80 mA   | 100 mA  |
| LED Matrix       | 0 mA    | 200 mA  | 400 mA* |
| MPU6050 IMU      | 0.5 mA  | 3.5 mA  | 3.9 mA  |
| ESP32 ProS3      | 20 mA   | 150 mA  | 200 mA  |
| **TOTAL**        | **60 mA** | **430 mA** | **700 mA** |

*LED brightness limited to 25% in firmware for safety

### Badge Power Supply Capability

- **USB:** 500 mA available (sufficient for all operations)
- **Battery (2×AA):** 2,500 mAh capacity
  - Typical use: 430 mA → ~6 hours runtime
  - Heavy use: 700 mA → ~3.5 hours runtime
  - Recommended: Monitor battery voltage (IO#26) and reduce features when low

### Power Optimization Tips

1. **Disable WiFi when not needed:** ESP32 drops from 150mA to 20mA
2. **Use ESP32 light sleep:** Reduces to 10µA when idle
3. **Limit LED animations:** Use fewer pixels or lower brightness
4. **Put MPU6050 to sleep:** Call `mpu.enableSleep(true)` when not reading sensors

## Advanced Configuration

### Using MPU6050's Interrupt Pin

The MPU6050 can trigger interrupts when motion is detected, saving power:

```cpp
// Connect MPU6050 INT pin to RP2040 GPIO (e.g., IO#25)
#define MPU_INT_PIN 25

void setup() {
  pinMode(MPU_INT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(MPU_INT_PIN), motionDetected, FALLING);

  // Configure MPU6050 for motion detection
  mpu.setMotionDetectionThreshold(10);
  mpu.setMotionDetectionDuration(20);
  mpu.setInterruptPinLatch(true);
  mpu.setInterruptPinPolarity(false);
  mpu.setMotionInterrupt(true);
}

void motionDetected() {
  // Wake up and read sensor data
}
```

### Sharing IMU Data with ESP32

The ESP32 can read MPU6050 data from the RP2040 via I2C commands:

**On RP2040 (I2C slave mode):**
```cpp
Wire1.begin(0x50);  // RP2040 acts as slave at address 0x50
Wire1.onRequest(sendIMUData);

void sendIMUData() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Send accelerometer X value
  int16_t accelX = (int16_t)(a.acceleration.x * 100);
  Wire1.write((byte*)&accelX, 2);
}
```

**On ESP32 (I2C master):**
```cpp
Wire.requestFrom(0x50, 2);  // Request 2 bytes from RP2040
int16_t accelX = Wire.read() | (Wire.read() << 8);
```

This allows the ESP32 to stream orientation data over WiFi!

## Next Steps

Once wiring is complete:
1. Upload `IMU_LED_Control.ino` to the badge
2. Test each animation mode (Liquid, Bubble, Tilt Meter)
3. Verify ESP32 ProS3 still works (WiFi AP, web server, BLE)
4. Consider combining features (e.g., tilt data on web interface)

## Reference Documents

- **MPU6050 Datasheet:** https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Datasheet1.pdf
- **ESP32 ProS3 Pinout:** https://esp32s3.com/pros3.html
- **I2C Specification:** https://www.nxp.com/docs/en/user-guide/UM10204.pdf
- **RP2040 I2C Documentation:** https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf (Section 4.3)
