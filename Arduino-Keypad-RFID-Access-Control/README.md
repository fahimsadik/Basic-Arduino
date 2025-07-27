# Keypad and RFID Access Control System

This project is an access control system that uses a combination of an MFRC522 RFID reader and a 4x4 keypad. It allows access by either scanning a specific RFID card or entering a correct 4-digit PIN. The system controls a relay that can be used to open a door or activate a device.

# Components Needed

Arduino board (e.g., Uno)

MFRC522 RFID Reader 

4x4 Keypad 

16x2 I2C LCD Display 

Relay Module 

Jumper wires and breadboard

# Circuit Connections

Keypad: Connect rows to digital pins 0, A3, 2, 3 and columns to digital pins 4, 5, 6, 7 on the Arduino.


RFID: Connect the MFRC522 SS_PIN to digital pin 10 and RST_PIN to digital pin 9. The other SPI pins (MOSI, MISO, SCK) connect to the standard SPI pins on the Arduino.

LCD: Connect SCL and SDA to the corresponding pins on the Arduino.


Relay: Connect the relay's control pin to Arduino analog pin A0.



# Libraries Used

SPI.h (Standard Arduino library for SPI communication) 



Wire.h (Standard Arduino library for I2C communication) 


MFRC522.h (for the RFID reader) 


LiquidCrystal_I2C.h (for the I2C LCD) 



Keypad.h (for the keypad) 


# Code Description
The code handles two primary modes of authentication:


RFID Authentication: The handleRFID() function continuously checks for a new card. If a card with the ID "b37855f7" is scanned, it displays "Authorized" on the LCD, activates the relay, and then deactivates it after a delay. Otherwise, it displays "Access denied".

PIN Authentication: The handleKeypad() function manages PIN entry. Pressing the 'D' key enters PIN mode. Users can then enter a 4-digit PIN, which is displayed as asterisks on the LCD. If the entered PIN matches the correct PIN ("1111") and the 'D' key is pressed again, access is granted, and the relay is activated. An incorrect PIN results in a "Access denied" message.
