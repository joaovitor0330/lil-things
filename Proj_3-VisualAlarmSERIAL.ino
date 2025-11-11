const int TggPin = 2;
const int EchoPin = 3;
      int red1 = 13;

void setup() {
  // initialize serial communication:
  pinMode(EchoPin, INPUT);
  pinMode(TggPin, OUTPUT);
  pinMode(red1, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // establish variables for duration of the ping, and the distance result
  // in inches and centimeters:
  long duration, inches, cm;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(TggPin, LOW);
  delayMicroseconds(2);
  digitalWrite(TggPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TggPin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH pulse
  // whose duration is the time (in microseconds) from the sending of the ping
  // to the reception of its echo off of an object.
  
  duration = pulseIn(EchoPin, HIGH);

  // convert the time into a distance
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);

  Serial.print(inches);
  Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();


  //LED integration, like a car sensor
  if (cm < 7){
    digitalWrite(red1, HIGH);
    delay(25);
    digitalWrite(red1, LOW);
    delay(25);
  }
    else if(cm < 15){
    digitalWrite(red1, HIGH);
    delay(150);
    digitalWrite(red1, LOW);
    delay(150);
    }
    else{
    digitalWrite(red1,LOW);
    }
  //end of LED lines

  delay(100);
}

long microsecondsToInches(long microseconds) {
  // According to Parallax's datasheet for the PING))), there are 73.746
  // microseconds per inch (i.e. sound travels at 1130 feet per second).
  // This gives the distance travelled by the ping, outbound and return,
  // so we divide by 2 to get the distance of the obstacle.
  // See: https://www.parallax.com/package/ping-ultrasonic-distance-sensor-downloads/
      //d=(Vsound*𝚫t)/2
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the object we
  // take half of the distance travelled.
  return microseconds / 29 / 2;
}

