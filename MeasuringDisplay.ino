#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

const int Tgg_PIN = 2;
const int Echo_PIN = 3;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

// preventing USB powering errors
  delay(1000);

  pinMode(Tgg_PIN, OUTPUT);
  pinMode(Echo_PIN, INPUT);


  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    // tries to turn display on and repeats
    while (true); 
  }

}

void loop() {
  // put your main code here, to run repeatedly:

  long duration, cm; // Distance variable

//starts the ultrasound
  digitalWrite(Tgg_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(Tgg_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(Tgg_PIN, LOW);

duration = pulseIn(Echo_PIN, HIGH);

// find out distance by the time
  // Formula: D=Vsom*T/2
  // time / 29 / 2
cm = duration / 29 / 2;

//starting the display by clearing it
display.clearDisplay();

//Using the display
// Set font for the "header" (Top yellow part)
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);      // Column 0, Row 0
  display.print("Distance:");

  display.setTextSize(3);       // Bigger font
  display.setCursor(0, 25);     // Column 0, Row 25
  display.print(cm);   // Print the calculated value
  display.print(" cm");

  display.display(); //Sends the created image to the display
  delay(100);




  //DEBUG TAB
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();

}
