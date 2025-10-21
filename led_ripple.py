"""
Framework-2025-Badge LED Ripple Effect
MicroPython sketch for creating a ripple animation on the 15x7 LED matrix

IMPORTANT: LED matrix power management is critical!
- Never use full brightness white (hardware damage risk)
- This sketch uses low brightness (0.15 max) for safety
- Limits number of lit pixels to prevent over-current
"""

import time
import math
from machine import Pin
import neopixel

# Configuration
LED_PIN = 4          # GPIO pin for LED matrix data
NUM_LEDS = 105       # 15 columns × 7 rows
COLS = 15
ROWS = 7
MAX_BRIGHTNESS = 0.15  # Safety limit: 15% maximum brightness

# Button Configuration
BUTTON_LEFT = 5      # GPIO #5
BUTTON_CENTER = 6    # GPIO #6 - "MEOW" button
BUTTON_RIGHT = 7     # GPIO #7

# Initialize NeoPixel strip
np = neopixel.NeoPixel(Pin(LED_PIN), NUM_LEDS)

# Initialize buttons (active low with pull-up resistors)
btn_center = Pin(BUTTON_CENTER, Pin.IN, Pin.PULL_UP)

def xy_to_index(x, y):
    """
    Convert x,y coordinates to LED index
    LED matrix is addressed by columns (top to bottom, left to right)
    Column 0: LEDs 0-6, Column 1: LEDs 7-13, etc.
    """
    if x < 0 or x >= COLS or y < 0 or y >= ROWS:
        return None
    return x * ROWS + y

def index_to_xy(index):
    """Convert LED index back to x,y coordinates"""
    x = index // ROWS
    y = index % ROWS
    return (x, y)

def distance(x1, y1, x2, y2):
    """Calculate Euclidean distance between two points"""
    return math.sqrt((x2 - x1)**2 + (y2 - y1)**2)

def hsv_to_rgb(h, s, v):
    """
    Convert HSV color to RGB (0-255 range)
    h: 0-360, s: 0-1, v: 0-1
    """
    h = h % 360
    c = v * s
    x = c * (1 - abs((h / 60) % 2 - 1))
    m = v - c

    if h < 60:
        r, g, b = c, x, 0
    elif h < 120:
        r, g, b = x, c, 0
    elif h < 180:
        r, g, b = 0, c, x
    elif h < 240:
        r, g, b = 0, x, c
    elif h < 300:
        r, g, b = x, 0, c
    else:
        r, g, b = c, 0, x

    return (int((r + m) * 255), int((g + m) * 255), int((b + m) * 255))

def ripple_effect(center_x, center_y, wave_speed=0.3, duration=3.0):
    """
    Create a ripple effect emanating from a center point

    Args:
        center_x: X coordinate of ripple center (0-14)
        center_y: Y coordinate of ripple center (0-6)
        wave_speed: Speed of ripple propagation
        duration: How long the ripple lasts (seconds)
    """
    start_time = time.time()

    while time.time() - start_time < duration:
        elapsed = time.time() - start_time

        # Calculate wave position
        wave_pos = elapsed * wave_speed * 10

        for x in range(COLS):
            for y in range(ROWS):
                # Calculate distance from center
                dist = distance(center_x, center_y, x, y)

                # Calculate wave intensity based on distance
                wave_offset = abs(dist - wave_pos)

                # Create wave envelope (bright at wave front, dim elsewhere)
                if wave_offset < 1.5:
                    intensity = (1.5 - wave_offset) / 1.5
                    intensity = max(0, min(1, intensity))

                    # Color based on distance (rainbow ripple)
                    hue = (dist * 30 + elapsed * 100) % 360
                    r, g, b = hsv_to_rgb(hue, 1.0, intensity * MAX_BRIGHTNESS)

                    index = xy_to_index(x, y)
                    if index is not None:
                        np[index] = (r, g, b)
                else:
                    # Fade out pixels not in wave
                    index = xy_to_index(x, y)
                    if index is not None:
                        np[index] = (0, 0, 0)

        np.write()
        time.sleep(0.05)  # ~20 FPS

def multi_ripple():
    """
    Multiple ripples from different points
    """
    ripple_centers = [
        (7, 3),   # Center
        (0, 0),   # Top-left
        (14, 0),  # Top-right
        (0, 6),   # Bottom-left
        (14, 6),  # Bottom-right
    ]

    start_time = time.time()
    duration = 5.0

    while time.time() - start_time < duration:
        elapsed = time.time() - start_time

        for x in range(COLS):
            for y in range(ROWS):
                total_r, total_g, total_b = 0, 0, 0

                # Combine ripples from all centers
                for i, (cx, cy) in enumerate(ripple_centers):
                    dist = distance(cx, cy, x, y)

                    # Stagger ripple start times
                    wave_time = elapsed - (i * 0.3)
                    if wave_time > 0:
                        wave_pos = wave_time * 0.4 * 10
                        wave_offset = abs(dist - wave_pos)

                        if wave_offset < 1.5:
                            intensity = (1.5 - wave_offset) / 1.5
                            intensity = max(0, min(1, intensity))

                            hue = (i * 72 + dist * 30) % 360  # Different color per ripple
                            r, g, b = hsv_to_rgb(hue, 1.0, intensity * MAX_BRIGHTNESS)

                            total_r += r
                            total_g += g
                            total_b += b

                # Cap values at 255
                total_r = min(255, total_r)
                total_g = min(255, total_g)
                total_b = min(255, total_b)

                index = xy_to_index(x, y)
                if index is not None:
                    np[index] = (int(total_r), int(total_g), int(total_b))

        np.write()
        time.sleep(0.05)

def clear_display():
    """Turn off all LEDs"""
    for i in range(NUM_LEDS):
        np[i] = (0, 0, 0)
    np.write()

def rainbow_ripple_from_center():
    """
    Rainbow ripple effect triggered by the MEOW button
    Emanates from the center of the screen with vibrant rainbow colors
    """
    center_x = COLS // 2  # Center of display (x=7)
    center_y = ROWS // 2  # Center of display (y=3)

    start_time = time.time()
    duration = 2.5  # Ripple duration in seconds

    while time.time() - start_time < duration:
        elapsed = time.time() - start_time

        # Wave propagation speed
        wave_pos = elapsed * 0.5 * 10

        for x in range(COLS):
            for y in range(ROWS):
                # Calculate distance from center
                dist = distance(center_x, center_y, x, y)

                # Calculate wave intensity
                wave_offset = abs(dist - wave_pos)

                # Create expanding wave with trailing fade
                if wave_offset < 2.0:
                    # Main wave front
                    intensity = (2.0 - wave_offset) / 2.0
                    intensity = max(0, min(1, intensity))

                    # Rainbow colors based on distance (creates rainbow bands)
                    hue = (dist * 40 + elapsed * 60) % 360
                    r, g, b = hsv_to_rgb(hue, 1.0, intensity * MAX_BRIGHTNESS)

                    index = xy_to_index(x, y)
                    if index is not None:
                        np[index] = (r, g, b)
                elif dist < wave_pos:
                    # Trailing sparkle/fade effect
                    fade_intensity = max(0, 1.0 - (wave_pos - dist) / 5.0)
                    hue = (dist * 40) % 360
                    r, g, b = hsv_to_rgb(hue, 0.8, fade_intensity * MAX_BRIGHTNESS * 0.3)

                    index = xy_to_index(x, y)
                    if index is not None:
                        np[index] = (r, g, b)
                else:
                    # Not yet reached by wave
                    index = xy_to_index(x, y)
                    if index is not None:
                        np[index] = (0, 0, 0)

        np.write()
        time.sleep(0.04)  # ~25 FPS for smooth animation

def idle_animation():
    """
    Gentle pulsing animation when waiting for button press
    """
    # Simple breathing effect on center pixel
    center_index = xy_to_index(COLS // 2, ROWS // 2)

    for i in range(20):  # Short animation cycle
        intensity = (math.sin(i * 0.3) + 1) / 2  # 0 to 1
        r, g, b = hsv_to_rgb(200, 0.7, intensity * MAX_BRIGHTNESS * 0.5)

        if center_index is not None:
            np[center_index] = (r, g, b)

        np.write()
        time.sleep(0.05)

        # Check for button press during idle
        if btn_center.value() == 0:  # Button pressed (active low)
            return True

    return False

# Main program - Button-triggered rainbow ripple
if __name__ == "__main__":
    print("Framework Badge - MEOW Button Rainbow Ripple")
    print("Press CENTER button for rainbow ripple effect!")
    print("Press Ctrl+C to stop")

    # Debounce variables
    last_press_time = 0
    debounce_delay = 0.3  # 300ms debounce

    clear_display()

    try:
        while True:
            # Check if center button is pressed (active low)
            if btn_center.value() == 0:  # Button pressed
                current_time = time.time()

                # Debounce check
                if current_time - last_press_time > debounce_delay:
                    print("MEOW! 🌈 Rainbow ripple triggered!")
                    last_press_time = current_time

                    # Trigger rainbow ripple
                    rainbow_ripple_from_center()

                    # Clear display after ripple
                    clear_display()

                    # Wait for button release
                    while btn_center.value() == 0:
                        time.sleep(0.01)

            # Gentle idle animation while waiting
            button_pressed = idle_animation()
            if button_pressed:
                # Button was pressed during idle, trigger immediately
                current_time = time.time()
                if current_time - last_press_time > debounce_delay:
                    print("MEOW! 🌈 Rainbow ripple triggered!")
                    last_press_time = current_time
                    rainbow_ripple_from_center()
                    clear_display()

                    # Wait for button release
                    while btn_center.value() == 0:
                        time.sleep(0.01)

    except KeyboardInterrupt:
        print("\nStopping...")
        clear_display()
        print("Done! Goodbye! 😺")
