from machine import Pin, I2C
# from display import Display
import time
import math
import bme280

"""
h = (101,325/P - 1) * (T + 273.15) / 6.5
h is the altitude in meters. 
P0 is the pressure at sea level (reference pressure) in Pascals. 
P is the current pressure in Pascals. 
T is the current temperature in Celsius. 
L is the average temperature lapse rate (approximately 6.5 °C/km). 
"""
SEA_LEVEL_PRESSURE = 1013.25  # in hPa
METERS_TO_FEET = 3.28084

def main():
    # Use I2C0 on GP4 (SDA), GP5 (SCL)
    i2c = I2C(0, scl=Pin(5), sda=Pin(4), freq=100_000)
    # display = Display(i2c)
    # Check if sensor is detected
    devices = i2c.scan()
    if not devices:
        print("No I2C devices found.")
    else:
        print("I2C devices found:", devices)
        bme = bme280.BME280(i2c=i2c)

        while True:
            temp, pres, hum = bme.read_compensated_data()

            temp_k = (temp/100) + 273.15
        
            # Constants
            R = 8.31432       # J/(mol·K)
            g = 9.80665       # m/s²
            M = 0.0289644     # kg/mol

            # Convert pressures to Pascals (if needed)
            P = pres / 25600
            P0 = SEA_LEVEL_PRESSURE

            # Hypsometric formula
            altitude = (R * temp_k) / (g * M) * math.log(P0 / P)
            #altitude = 44330.0 * (1.0 - ((pres / 25600) / SEA_LEVEL_PRESSURE) ** 0.1903)
            print("Temperature: {:.2f} F".format((temp / 100) * 9/5 + 32))
            print("Pressure: {:.2f} hPa".format(pres / 25600))
            print("Humidity: {:.2f} %".format(hum / 1024))
            print("Approximate Altitude: {:.2f} f".format(altitude * METERS_TO_FEET))
            print("-" * 30)

            # display.display(f"Altitude: {int(altitude * METERS_TO_FEET)}FT")
            time.sleep(2)

if __name__ == "__main__":
    main()