# Arduino-PIR-LCD
An Arduino project that uses a Passive Infrared (PIR) sensor to detect motion and display the status on an I2C LCD. 

This project uses a PIR (Passive Infrared) sensor to detect motion and displays the status on an I2C Liquid Crystal Display (LCD). When motion is detected, a message is shown on the LCD and printed to the serial monitor. When no motion is detected, a different message is displayed.

Components Needed
Arduino board (e.g., Uno, Nano)

PIR Motion Sensor 

16x2 I2C LCD Display 

Jumper wires


Circuit Connections
PIR Sensor OUT pin -> Arduino Digital Pin 4 

I2C LCD SCL -> Arduino SCL pin

I2C LCD SDA -> Arduino SDA pin

I2C LCD VCC -> Arduino 5V

I2C LCD GND -> Arduino GND


Libraries Used
Wire.h (Standard Arduino library for I2C communication) 

LiquidCrystal_I2C.h (for the I2C LCD) 

Code Description
The code initializes the I2C LCD and sets the PIR sensor pin as an input in the setup() function. 

In the loop(), it continuously reads the state of the PIR sensor. If motionDetected is HIGH, it prints "Motion detected!" on both the serial monitor and the LCD. If the state is LOW, it prints "Motion not detected!" to the serial monitor and "No motion" on the LCD.
