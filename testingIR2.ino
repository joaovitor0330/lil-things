/*
 * Projeto 09 - Contador de Objetos (IR + OLED)
 */

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// --- Hardware Pins ---
const int irPin = 7; // O pino do sensor IR

// --- Display Config ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// --- Variables ---
int counter = 0;          // O nosso contador
int irState = 0;          // Estado ATUAL do sensor
int lastIrState = 0;      // Estado ANTERIOR (Memória)

void setup() {
  Serial.begin(9600);
  
  pinMode(irPin, INPUT);
  
  // Inicializa OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    while (true); // Trava se falhar
  }
  
  // Tela de Boas Vindas
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10, 10);
  display.print("CONTADOR");
  display.setCursor(10, 30);
  display.print("ATIVO");
  display.display();
  delay(2000);
}

void loop() {
  // 1. Ler o sensor
  // (A maioria dos sensores IR dá LOW quando detecta algo)
  irState = digitalRead(irPin);

  // 2. Lógica de Detecção de Mudança (Borda)
  // Só fazemos algo se o estado MUDOU em relação à memória
  if (irState != lastIrState) {
    
    // Se o estado mudou para LOW (Detectou!)
    if (irState == LOW) { 
      counter++; // Conta +1
      Serial.println("Objeto Detectado!");
    }
    
    // Delay pequeno para evitar contar ruído (debounce)
    delay(50);
  }
  
  // 3. Atualiza a Memória para a próxima volta
  lastIrState = irState;

  // 4. Desenha o Placar no OLED
  display.clearDisplay();
  
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("Contagem:");
  
  display.setTextSize(4); // Fonte Gigante para o número
  display.setCursor(40, 25);
  display.print(counter);
  
  display.display();
}