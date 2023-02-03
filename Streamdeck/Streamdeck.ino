#define CS 10
#define MISO 11
#define MOSI 12
#define SCK 13
#define MAX_LEDS 16
byte lights[3][16];  // [3] = Red, Blue, Green //
byte red[MAX_LEDS];
byte blue[MAX_LEDS];
byte green[MAX_LEDS];

unsigned int buttons = 0;

void setup() {
  Serial.begin(9600);
  pinMode(CS, OUTPUT);
  pinMode(MISO, OUTPUT);
  pinMode(MOSI, INPUT);
  pinMode(SCK, OUTPUT);
  digitalWrite(CS, LOW);
  delay(1);
  memset(lights, 0, sizeof(lights)); //fill the lights array with zeros
  memset(red,0, sizeof(red));
  memset(blue,0, sizeof(blue));
  memset(green,0, sizeof(green));
}
void loop() {
  //Serial.println("helk");
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
      //we are iterating through each of the bits to set the which lights need to turn on and at what intensity
      for (int ii = 0; ii < 8; ii++) {
        //clock low and hold
        digitalWrite(SCK, LOW);
        delayMicroseconds(5);
        //setting the data on MISO to clock into the button pad
        //if we are in the colour frames set the MISO to clock out data
        if (f < 3) digitalWrite(MISO, bitRead(lights[f][i], ii));
        //this next statement is done on all the frames but for the first three frames the data doesn't really matter
        //according to the documentation the last frame is the button frame and that is where we read in the button data
        //while we clock in data through MISO, we read the bytes out from MOSI
        //there are only two possible values 0x00 and 0xFF
        //0x00 - represents a pressed button
        //0xFF - represents a non pressed button
        //since all the values in the byte are the same in the byte when pressed or not, we just need to read
        //the first bit to represent the state of the button.
        //This is stored in the buttons variable which is a 16 bit unsigned int.
        //each of the bits represents one of the buttons, so when we check it with the bitRead() we can just isolate the one 
        //button to light up whatever needs to be lit

        else if (ii == 0) {
          bitWrite(buttons, i, digitalRead(MOSI));
        }
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