# Keypad, RFID, and Servo Door Lock System
This project is an advanced access control system for a door lock, utilizing an Arduino with a keypad for PIN entry, an MFRC522 RFID reader for card authentication, and a servo motor to act as the locking mechanism. The system also includes an I2C LCD to provide user feedback.

# Components Needed
Arduino board (e.g., Uno)

MFRC522 RFID Reader

4x4 Keypad

Servo Motor

16x2 I2C LCD Display

Jumper wires

Breadboard

5V Power Supply (for the servo)

# Circuit Connections
MFRC522 RFID Reader:

SS_PIN -> Arduino Digital Pin 10

RST_PIN -> Arduino Digital Pin 9

Connect the remaining SPI pins (MOSI, MISO, SCK) to the standard SPI pins on your Arduino.

4x4 Keypad:

Row Pins -> Arduino Digital Pins 5, 6, 7, 8

Column Pins -> Arduino Analog Pins A3, A2, A1, A0

Servo Motor:

Signal Pin -> Arduino Digital Pin 3

VCC -> External 5V supply

GND -> Ground

I2C LCD Display:

SDA -> Arduino SDA pin

SCL -> Arduino SCL pin

VCC -> 5V

GND -> Ground

# Libraries Used
Keypad.h

Servo.h

MFRC522.h

SPI.h

LiquidCrystal_I2C.h

Wire.h

EEPROM.h

# Code Description
The project functions as a secure door lock system with two methods of authentication:

RFID Authentication: The system continuously checks for an RFID card. If a card with a UID matching one of the two pre-defined authorized UIDs is detected, the changeServo() function is called to toggle the servo's position, effectively unlocking the door. The door's state is then printed to the serial monitor.

PIN Authentication: The user can press the 'A' key on the keypad to enter PIN entry mode. The system prompts the user to enter a 4-digit PIN. If the entered PIN matches the correctPIN ("1234"), the servo is moved to position 0, and a "Door is opened" message is printed. If the PIN is incorrect, a message is displayed, and the system resets.

The LCD displays "Smart home Welcome" on startup and then prompts the user to choose between PIN or RFID authentication. The servo can be closed manually by pressing the 'C' key while the door is open.

# Customization
Authorized RFID Cards: To change the authorized RFID cards, you can update the authorizedUID1 and authorizedUID2 arrays. You can use the IR_Receiver.ino project to find the UID of your own card.

Correct PIN: To change the PIN, simply modify the correctPIN constant.

Servo Positions: The changeServo() function toggles the servo between 0 and 90 degrees. You may need to adjust these values to fit your specific servo and lock mechanism.
