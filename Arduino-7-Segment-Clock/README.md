# 7-Segment Clock with RTC and Rotary Encoder
This project creates a digital clock using two 8x8 LED matrix displays controlled by an LedControl library. It keeps accurate time using a DS1302 Real-Time Clock (RTC) module and features a rotary encoder for adjusting brightness and setting the time.

# Components Needed
Arduino board (e.g., Uno)

MAX7219-based 7-Segment Displays (two modules)

DS1302 RTC Module 



Rotary Encoder with Pushbutton 

Jumper wires

# Circuit Connections
RTC DS1302:

IO -> Arduino Digital Pin 4 

SCLK -> Arduino Digital Pin 5 

CE -> Arduino Digital Pin 2 

7-Segment Displays:

DIN -> Arduino Digital Pin 12 


CLK -> Arduino Digital Pin 10 


CS -> Arduino Digital Pin 11 


Rotary Encoder:

CLK/Pin A -> Arduino Digital Pin 6 


DT/Pin B -> Arduino Digital Pin 7 


SW/Switch -> Arduino Digital Pin 8 


# Libraries Used

LedControl.h 



RtcDS1302.h 

# Code Description
The setup() function initializes the RTC module and the 7-segment displays. It also sets the initial brightness and clears the displays. The loop() function handles two modes: Time/Brightness Mode: The clock displays the current time and date. The rotary encoder's rotation adjusts the display brightness.
Time/Date Setting Mode: Pressing the encoder's pushbutton switches to a time and date setting mode. The encoder rotation increments or decrements the selected value (hours, minutes, day, etc.), and the pushbutton cycles through the settings. The currently selected value blinks to indicate it's being edited. Once all settings are confirmed, the new time is saved to the RTC.
