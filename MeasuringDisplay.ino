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
  // main system with random spikes filtering
  
  const int SAMPLES = 11;      // We will take 11 smaples (previously 5, but that didnt filter much )
  long readings[SAMPLES];     // An array to store the 5 readings
  long final_distance_cm;     // The final filtered distance

  //READ THE SENSOR 5 TIMES
  for (int i = 0; i < SAMPLES; i++) {
    // Standard Sensor Reading Logic
    long duration;
    digitalWrite(Tgg_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(Tgg_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(Tgg_PIN, LOW);
    
    duration = pulseIn(Echo_PIN, HIGH);
    
    // Check for bad readings (timeout or 0)
    if (duration == 0 || duration > 38000) { // 38000us is 6.5m, a bad reading
      readings[i] = 999; // Give it a high "bad" value to be sorted out
    } else {
      readings[i] = duration / 29 / 2;
    }
    
    delay(25); // Wait 25ms between samples(10 didnt work)
  }

  // filtering the data
  // Sort an array with increasing values
  sortArray(readings, SAMPLES);
  
  // Get the median
  final_distance_cm = readings[SAMPLES / 2];
  
  // writing THE DISPLAY
  display.clearDisplay();
  
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("Distance:");
  
  display.setTextSize(3);
  display.setCursor(0, 25);
  display.print(final_distance_cm); // Print the CLEAN value
  display.print(" cm");
  
  display.display();
  
  //DEBUG
  Serial.print("Raw: [");
  for(int i=0; i < SAMPLES; i++) {
    Serial.print(readings[i]);
    Serial.print(" ");
  }
  Serial.print("] -> Filtered: ");
  Serial.println(final_distance_cm);

  delay(100);
}


//bubblesorting system
void sortArray(long *array, int size) {
  for(int i=0; i < size-1; i++) {
    for(int j=0; j < size-1-i; j++) {
      if(array[j] > array[j+1]) {
        long temp = array[j];
        array[j] = array[j+1];
        array[j+1] = temp;
      }
    }
  }
}
