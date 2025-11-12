/*
 * Project 09 - Bluetooth LED Control
 */

int green = 10;
char command; // Variable to store the command from the phone

void setup() {
  pinMode(green, OUTPUT);
  digitalWrite(green, LOW); // Start with LED off
  
  // Start the PC connection (for debugging)
  Serial.begin(9600);
  Serial.println("Arduino is ready.");

  // Start the Bluetooth connection
  Serial1.begin(9600); // 9600 is the default speed for HC-05
  Serial.println("Bluetooth module is waiting for connection...");
}

void loop() {

  // Check if data is coming FROM the Bluetooth module
  if (Serial1.available() > 0) {
    
    // Read the incoming byte (the character)
    command = Serial1.read();
    
    // Print to PC monitor (so we can see what the phone sent)
    Serial.print("Command received: ");
    Serial.println(command);

    // Check the command
    if (command == 'a') {
      digitalWrite(green, HIGH);
      Serial.println("LED ON");
    }
    
    if (command == 'b') {
      digitalWrite(green, LOW);
      Serial.println("LED OFF");
    }
  }
}