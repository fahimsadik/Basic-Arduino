#include <SPI.h>
#include <Wire.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#define RST_PIN 9                  // Configurable, see typical pin layout above
#define SS_PIN 10                  // Configurable, see typical pin layout above
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
const byte numRows = 4;
const byte numCols = 4;
// Create instances
LiquidCrystal_I2C lcd(0x27, 20, 4);  // Set the LCD address to 0x27 for a 16x2 display
const int relayPin = A0;             // Connect the relay to digital pin 8
char keys[4][4] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};
byte rowPins[4] = { 0, A3, 2, 3 };  // Connect to R1, R2, R3, R4
byte colPins[4] = { 4, 5, 6, 7 };   // Connect to C1, C2, C3, C4
Keypad customKeypad = Keypad(makeKeymap(keys), rowPins, colPins, 4, 4);

void setup() {
  Serial.begin(9600);
  SPI.begin();
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);
  mfrc522.PCD_Init();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);           // Set cursor to the first row
  lcd.print("Scan RFID Card");   // Display the message
  lcd.setCursor(0, 1);           // Set cursor to the first row
  lcd.print("Enter D for PIN");  // Display the message
}
void loop() {
  handleKeypad();  // Function to handle custom keypad input
  handleRFID();    // Function to handle RFID card scanning
}
void handleRFID() {
  // Serial.println("OK");
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {

    String cardID = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      cardID += String(mfrc522.uid.uidByte[i], HEX);
    }
    Serial.println(cardID);  // Send the UID over serial
    delay(1000);  // Delay to avoid multiple reads
    if (cardID == "b37855f7") {
      lcd.clear();
      lcd.print("Authorized");
      digitalWrite(relayPin, LOW);   // Turn the relay on
      delay(1000);                   // Wait for 1 second
      digitalWrite(relayPin, HIGH);  // Turn the relay off
      delay(1000);                   // Wait for 1 second
      setup();
    } else {
      lcd.clear();
      lcd.print("Access denied");
      lcd.setCursor(0, 1);
      lcd.print("WRONG RFID");
      delay(3000);
      setup();
    }
  }
}
void handleKeypad() {
  static char enteredPIN[4];       // Array to store the entered PIN
  static byte pinIndex = 0;        // Index to keep track of the current digit
  static bool pinEntered = false;  // Flag to track whether PIN entry is complete
  static bool inPINMode = false;   // Flag to track if we're in PIN entry mode
  char customKey = customKeypad.getKey();
  if (customKey) {
    Serial.println(customKey);
    if (!inPINMode) {
      // Waiting for 'D' key to enter PIN mode
      if (customKey == 'D') {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Enter PIN");
        inPINMode = true;
      }
    } else {
      // PIN entry mode
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Enter PIN");
      // Check if the pressed key is a digit (0-9)
      if (isdigit(customKey) && pinIndex < 4) {
        enteredPIN[pinIndex] = customKey;
        lcd.setCursor(pinIndex, 1);
        for (byte i = 0; i <= pinIndex; i++) {
          lcd.print('*');  // Display an asterisk for each entered digit
        }
        pinIndex++;

        // If all 4 digits are entered, mark PIN entry as complete
        if (pinIndex == 4) {
          pinEntered = true;
        }
      }
      // Check if the user presses 'D' again to verify the PIN
      if (customKey == 'D' && pinEntered) {
        const char correctPIN[] = "1111";  // Replace with your actual correct PIN
        if (memcmp(enteredPIN, correctPIN, 4) == 0) {
          // Correct PIN! Activate the relay or perform other actions.
          lcd.clear();
          lcd.print("Access granted");
          // Add your desired behavior (e.g., unlock a servo, turn on a light, etc.)
          digitalWrite(relayPin, LOW);   // Turn the relay on
          delay(1000);                   // Wait for 1 second
          digitalWrite(relayPin, HIGH);  // Turn the relay off
          delay(2000);                   // Wait for 1 second
          setup();
        } else {
          // Incorrect PIN
          lcd.clear();
          lcd.print("Access denied");
          delay(2000);
          setup();
          // Add any other behavior for incorrect PIN (e.g., sound an alarm, display a warning, etc.)
        }
        // Reset for the next PIN entry
        pinEntered = false;
        pinIndex = 0;
        inPINMode = false;
      }
    }
  }
}