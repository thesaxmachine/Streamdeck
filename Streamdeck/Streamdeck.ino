#define CS 10
#define MISO 11
#define MOSI 12
#define SCK 13
byte lights[3][16];  // [3] = Red, Blue, Green //
unsigned int buttons = 0;
void setup() {
  pinMode(CS, OUTPUT);
  pinMode(MISO, OUTPUT);
  pinMode(MOSI, INPUT);
  pinMode(SCK, OUTPUT);
  digitalWrite(CS, LOW);
  delay(1);
  memset(lights, 0, sizeof(lights));
}
void loop() {
  digitalWrite(SCK, HIGH);
  digitalWrite(CS, HIGH);
  delayMicroseconds(15);
  for (int f = 0; f < 4; f++) {
    for (int i = 0; i < 16; i++) {
      lights[f][i] = bitRead(buttons, i) ? 0 : 10;  // lights up when button pressed //
      for (int ii = 0; ii < 8; ii++) {
        digitalWrite(SCK, LOW);
        delayMicroseconds(5);
        if (f < 3) digitalWrite(MISO, bitRead(lights[f][i], ii));
        else if (ii == 0) bitWrite(buttons, i, digitalRead(MOSI));
        delayMicroseconds(5);
        digitalWrite(SCK, HIGH);
        delayMicroseconds(10);
      }
    }
  }
  digitalWrite(CS, LOW);
  delayMicroseconds(400);
}