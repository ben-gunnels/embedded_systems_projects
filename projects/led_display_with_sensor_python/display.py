from time import sleep
from machine import I2C, Pin
from lcd_api import LcdApi
from pico_i2c_lcd import I2cLcd

class Display:
    def __init__(self, i2c):
        lcd_address = 0x27
        self.lcd = I2cLcd(i2c, lcd_address, 2, 16)
            
    def display(self, message):
        self.lcd.clear()
        self.lcd.putstr(message[:16])
