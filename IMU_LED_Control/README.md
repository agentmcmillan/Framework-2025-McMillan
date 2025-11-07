# MPU6050 Tilt-Controlled LED Matrix

Turns your Framework Badge into a gravity-responsive pixel display using the MPU6050 accelerometer/gyroscope.

## Features

### Three Animation Modes

1. **Liquid Mode** (LEFT button)
   - LEDs act like water pooling at the lowest edge
   - Tilt the badge to watch liquid flow and settle
   - Color gradient: Blue → Cyan → White (based on liquid density)

2. **Bubble Mode** (CENTER button)
   - Bubbles spawn at the bottom and rise against gravity
   - Changes direction based on badge orientation
   - Color: Yellow/orange bubbles with shimmer effect

3. **Tilt Meter Mode** (RIGHT button)
   - Real-time visualization of tilt angle and direction
   - Arrow points in the direction of gravity
   - Color changes from green (level) to red (tilted)

## Hardware Setup

### Required Components
- Framework Badge (RP2040-based)
- MPU6050 IMU breakout board
- 4 jumper wires

### Wiring (Whisker Port)

```
MPU6050 Pin    →    Badge Whisker Port
----------------------------------------
VCC            →    +3.3V
GND            →    GND
SDA            →    IO#23 (GPIO23)
SCL            →    IO#24 (GPIO24)
```

**Note:** The MPU6050 AD0 pin can be left floating or connected to GND for I2C address 0x68 (default).

### Can I Use This With ESP32 ProS3 Coprocessor?

**YES!** The MPU6050 shares the I2C bus with the ESP32 ProS3 (dual-MCU setup). Both devices connect to the same SDA/SCL lines:

```
                    Badge Whisker/SAO Port
                    ┌─────────────┐
MPU6050 SDA ────────┤ IO#23 (SDA) │──────── ESP32 GPIO1 (SDA)
MPU6050 SCL ────────┤ IO#24 (SCL) │──────── ESP32 GPIO2 (SCL)
MPU6050 VCC ────────┤ +3.3V       │──────── ESP32 3V3
MPU6050 GND ────────┤ GND         │──────── ESP32 GND
                    └─────────────┘
```

Each I2C device has a unique address:
- **MPU6050:** 0x68
- **ESP32 ProS3:** 0x42

They won't interfere with each other. See `WIRING_WITH_ESP32.md` for detailed connection diagrams and troubleshooting.

### Can I Use This With TFT Display?

**YES!** The MPU6050 shares the I2C bus with other I2C devices. However, most TFT displays use SPI (not I2C), so they use different pins and won't conflict.

## Software Setup

### Required Arduino Libraries

Install via **Sketch → Include Library → Manage Libraries**:

1. **Adafruit NeoPixel** (for LED matrix control)
2. **Adafruit MPU6050** (for IMU sensor)
3. **Adafruit Unified Sensor** (dependency for MPU6050)

### Upload Instructions

1. Open `IMU_LED_Control.ino` in Arduino IDE
2. Select board: **Raspberry Pi Pico**
3. Select port: Your badge's USB serial port
4. Hold **BOOT** button, tap **RESET** button
5. Click **Upload**

## Usage

### Controls

- **LEFT Button:** Switch to Liquid mode
- **CENTER Button:** Switch to Bubble mode
- **RIGHT Button:** Switch to Tilt Meter mode

### Startup Sequence

1. LEDs flash **RED** if MPU6050 is not detected (check wiring!)
2. LEDs fade in **GREEN** when IMU initializes successfully
3. Default mode: **Liquid**

### Serial Monitor (Optional)

Open **Tools → Serial Monitor** (115200 baud) to see:
- MPU6050 initialization status
- Current animation mode
- Debugging info

## Technical Details

### Power Management

- Maximum brightness: **25%** (64/255)
- Safe for battery operation (~2,500mA board limit)
- Liquid/bubble effects naturally limit active LED count

### LED Matrix Addressing

- Layout: 15 columns × 7 rows
- Addressing: Column-based (top to bottom, left to right)
- First column: LEDs 0-6, Second column: LEDs 7-13, etc.

### MPU6050 Configuration

- Accelerometer range: ±2G
- Gyroscope range: ±250°/s
- Low-pass filter: 21 Hz bandwidth
- Update rate: ~33 FPS (30ms delay)

### Physics Simulation

The liquid/bubble effects use a simple cellular automaton:

1. **Gravity calculation:** Accelerometer values mapped to -1 to 1 range
2. **Liquid transfer:** Each pixel transfers a percentage of its "amount" to neighbors in the gravity direction
3. **Decay:** Values slowly decrease over time to prevent oversaturation
4. **Rendering:** Liquid amount maps to color brightness/hue

## Troubleshooting

### LEDs Flash Red on Startup

**Problem:** MPU6050 not detected on I2C bus

**Solutions:**
- Check wiring (especially SDA/SCL pins)
- Verify MPU6050 has power (VCC = 3.3V)
- Try I2C scanner sketch to detect address
- Some MPU6050 boards need pull-up resistors (4.7kΩ on SDA/SCL)

### Liquid Doesn't Flow Smoothly

**Problem:** IMU readings jittery or unstable

**Solutions:**
- Keep badge flat during startup (IMU calibrates on boot)
- Avoid strong vibrations
- Increase filter bandwidth if needed (line 92: `MPU6050_BAND_21_HZ`)

### Animation Runs Slowly

**Problem:** Frame rate drops below 33 FPS

**Solutions:**
- Reduce liquid simulation iterations (line 178: `for (int iter = 0; iter < 3; iter++)`)
- Increase loop delay (line 136: `delay(30)`)

### I2C Bus Conflict With Other Devices

**Problem:** Multiple devices on same SDA/SCL lines not working

**Solutions:**
- Each device needs a unique I2C address
- MPU6050 uses 0x68 by default (or 0x69 if AD0 is HIGH)
- Check other device addresses don't conflict
- Add 4.7kΩ pull-up resistors on SDA/SCL if using long wires

## Expansion Ideas

- Add gyroscope data for rotation detection
- Combine with temperature sensor (built into MPU6050)
- Create gesture recognition (shake detection, tap detection)
- Add magnetometer for compass mode (requires MPU9250 instead)
- Display tilt data on TFT screen (if dual-display setup)
- Stream orientation data over WiFi (if using ESP32-C6 coprocessor)

## Technical Reference

- **MPU6050 Datasheet:** https://invensense.tdk.com/products/motion-tracking/6-axis/mpu-6050/
- **I2C Protocol:** https://learn.sparkfun.com/tutorials/i2c
- **Adafruit MPU6050 Guide:** https://learn.adafruit.com/mpu6050-6-dof-accelerometer-and-gyro

## License

MIT No Attribution (MIT-0) - Same as Framework Badge project
