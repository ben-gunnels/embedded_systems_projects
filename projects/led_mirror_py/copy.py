import board
import time
import neopixel
import digitalio

# Setup for Onboard LED (GP25 for Pico)
led = digitalio.DigitalInOut(board.LED)
led.direction = digitalio.Direction.OUTPUT

# Flash the onboard LED three times at startup
for _ in range(3):
    led.value = True
    time.sleep(0.2)
    led.value = False
    time.sleep(0.2)

# Set the NeoPixel configuration
pixel_pin = board.GP15  # GPIO pin connected to the data line
num_pixels = 16         # Number of LEDs in your strip or matrix

# Create NeoPixel object
pixels = neopixel.NeoPixel(pixel_pin, num_pixels, brightness=0.5, auto_write=False)

# Fill with red color
pixels.fill((255, 0, 0))  # Red color
pixels.show()
time.sleep(2)

# Rainbow Animation
def rainbow_cycle(wait):
    for j in range(255):
        for i in range(num_pixels):
            pixel_index = (i * 256 // num_pixels) + j
            pixels[i] = wheel(pixel_index & 255)
        pixels.show()
        time.sleep(wait)

# Color Wheel Function
def wheel(pos):
    if pos < 85:
        return (pos * 3, 255 - pos * 3, 0)
    elif pos < 170:
        pos -= 85
        return (255 - pos * 3, 0, pos * 3)
    else:
        pos -= 170
        return (0, pos * 3, 255 - pos * 3)

# Run Rainbow Animation
while True:
    rainbow_cycle(0.01)

