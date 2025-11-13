/*
 * Teste Simples - Sensor IR de Obstáculo
 * Pino 7 -> OUT do Sensor
 * Abra o Serial Monitor em 9600
 */

const int irPin = 7; // Seu pino de sinal

void setup() {
  Serial.begin(9600);
  pinMode(irPin, INPUT);
  Serial.println("--- Teste do Sensor IR Iniciado ---");
}

void loop() {
  int leitura = digitalRead(irPin);

  // A maioria desses sensores é "Active LOW":
  // 0 (LOW) = Detectou Obstáculo
  // 1 (HIGH) = Caminho Livre
  
  if (leitura == LOW) {
    Serial.println("ALERTA: Objeto Detectado! [0]");
  } else {
    Serial.println("... Livre ... [1]");
  }

  delay(200); // Um pouco de delay para não poluir a tela
}