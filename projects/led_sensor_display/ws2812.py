import math
from time import sleep
from machine import Pin, I2C
from neopixel import NeoPixel

ROWS = 16
COLS = 16
NUM_LEDS = 256
red = (100, 0, 0)
no_color = (0, 0, 0)

def remap_leds():
    remapping = [[0 for _ in range(COLS)] for _ in range(ROWS)]
    
    counter = 0
    for row in range(ROWS):
        for col in range(COLS):
            if (row % 2 == 0):
                remapping[row][col] = counter
            else:
                remapping[row][COLS-1-col] = counter
            counter += 1
    return remapping

def clear(strip):
    for i in range(0, NUM_LEDS):
        strip[i] = no_color

def rgb_color(r, g, b, brightness):
    r = int(r*brightness)
    g = int(g*brightness)
    b = int(b*brightness)
    return (r, g, b)
        
        
def sensor_wave(strip, remap: list, i, brightness, temperature, altitude, humidity):
    """
        Builds an aurora based on 3 sin waves.
    """
    def line1(x, ps):
        return int(ROWS - (3*math.sin((1/5) * x + ps) + 10))
    
    def line2(x, ps):
        return int(ROWS - (3*math.sin((1/5) * x + ps) + 4))
    
    def interpolate_color(c1, c2, t):
        return tuple(int(c1[i] + (c2[i] - c1[i]) * t) for i in range(3))
    
    def temperature_to_color(temperature):
        """
            Get a color on the temperature color scale from a temperature.
        """
        colors = [
            (30,  (0, 0, 255)),     # Deep blue (coldest)
            (50,  (0, 255, 255)),   # Cyan (cool)
            (70,  (0, 255, 0)),     # Green (neutral)
            (80,  (255, 255, 0)),   # Yellow (warm)
            (90,  (255, 128, 0)),   # Orange (hot)
            (110, (255, 0, 0))      # Red (hottest)
        ]

        for i in range(len(colors) - 1):
            h1, c1 = colors[i]
            h2, c2 = colors[i + 1]
            if h1 <= temperature <= h2:
                t = (temperature - h1) / (h2 - h1)
                r, g, b = interpolate_color(c1, c2, t)
                return rgb_color(r, g, b, brightness)
        # Clamp at ends
        return rgb_color(*colors[-1][1], brightness) if temperature > colors[-1][0] else rgb_color(*colors[0][1], brightness)

    def altitude_to_color(altitude):
        """
            Get a color on the altitude color scale from an altitude.
        """
        colors = [
            (0,      (0, 0, 128)),     # Deep ocean blue
            (500,    (0, 70, 200)),    # Coastal blue
            (1000,   (0, 160, 100)),   # Green lowlands
            (2000,   (120, 220, 0)),   # Hills, bright green
            (3000,   (252, 252, 58)),   # Plateau, yellowish
            (5000,   (255, 165, 0)),   # Orange for high terrain
            (7000,   (255, 80, 0)),    # Red-orange for mountains
            (9000,   (180, 30, 30)),   # Dark red for high altitudes
            (11000,  (150, 60, 90)),   # Reddish-purple for near stratosphere
            (15000,  (160, 160, 160)), # Light gray for thin air / frost line
            (20000,  (220, 220, 220)), # Pale gray/white
            (30000,  (255, 255, 255))  # White - snow/ice cap altitudes
        ]
        
        for i in range(len(colors) - 1):
            h1, c1 = colors[i]
            h2, c2 = colors[i + 1]
            if h1 <= altitude <= h2:
                t = (altitude - h1) / (h2 - h1)
                r, g, b = interpolate_color(c1, c2, t)
                return rgb_color(r, g, b, brightness)
        # Clamp at ends
        return rgb_color(*colors[-1][1], brightness) if altitude > colors[-1][0] else rgb_color(*colors[0][1], brightness)

    def humidity_to_color(humidity):
        """
            Get a color on the humidity color scale from a humidity value.
        """
        colors = [
            (0,    (165, 42, 42)),    # Brown – arid air
            (20,   (255, 165, 0)),    # Orange – dry
            (40,   (255, 255, 0)),    # Yellow – borderline dry
            (60,   (0, 255, 0)),      # Green – comfortable
            (80,   (0, 255, 255)),    # Cyan – humid
            (100,  (0, 0, 255))       # Blue – saturated
        ]

        for i in range(len(colors) - 1):
            h1, c1 = colors[i]
            h2, c2 = colors[i + 1]
            if h1 <= humidity <= h2:
                t = (humidity - h1) / (h2 - h1)
                r, g, b = interpolate_color(c1, c2, t)
                return rgb_color(r, g, b, brightness)
        # Clamp at ends
        return rgb_color(*colors[-1][1], brightness) if humidity > colors[-1][0] else rgb_color(*colors[0][1], brightness)

    temp =  temperature_to_color(temperature)
    alt  =  altitude_to_color(altitude)
    hum  =  humidity_to_color(humidity)

    # print(f"Temperature: {temperature}, Altitude: {altitude}, Humidity: {humidity}")

    for row in range(ROWS):
        for col in range(COLS):
            phase_shift = math.floor(i / 4)
            if (row <= line1(col, phase_shift)): # Temperature
                color = temp
            elif (row >= line1(col, phase_shift) and
                  row <= line2(col, phase_shift)): # Altitude
                color = alt  
            elif (row >= line2(col, phase_shift)): # Humidity
                color = hum
            strip[remap[row][col]] = color
    return strip

# remapped_leds = remap_leds()
# pin = Pin(18)

# # Initialize LED object
# strip = NeoPixel(pin, NUM_LEDS)

# i = 0

# while True:
#     if i % 2000 == 0:
#         i = 0 # Reset the counter
#         clear(strip) # Clear less frequently
#     strip = aurora(strip, remapped_leds, i, 0.01, 50, 1000, 10)
#     strip.write() 
#     i += 1
#     sleep(0.3)



