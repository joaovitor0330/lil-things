#include <Keypad.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Servo.h>

// Millis() Timer Variables
unsigned long lockoutStartTime;
unsigned long lastBlinkTime;
const long lockoutDuration = 30000; // 30 seconds
const long blinkInterval = 500;     // 0.5 second blink

// Component Pins
int green = 10;
int red = 11;
int servoPin = 12;

// PIN Variables
char lastkeypressed = ' ';
const int PIN_LENGTH = 4;
char typedPin[PIN_LENGTH + 1];
int pinIndex = 0;
char SECRET_PIN[PIN_LENGTH + 1] = "4590"; //PASSWORD
int errorIndex = 0;
char MASTER_KEY = 'A';
char LOCKKEY = 'D';

// Display Config
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Keypad Config
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'},
};
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

Servo myServo;
void setup(){
  Serial.begin(9600);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    while (true); 
  }
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);

  myServo.attach(servoPin); // Tell the servo it's on pin 12
  myServo.write(0);         // Start in "LOCKED" position (0 degrees)
}


void loop() {
  // Read the keypad *every* loop, no matter what
  char key = keypad.getKey();

  // Check state: locked or not?
  if (errorIndex >= 3) {
    // We are locked. Run the lockout handler
    // AND pass the key to it.
    handleLockout(key);
  } else {
    // We are NOT locked. Run the normal keypad handler
    // AND pass the key to it.
    handleKeypad(key);
    
  }
}


void handleKeypad(char key) {

  
  if (key) {
    lastkeypressed = key;
    Serial.println(key); // debug
    
    // key is a number and PIN is not full
    if ( (key >= '0' && key <= '9') && (pinIndex < PIN_LENGTH) ) {
      typedPin[pinIndex] = key;
      pinIndex++;
    }
    
    // clear key
    if (key == '*') {
      pinIndex = 0;
      Serial.println("PIN Cleared");
    }

    // Enter key
    if (key == '#') {
      Serial.println("PIN Entered. Checking...");
      typedPin[pinIndex] = '\0'; // null-terminate the string
      
      if ( checkPassword() ) {
        // correct password
        myServo.write(90);

        digitalWrite(green, HIGH);
        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(WHITE);
        display.setCursor(0, 10);
        display.print("ACCESS");
        display.setCursor(0, 30);
        display.print("GRANTED");
        display.display();
        delay(2000);
        digitalWrite(green, LOW);

        myServo.write(0);
        
        errorIndex = 0; // Reset error count on success
      } 
      else {
        // wrong password
        digitalWrite(red, HIGH);
        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(WHITE);
        display.setCursor(0, 10);
        display.print("ACCESS");
        display.setCursor(0, 30);
        display.print("DENIED");
        display.display();
        delay(2000);
        digitalWrite(red, LOW);
        
        errorIndex++; // Increment error counter

        // Check if this is the 3rd strike
        if (errorIndex >= 3) {
          // Set the START time for our timers
          lockoutStartTime = millis(); 
          lastBlinkTime = millis();
          
          // Show "LOCKED" message ONCE
          display.clearDisplay();
          display.setTextSize(2);
          display.setTextColor(WHITE);
          display.setCursor(0, 10);
          display.print("SYSTEM");
          display.setCursor(0, 30);
          display.print("LOCKED");
          display.display();
          
          digitalWrite(red, HIGH); // Turn LED on for first blink

          return;
        }
      }
      pinIndex = 0; // Reset for next try
    } 
  } 
  
  // Constant display (Enter PIN)
  display.clearDisplay();
  display.setCursor(0, 0); 
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.print("Enter PIN:");
  display.setCursor(0, 25);
  display.setTextSize(2);
  for (int i = 0; i < pinIndex; i++) {
    display.print("*");
  }
  display.display(); 
  delay(50);
}


bool checkPassword() {
  if (strcmp(typedPin, SECRET_PIN) == 0) {
    return true;
  } else {
    return false;
  }
}


void handleLockout(char key) {
  unsigned long agora = millis(); // Get current time

  //master KEY check
  if (key == MASTER_KEY) {
    errorIndex = 0;       // UNLOCK the system
    digitalWrite(red, LOW); // Ensure red LED is off
    
    //"UNLOCKED" message
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    display.print("ADMIN");
    display.setCursor(0, 30);
    display.print("OVERRIDE");
    display.display();
    delay(2000); // This delay is OK, it's a one-time event
    
    return; // Exit the lockout function immediately
  }
  


  // TIMER 1: Check the 30-second TOTAL duration
  if (agora - lockoutStartTime >= lockoutDuration) {
    errorIndex = 0;       // UNLOCK the system
    digitalWrite(red, LOW); // Ensure red LED is off
  }

  // TIMER 2: Check the 0.5-second BLINKER
  if (agora - lastBlinkTime >= blinkInterval) {
    lastBlinkTime = agora; // Reset the blink timer
    digitalWrite(red, !digitalRead(red)); // Toggle the LED
  }
}