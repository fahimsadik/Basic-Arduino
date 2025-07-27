#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // Adjust the address if needed

const int pirPin = 4; // Pin connected to PIR sensor OUT

void setup() {
    Serial.begin(9600);
    lcd.init(); // Initialize the LCD
    lcd.backlight(); // Turn on the backlight
    lcd.setCursor(0, 0);
    lcd.print("Ready for action!");
    pinMode(pirPin, INPUT);
}

void loop() {
    int motionDetected = digitalRead(pirPin);
    if (motionDetected == HIGH) {
      Serial.println("Motion detected!");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Motion detected!");
        // You can add more info or actions here
        delay(1000);
    } else {
        lcd.clear();
        Serial.println("Motion not detected!");
        lcd.setCursor(0, 0);
        lcd.print("No motion");
        // Maybe display the time or other info here
        delay(1000);
    }
    // Add any other code you need to run continuously here
}
