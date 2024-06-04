// Libraries
#include <LiquidCrystal_I2C.h>  // for LCD screen
#include <RTClib.h>             // for real-time clock module
#include <Servo.h>              // for servos
#include <SPI.h>                // for SPI serial communication - for RFID (SPI = Serial Peripheral Interface - for data transmission and collection, synchronization, communication)
#include <MFRC522.h>            // for RFID module

LiquidCrystal_I2C lcd(0x27, 16, 2);  // I2C address 0x27 (from DIYables LCD), 16 columns & 2 rows
RTC_DS3231 rtc;                      // declare an rtc object for the clock
Servo servo1;                        // create a servo object to control servo motor 1
Servo servo2;                        // create a servo object to control servo motor 2
Servo servo3;                        // create a servo object to control servo motor 3

const int BUTTON_PIN = 2;                                // Arduino pin connected to the button pin
const int BUZZER_PIN = 4;                                // Arduino pin connected to the buzzer pin
const int SERVO1_PIN = 7;                                // Arduino pin connected to the servo motor 1 pin
const int SERVO2_PIN = 9;                                // Arduino pin connected to the servo motor 2 pin
const int SERVO3_PIN = 8;                                // Arduino pin connected to the servo motor 3 pin
byte authorizedUID[4] = { 0xEA, 0x6B, 0x0D, 0x80 };      // authorized code for RFID (UID = Unique ID)

// Reset and ss (slave select) / cs (chip select) pins - specific to the SPI protocol
#define SS_PIN 10
#define RST_PIN 5
MFRC522 rfid(SS_PIN, RST_PIN);

bool alarmTriggered = false;  // Indicator to check if the alarm has been triggered (initially off)
bool alarmTriggered2 = false;
bool alarmTriggered3 = false;

int angle1 = 0;  // current angle of the servo motor
int angle2 = 0;
int angle3 = 0;
int buttonState;
bool authorized = false;

void setup() {
  Serial.begin(9600);

  // RFID MODULE CONFIGURATION
  SPI.begin();      // initialize SPI bus
  rfid.PCD_Init();  // initialize MFRC522

  // LCD SCREEN CONFIGURATION
  lcd.init();       // initialize the LCD screen
  lcd.backlight();  // turn on the LCD backlight

  // RTC MODULE CONFIGURATION
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (true)
      ;
  }

  // automatically set RTC to the date and time from the PC at the moment this sketch was compiled
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  pinMode(BUTTON_PIN, INPUT_PULLUP);  // set the button pin to input pull-up mode
  pinMode(BUZZER_PIN, OUTPUT);        // set the buzzer pin to output mode

  servo1.attach(SERVO1_PIN);  // attach servo 1 to pin 9 on servo1 object (signal pin)
  servo1.write(angle1);
  servo2.attach(SERVO2_PIN);  // attach servo 2 to pin 8 on servo2 object
  servo2.write(angle2);
  servo3.attach(SERVO3_PIN);  // attach servo 3 to pin 7 on servo3 object
  servo3.write(angle3);
  buttonState = digitalRead(BUTTON_PIN);
}

void loop() {
  DateTime now = rtc.now();  // read information from the rtc module

  int year = now.year();
  int month = now.month();
  int day = now.day();
  int hour = now.hour();
  int minute = now.minute();
  int second = now.second();

  // display the read data on the LCD screen
  lcd.clear();

  if (alarmTriggered || alarmTriggered2 || alarmTriggered3)  // message corresponding to pill times
  {
    lcd.setCursor(0, 0);  // start writing on the first row
    lcd.print("Take the pill from");
  } else {
    lcd.setCursor(0, 0);  // start writing on the first row
    lcd.print("Date: ");
    lcd.print(day);
    lcd.print("/");
    lcd.print(month);
    lcd.print("/");
    lcd.print(year);
  }

  lcd.setCursor(0, 1);  // start writing on the second row
  lcd.print("Time: ");
  lcd.print(hour);
  lcd.print(":");
  lcd.print(minute);
  lcd.print(":");
  lcd.print(second);

  buttonState = digitalRead(BUTTON_PIN);  // read the new state of the button

  // RFID CODE
  if (rfid.PICC_IsNewCardPresent()) {  // a new tag is available
    if (rfid.PICC_ReadCardSerial()) {  // NUID has been read
      MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);

      if (rfid.uid.uidByte[0] == authorizedUID[0] && rfid.uid.uidByte[1] == authorizedUID[1] && rfid.uid.uidByte[2] == authorizedUID[2] && rfid.uid.uidByte[3] == authorizedUID[3]) {
        Serial.println("Authorized Tag");
        authorized = true;
      } else {  // in case we have an unauthorized tag, display its UID on the serial monitor
        Serial.print("Unauthorized Tag with UID:");
        for (int i = 0; i < rfid.uid.size; i++) {
          Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
          Serial.print(rfid.uid.uidByte[i], HEX);
        }
        Serial.println();
      }

      rfid.PICC_HaltA();       // stop PICC
      rfid.PCD_StopCrypto1();  // stop encryption on PCD
    }
  }

  // ALARM 1 WITH SERVO1
  if (!alarmTriggered && hour == 12 && minute == 46 && second == 30) {  // if alarm 1 is not already activated and it's time 1 for pills, execute:
    digitalWrite(BUZZER_PIN, HIGH);                                      // turn on the buzzer
    tone(BUZZER_PIN, 260);
    alarmTriggered = true;  // set the indicator to true, indicating the alarm has been triggered
  }

  if (alarmTriggered && authorized == true) {  // if alarm 1 is triggered and the tag is authorized
    noTone(BUZZER_PIN);                        // stop the sound
    digitalWrite(BUZZER_PIN, LOW);             // turn off the buzzer
    alarmTriggered = false;                    // reset indicator 1

    // change the servo motor angle, opening the box
    if (angle1 == 0)
      angle1 = 90;
    servo1.write(angle1);  // set the servo to the new angle
  } else if (!alarmTriggered && buttonState == LOW)  // if the alarm was turned off and the button is pressed
  {
    if (angle1 == 90)  // if the box is open, then close it
      angle1 = 0;
    servo1.write(angle1);  // set the servo to the new angle
    authorized = false;
  }

  // ALARM 2 WITH SERVO2
  if (!alarmTriggered2 && hour == 12 && minute == 47 && second == 00) {  // if alarm 2 is not already activated and it's time 2 for pills, execute:
    digitalWrite(BUZZER_PIN, HIGH);                                      // turn on the buzzer
    tone(BUZZER_PIN, 260);
    alarmTriggered2 = true;  // set the indicator to true, indicating the alarm has been triggered
  }

  if (alarmTriggered2 && authorized == true) {  // if alarm 2 is triggered and the tag is authorized
    noTone(BUZZER_PIN);                         // stop the sound
    digitalWrite(BUZZER_PIN, LOW);              // turn off the buzzer
    alarmTriggered2 = false;                    // reset indicator 2

    // change the servo motor angle
    if (angle2 == 0)
      angle2 = 90;
    servo2.write(angle2);  // set the servo to the new angle
  } else if (!alarmTriggered2 && buttonState == LOW)  // if the alarm was turned off and the button is pressed
  {
    if (angle2 == 90)  // if the box is open, then close it
      angle2 = 0;
    servo2.write(angle2);
  }

  // ALARM 3 WITH SERVO3
  if (!alarmTriggered3 && hour == 12 && minute == 47 && second == 30) {  // if alarm 3 is not already activated and it's time 3 for pills, execute:
    digitalWrite(BUZZER_PIN, HIGH);                                      // turn on the buzzer
    tone(BUZZER_PIN, 260);
    alarmTriggered3 = true;  // set the indicator to true, indicating the alarm has been triggered
  }

  if (alarmTriggered3 && authorized == true) {  // if alarm 3 is triggered and the tag is authorized
    noTone(BUZZER_PIN);                         // stop the sound
    digitalWrite(BUZZER_PIN, LOW);              // turn off the buzzer
    alarmTriggered3 = false;                    // reset indicator 3

    // change the servo motor angle
    if (angle3 == 0)
      angle3 = 90;
    servo3.write(angle3);  // set the servo to the new angle
  } else if (!alarmTriggered3 && buttonState == LOW)  // if the alarm was turned off and the button is pressed
  {
    if (angle3 == 90)  // if the box is open, then close it
      angle3 = 0;
    servo3.write(angle3);
  }

  delay(1000);  // update once every second
}
