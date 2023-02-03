#define CS 10
#define MISO 11
#define MOSI 12
#define SCK 13
byte lights[3][16];  // [3] = Red, Blue, Green //
byte pressed[3][16];

unsigned int buttons = 0;

void setup() {
  pinMode(CS, OUTPUT);
  pinMode(MISO, OUTPUT);
  pinMode(MOSI, INPUT);
  pinMode(SCK, OUTPUT);
  digitalWrite(CS, LOW);
  delay(1);
  memset(lights, 0, sizeof(lights)); //fill the lights array with zeros
  memset(pressed,0, sizeof(pressed));
}
void loop() {
  detectButton();
}

void detectButton(){
  //set clock line high
  digitalWrite(SCK, HIGH);
  //set chip select high
  digitalWrite(CS, HIGH);
  
  delayMicroseconds(15);
  //first loop goes through each of the frames
  for (int f = 0; f < 4; f++) {
    //second loop goes through each of the buttons
    for (int i = 0; i < 16; i++) {
      //sets the level of light for the frame and button
      lights[f][i] = bitRead(buttons, i) ? 0 : 10;  //ternary operator
      //lights is a byte array so each of the datapoints in f and i are 8 bits
      //we are iterating through each of the bits to set the light level(???)
      for (int ii = 0; ii < 8; ii++) {
        //clock low and hold
        digitalWrite(SCK, LOW);
        delayMicroseconds(5);
        //setting the data on MISO to clock into the button pad
        //if we are in the colour frames set the MISO to clock out data
        if (f < 3) digitalWrite(MISO, bitRead(lights[f][i], ii));
        //if we are in the button frame clock in the data from the button pad into the button variable at the position it needs to be
        else if (ii == 0) bitWrite(buttons, i, digitalRead(MOSI));
        delayMicroseconds(5);
        //clock in the data
        digitalWrite(SCK, HIGH);
        delayMicroseconds(10);
      }
    }
  }
  digitalWrite(CS, LOW);
  delayMicroseconds(400);
}