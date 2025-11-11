int red1 = 2;

void setup() {
  // put your setup code here, to run once:
    pinMode(red1, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(red1, HIGH); // could be digitalWrite(red1,!digitalRead(red1));
  delay(1000);
  digitalWrite(red1, LOW);
  delay(1000);
}
