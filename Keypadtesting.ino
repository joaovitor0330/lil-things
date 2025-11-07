#include <Keypad.h>

//messingAROUND //this!!
//int red1 = 10;
char lastkeypressed = ' ';    //storing last key pressed



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



  //messingAROUND //this!!
 // pinMode(red1, OUTPUT);


}
  
void loop(){
  char key = keypad.getKey();
  
  if (key){
    lastkeypressed = key;
    Serial.println(key);
  }
 // if (lastkeypressed=='5'){
   // digitalWrite(red1, HIGH);
    
  //}
  //  else{
   // digitalWrite(red1, LOW);
 // }
  delay(75);
   
      
}

