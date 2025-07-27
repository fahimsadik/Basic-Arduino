#include <Keypad.h>
#include <Servo.h>
#include <MFRC522.h>
#include <SPI.h>
#include <LiquidCrystal_I2C.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include <EEPROM.h>


// Create instances
MFRC522 rfid(10, 9);  // SS_PIN, RST_PIN
// Define the connections of the keypad
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};
byte rowPins[ROWS] = { 5, 6, 7, 8 };
byte colPins[COLS] = { A3, A2, A1, A0 };

// Initialize the keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Define the servo
Servo myServo;
int pos = 0;                // Variable to store the servo position
bool doorOpen = false;      // Flag to track if the door is open
bool pinEntryMode = false;  // Flag for PIN entry

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Initialize the LCD (change address if needed)
// Initialize the I2C LCD display


// Define authorized UID (change this to match your tag)

byte authorizedUID1[] = {0x53, 0xDB, 0xC8, 0x2F};
byte authorizedUID2[] = {0xB3, 0x78, 0x55, 0xF7};

// Correct PIN code
const char correctPIN[] = "1234";
char enteredPIN[5];  // To store entered PIN
int index = 0;       // Index for storing PIN

void setup() {

  Serial.begin(9600);  // Start serial communication at baud rate of 9600
  // Initialize RFID module and servo
  //SPI.begin();        // Init SPI bus
  rfid.PCD_Init();    // Init MFRC522
  myServo.attach(3);  // Attach servo on pin 10
  lcd.init();
  lcd.backlight();  // Turn on the backlight (if supported)
  Serial.println("Welcome! Press 'A' to enter PIN.");

  // Print a welcome message on the LCD
  lcd.setCursor(0, 0);
  lcd.print("Smart home");
  lcd.setCursor(0, 1);
  lcd.print("Welcome");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Press A for PIN");
  lcd.setCursor(0, 1);
  lcd.print("Press B for RFID");

  //Serial.println("Welcome! Press 'b' to enter PIN.");
}


void loop() {
  if (rfid.PICC_IsNewCardPresent()) {
    if (rfid.PICC_ReadCardSerial()) {
      MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);

      if (memcmp(rfid.uid.uidByte, authorizedUID1, rfid.uid.size) == 0 || memcmp(rfid.uid.uidByte, authorizedUID2, rfid.uid.size) == 0) {
        Serial.println("Authorized Tag");
        changeServo();
      } else {
        Serial.print("Unauthorized Tag with UID: ");
        for (int i = 0; i < rfid.uid.size; i++) {
          Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
          Serial.print(rfid.uid.uidByte[i], HEX);
        }
        Serial.println();
      }

      rfid.PICC_HaltA();
      rfid.PCD_StopCrypto1();
    }
  }
 
  //Serial.println("Welcome! Press 'c' to enter PIN.");
  char key = keypad.getKey();
  //Serial.println("Welcome! Press 'd' to enter PIN.");

  if (key) {
    if (key == 'A') {
      //Serial.println("Welcome! Press 'c' to enter PIN.");
      index = 0;
      memset(enteredPIN, 0, sizeof(enteredPIN));
      pinEntryMode = true;  // Start PIN entry mode
      Serial.println("Enter PIN");
    }


    else if (pinEntryMode) {
      // Handle key presses during PIN entry
      if (key == 'D') {
        enteredPIN[index] = '\0';  // Null-terminate enteredPIN
        if (!strcmp(enteredPIN, correctPIN)) {
          // Correct PIN entered

          pos = 0;
          myServo.write(pos);

          doorOpen = true;
          Serial.println("Door is opened");



        } else {
          Serial.println("Incorrect PIN");
          Serial.println("Press'A' and Try Again ");
          index = 0;  // Reset index if PIN is incorrect
        }
        pinEntryMode = false;  // Reset PIN entry mode
      } else {
        enteredPIN[index] = key;  // Store key in enteredPIN
        index++;                  // Increment index
        // Display the entered key
        Serial.print("Entered key: ");
        Serial.println(enteredPIN);
      }
    } else if (doorOpen && key == 'C') {  // If door is open and 'C' button is pressed
      myServo.write(90);                  // Close the door immediately
      doorOpen = false;                   // Reset the door open flag
      Serial.println("Door is closed");
      index = 0;
    }
  }

}
void changeServo() {
  // Change angle of servo motor
  if (pos == 0)
    pos = 90;
  else
    pos = 0;
  
  // Control servo motor according to the angle
  myServo.write(pos);
  Serial.print("Rotate Servo Motor to ");
  Serial.print(pos);
  Serial.println("°");
}
