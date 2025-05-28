from machine import Pin, I2C
i2c = I2C(0, scl=Pin(5), sda=Pin(4))  # I2C0 on GP5/SCL and GP4/SDA
print("Scanning I2C bus...")
print("Devices found:", i2c.scan())