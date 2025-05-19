import board
import time
import random
import math
from neopixel import NeoPixel

ROWS = 16
COLS = 16
led_count = 256
red = (100, 0, 0)
no_color = (0, 0, 0)

strip = NeoPixel(board.GP18, led_count, auto_write=False)

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

def clear():
    for i in range(0, led_count):
        strip[i] = no_color
    strip.show()

def manage_brightness(color, brightness):
    r = int(color[0]*brightness)
    g = int(color[1]*brightness)
    b = int(color[2]*brightness)
    return (r, g, b)

# Function to simulate PWM by adjusting brightness
def pwm_brightness(color, brightness, duration=2):
    brightness = max(0, min(1, brightness))  # Constrain brightness between 0.0 and 1.0

    for _ in range(int(duration * 50)):  # 50 cycles per second
        # On cycle (brightness % of the time)
        for i in range(NUM_LEDS):
            np[i] = (
                int(color[0] * brightness),
                int(color[1] * brightness),
                int(color[2] * brightness)
            )
        np.write()
        sleep(brightness / 50)  # On time

        # Off cycle (remaining time)
        for i in range(NUM_LEDS):
            np[i] = (0, 0, 0)
        np.write()
        sleep((1 - brightness) / 50)  # Off time
        
def big_unit(frame, brightness):
    yellow = manage_brightness((255, 255, 0), 0.3)
    def left_nut(x, y):
        return (x-4)*(x-4) + (y-12)*(y-12), 9 # Bounds, radius
    
    def right_nut(x, y):
        return (x-10)*(x-10) + (y-12)*(y-12), 9 # Bounds, radius
    
    def shaft(x, y):
        first_term = ((x-7)*(x-7)) / 4
        second_term = ((y-4)*(y-4)) / 25
        return first_term + second_term
    
    for row in range(ROWS):
        for col in range(COLS):
            ln, rad1 = left_nut(row, col)
            rn, rad2 = right_nut(row, col)
            sh = shaft(row, col)
            if ln <= rad1 or rn <= rad2 or sh <= 1:
                strip[frame[row][col]] = yellow
        
def aurora(frame: list, i, brightness=1.0):
    """
        Builds an aurora based on 3 sin waves.
    """
    background_blue_dark = manage_brightness((4, 3, 9), brightness)
    majestic_purple      = manage_brightness((9, 7, 23), brightness)
    pink 				 = manage_brightness((26, 2, 16), brightness)
    aurora_green 		 = manage_brightness((5, 24, 17), brightness)
    bright_green 		 = manage_brightness((13, 23, 13), brightness)
    
    def line1(x, ps):
        return int(ROWS - (3*math.sin((1/5) * x + ps) + 13))
    
    def line2(x, ps):
        return int(ROWS - (3*math.sin((1/8) * x + ps) + 10))
    
    def line3(x, ps):
        return int(ROWS - (3*math.sin((1/7) * x + ps) + 7))
    
    def line4(x, ps):
        return int(ROWS - (3*math.sin((1/4) * x + ps) + 5))
    
    def line5(x, ps):  
        return int(ROWS - (3*math.sin((1/10) * x + ps) + 3))

    for row in range(ROWS):
        for col in range(COLS):
            phase_shift = math.floor(i / 4)
            if (row <= line1(col, phase_shift) or # background blue above line 1, below line 5 and between lines 2 and 3
                row >= line5(col, phase_shift)):
                color = background_blue_dark
            elif (row >= line1(col, phase_shift) and
                  row <= line2(col, phase_shift)):
                color = aurora_green
            elif (row >= line2(col, phase_shift) and 
                 row <= line3(col, phase_shift)):
                color = pink
            elif (row >= line3(col, phase_shift) and
                  row <= line4(col, phase_shift)):
                color = bright_green
            elif (row >= line4(col, phase_shift) and
                  row <= line5(col, phase_shift)):
                color = majestic_purple

            strip[frame[row][col]] = color
            
def noahs_rainbow(frame, i, brightness):
    # Defining Rainbow Colors with Brightness Control (0.3 = 30% brightness)
    red = manage_brightness((255, 0, 0), brightness)
    orange = manage_brightness((255, 165, 0), brightness)
    yellow = manage_brightness((255, 255, 0), brightness)
    green = manage_brightness((0, 255, 0), brightness)
    blue = manage_brightness((0, 0, 255), brightness)
    indigo = manage_brightness((75, 0, 130), brightness)
    violet = manage_brightness((138, 43, 226), brightness)
    
    rainbow = [red, indigo, orange, green, blue, yellow, violet]
    
    current = i % 7 # Determines the color of the first column
    
    def increment_current(current):
        if current == 6:
            current = 0
        else:
            current += 1
        return current
    
    for row in range(ROWS):
        for col in range(COLS):
            strip[frame[row][col]] = rainbow[current]
        current = increment_current(current)
             
def melancholic_rain(frame: list, j, brightness):
    dark_blue = manage_brightness((20, 20, 20), brightness)
    dark_purple = manage_brightness((82, 35, 252), brightness)

    raindrop_width = 1

    starting_column = random.randint(0, COLS)

    for i in range(55):
        raindrop_size = math.floor(min(0.1 * math.pow(1.3, i), 4)) # Grow the raindrop exponentially based on the frame, the rate is 1.3
        if raindrop_size < 4:
            for row in range(ROWS):
                for col in range(COLS):
                    column_condition = col >= starting_column and col <= starting_column + raindrop_width

                    if (column_condition and row <= raindrop_size):
                        strip[frame[row][col]] = dark_purple
                    
                    else:
                        strip[frame[row][col]] = dark_blue
        else:
            raindrop_position = int(math.pow(1.5, math.floor(i / 5)))
            for row in range(ROWS):
                for col in range(COLS):
                    column_condition = col >= starting_column and col <= starting_column + raindrop_width

                    if (column_condition and row in range(raindrop_position, raindrop_position + raindrop_size)):
                        strip[frame[row][col]] = dark_purple
                    else:
                        strip[frame[row][col]] = dark_blue
        strip.show()
        time.sleep(0.01)
        
def police_lights(frame: list, i, brightness):
    red = manage_brightness((255,0,0), brightness)
    white = manage_brightness((255, 255, 255), brightness)
    blue = manage_brightness((0, 0, 255), brightness)
    
    for row in range(ROWS):
        for col in range(COLS):
            choice = random.randrange(3)
            if choice == 0:
                strip[frame[row][col]] = red
            elif choice == 1:
                strip[frame[row][col]] = white
            else:
                strip[frame[row][col]] = blue
                
    
        
def chessboard(frame: list, i, brightness):
    forest_emerald = manage_brightness((0, 220, 30), brightness)
    crimson_velvet = manage_brightness((120, 0, 20), brightness)

    choose = i % 2

    for row in range(ROWS):
        for col in range(COLS):         
            if not (math.floor(row / 2) % 2 == math.floor(col / 2) % 2):
                if choose:
                    color = forest_emerald
                else:
                    color = crimson_velvet
            else:
                if choose:
                    color = crimson_velvet
                else:
                    color = forest_emerald

            strip[frame[row][col]] = color
            
def beating_heart(frame: list, i, brightness):
    red = manage_brightness((255, 0, 0), brightness)
    baby_blue = manage_brightness((137, 207, 240), brightness)

    def left_atrium(x, expansion):
        return (1/2)*(x-5)*(x-7) + 5 - expansion
    
    def right_atrium(x, expansion):
        return (1/2)*(x-11)*(x-9) + 5 - expansion
    
    def left_ventricle(x, expansion):
        return (-3/2)*x + 24 + expansion
    
    def right_ventricle(x, expansion):
        return (3/2)*x + expansion
    
    # Whether to initiate a heart expansion/contraction during these frames,
    # Even quotients of 4 are the beating phase
    beating_phase = not math.floor(i / 4) % 2
    expansion = int(beating_phase & i % 2) # Has to be in beating phase and odd index

    for row in range(ROWS):
        for col in range(COLS):
            la = left_atrium(col, expansion)
            ra = right_atrium(col, expansion)
            lv = left_ventricle(col, expansion)
            rv = right_ventricle(col, expansion)
            if ((row >= la or
                row >= ra) and
                (row <= lv and
                row <= rv)):
                strip[frame[row][col]] = red
            else:
                strip[frame[row][col]] = baby_blue

    
remapped_leds = remap_leds()

# for i in range(100):
i = 0
while True:
    if i % 2000 == 0:
        i = 0 # Reset the counter
        clear() # Clear less frequently
    beating_heart(remapped_leds, i, 0.01)
    strip.show() 
    i += 1
    time.sleep(0.3)
    #time.sleep(0.5)



