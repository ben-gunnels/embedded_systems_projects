import board
import neopixel
import time

# Set up the WS2812B LEDs
num_pixels = 256
pixel_pin = board.D18

pixels = neopixel.NeoPixel(pixel_pin, num_pixels, auto_write=False)
pixels.fill((255, 0, 0))  # Set all pixels to red
pixels.show()
time.sleep(2)
pixels.fill((0, 0, 0))  # Turn off LEDs
pixels.show()