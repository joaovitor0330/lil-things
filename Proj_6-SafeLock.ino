#include <Keypad.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


//messingAROUND //this!!
int green = 10;
int red = 11;
char lastkeypressed = ' ';    //storing last key pressed
const int PIN_LENGTH = 4;
char typedPin[PIN_LENGTH + 1];
int pinIndex = 0; //
char SECRET_PIN[PIN_LENGTH + 1] = "4590"; //PASSWORD
int errorIndex = 0;//


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'},
};
byte rowPins[ROWS] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad (this example used the 3x3, adjusted to work with 4x4)
byte colPins[COLS] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup(){
  Serial.begin(9600);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    // tries to turn display on and repeats
    while (true); 
  }



  //messingAROUND //this!!
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);


}
  

 void loop() {
  char key = keypad.getKey();

  if (errorIndex >= 3) {
    lockoutSystem(); // Run the lockout
    errorIndex = 0;  // Reset the counter
  }
else{
  //updating if key is pressed
  if (key) {
    lastkeypressed = key; //testing from last project
    Serial.println(key);
    
    // Not counting letters
    if ( (key >= '0' && key <= '9') && (pinIndex < PIN_LENGTH) ) {
      typedPin[pinIndex] = key; // storing number in memory
      pinIndex++;               // +1 to the counter
    }
    
    // clear key
    if (key == '*') {
      pinIndex = 0; // Reset the counter
      Serial.println("PIN Cleared");
    }

    // Enter key
    if (key == '#') {
      Serial.println("PIN Entered. Checking...");
      
      // Add a null terminator to our typedPin for comparison
      typedPin[pinIndex] = '\0';
      
      
      //
      if ( checkPassword() ) {
        // correct passowrd
        digitalWrite(green, HIGH);
        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(WHITE);
        display.setCursor(0, 10);
        display.print("ACCESS");
        display.setCursor(0, 30);
        display.print("GRANTED");
        display.display();
        delay(2000); // Show for 2 seconds
        digitalWrite(green, LOW);
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
        delay(2000); // Show for 2 seconds
        digitalWrite(red, LOW);
        errorIndex++;

      }
      // After checking, reset for the next try
      pinIndex = 0;
      //
      
    } // 
    
  } // 
  
  

  display.clearDisplay();
  display.setCursor(0, 0); // Top (yellow) part
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.print("Enter PIN:");

  display.setCursor(0, 25); // Bottom (blue) part
  display.setTextSize(2);
  for (int i = 0; i < pinIndex; i++) {
    display.print("*");
  }
  
  display.display(); 
  
  delay(50);

if (errorIndex >= 3){ //LOCKED OUT
  lockoutSystem();
  errorIndex = 0;



}
}
}

bool checkPassword() {
  // 'strcmp' is a C++ function that compares two char arrays (strings).
  // It returns 0 if they are identical.
  if (strcmp(typedPin, SECRET_PIN) == 0) {
    return true;
  } else {
    return false;
  }
}

void lockoutSystem() {
  Serial.println("SYSTEM LOCKED FOR 30 SECONDS!");
  
  // Show "LOCKED" message on the OLED
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.print("SYSTEM");
  display.setCursor(0, 30);
  display.print("LOCKED");
  display.display();
  
  // 
  // lockout operation
  for (int i = 0; i < 15; i++) {
    digitalWrite(red, HIGH);
    delay(1000);
    digitalWrite(red, LOW);
    delay(1000);
  }
}


