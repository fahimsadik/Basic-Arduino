#include <LedControl.h>
#include <Arduino.h>
#include <RtcDS1302.h>

// RTC Setup
ThreeWire myWire(4, 5, 2);  // IO, SCLK, CE
RtcDS1302<ThreeWire> Rtc(myWire);

// Encoder and Display
int encoderPinA = 6;
int encoderPinB = 7;
int encoderSwitch = 8;
int brightnessLevel = 5;
int encoderState = 0;
int lastEncoderState = 0;
int mode = 1;  // 1 = Brightness Mode, 2 = Time & Date Setting Mode
int settingStep = 0;

LedControl lc(12, 10, 11, 2);

// Function Prototypes
void displayTime(RtcDateTime now, bool blink);
bool isLeapYear(int year);
int getDaysInMonth(int month, int year);
void adjustTime();

void setup() {
    Serial.begin(9600);
    pinMode(encoderPinA, INPUT);
    pinMode(encoderPinB, INPUT);
    pinMode(encoderSwitch, INPUT_PULLUP);

    lc.shutdown(0, false);
    lc.shutdown(1, false);
    lc.setIntensity(0, brightnessLevel);
    lc.setIntensity(1, brightnessLevel);
    lc.clearDisplay(0);
    lc.clearDisplay(1);

    Rtc.Begin();
    if (!Rtc.IsDateTimeValid()) {
        Serial.println("RTC lost confidence in the DateTime!");
        Rtc.SetDateTime(RtcDateTime(__DATE__, __TIME__));
    }
}

void displayTime(RtcDateTime now, bool blink) {
    int hour24 = now.Hour();
    int hour12 = hour24 % 12;
    if (hour12 == 0) hour12 = 12;
    bool isPM = (hour24 >= 12);
    int minutes = now.Minute();
    int seconds = now.Second();
    int date = now.Day();
    int month = now.Month();
    int year = now.Year() % 100;

    // Display 12-hour time with AM/PM
    lc.setDigit(0, 7, hour12 / 10, false);
    lc.setDigit(0, 6, hour12 % 10, blink);
    lc.setDigit(0, 5, minutes / 10, false);
    lc.setDigit(0, 4, minutes % 10, blink);
    lc.setDigit(0, 3, seconds / 10, false);
    lc.setDigit(0, 2, seconds % 10, blink);
    lc.setChar(0, 1, blink ? '-' : ' ', false);
    lc.setChar(0, 0, isPM ? 'P' : 'A', false);

    // Display date
    lc.setDigit(1, 7, date / 10, false);
    lc.setDigit(1, 6, date % 10, true);
    lc.setDigit(1, 5, month / 10, false);
    lc.setDigit(1, 4, month % 10, true);
    lc.setDigit(1, 3, 2, false);
    lc.setDigit(1, 2, 0, false);
    lc.setDigit(1, 1, year / 10, false);
    lc.setDigit(1, 0, year % 10, false);
}

void adjustTime() {
    static RtcDateTime now = Rtc.GetDateTime();
    static int values[] = {now.Hour(), now.Minute(), now.Day(), now.Month(), now.Year() % 100};

    encoderState = digitalRead(encoderPinA);
    if (encoderState != lastEncoderState) {
        if (digitalRead(encoderPinB) != encoderState) { // Clockwise
            values[settingStep]++;
        } else {  // Counterclockwise
            values[settingStep]--;
        }

        // Apply limits
        switch (settingStep) {
            case 0: // Hours
                if (values[0] > 23) values[0] = 0;
                if (values[0] < 0) values[0] = 23;
                break;
            case 1: // Minutes
                if (values[1] > 59) values[1] = 0;
                if (values[1] < 0) values[1] = 59;
                break;
            case 2: // Day
                if (values[2] > getDaysInMonth(values[3], values[4] + 2000)) values[2] = 1;
                if (values[2] < 1) values[2] = getDaysInMonth(values[3], values[4] + 2000);
                break;
            case 3: // Month
                if (values[3] > 12) values[3] = 1;
                if (values[3] < 1) values[3] = 12;
                break;
            case 4: // Year
                if (values[4] > 99) values[4] = 0;
                if (values[4] < 0) values[4] = 99;
                break;
        }
    }
    lastEncoderState = encoderState;

    // Display blinking values
    bool blink = millis() % 1000 < 500;
    switch (settingStep) {
        case 0: { // 12-hour format
            int hour12 = values[0] % 12;
            if (hour12 == 0) hour12 = 12;
            bool isPM = (values[0] >= 12);
            lc.setDigit(0, 7, hour12 / 10, blink);
            lc.setDigit(0, 6, hour12 % 10, blink);
            lc.setChar(0, 0, isPM ? 'P' : 'A', false);
            break;
        }
        case 1:
            lc.setDigit(0, 5, values[1] / 10, blink);
            lc.setDigit(0, 4, values[1] % 10, blink);
            break;
        case 2:
            lc.setDigit(1, 7, values[2] / 10, blink);
            lc.setDigit(1, 6, values[2] % 10, blink);
            break;
        case 3:
            lc.setDigit(1, 5, values[3] / 10, blink);
            lc.setDigit(1, 4, values[3] % 10, blink);
            break;
        case 4:
            lc.setDigit(1, 1, values[4] / 10, blink);
            lc.setDigit(1, 0, values[4] % 10, blink);
            break;
    }

    if (digitalRead(encoderSwitch) == LOW) {
        settingStep++;
        if (settingStep > 4) {
            Rtc.SetDateTime(RtcDateTime(values[4] + 2000, values[3], values[2], values[0], values[1], 0));
            settingStep = 0;
            mode = 1;
        }
        delay(300);
    }
}

bool isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int getDaysInMonth(int month, int year) {
    if (month == 2) return isLeapYear(year) ? 29 : 28;
    return (month == 4 || month == 6 || month == 9 || month == 11) ? 30 : 31;
}

void loop() {
    if (digitalRead(encoderSwitch) == LOW) {
        mode = (mode == 1) ? 2 : 1;  
        delay(300);
    }

    switch (mode) {
        case 1:
            encoderState = digitalRead(encoderPinA);
            if (encoderState != lastEncoderState) {
                brightnessLevel += (digitalRead(encoderPinB) != encoderState) ? 1 : -1;
                brightnessLevel = constrain(brightnessLevel, 0, 15);
                lc.setIntensity(0, brightnessLevel);
                lc.setIntensity(1, brightnessLevel);
            }
            lastEncoderState = encoderState;
            displayTime(Rtc.GetDateTime(), millis() % 1000 < 500);
            break;
        case 2:
          
            adjustTime();
            break;
    }
}

