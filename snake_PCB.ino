const int latchPin = 5;		      // Latch pin of 74HC595 is connected to Digital pin 5
const int clockPin = 6;		      // Clock pin of 74HC595 is connected to Digital pin 6
const int dataPin = 4;		      // Data pin of 74HC595 is connected to Digital pin 4
const int outputEnablePin = 3;	// OE pin of 74HC595 is connected to PWM pin 3

const int sw1Pin = 8;   // right button
const int sw2Pin = 9;   // left button

byte speed; // determines the speed of the game, lower is faster


void setup() {
  //Serial.begin(9600);

  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
  pinMode(outputEnablePin, OUTPUT); 

  pinMode(sw1Pin, INPUT);
  pinMode(sw2Pin, INPUT);

  setBrightness(255);

  setupGame();
}

void loop() {
  static byte speed_count = 0; // keeps track of when to progress the game

  static byte blink = 0; // only used to calclulate when the food LED should be on or off

  static bool right = false;
  static bool left = false;

  static bool buttonState1 = LOW;
  static bool buttonState2 = LOW;

  // This bit allows us to detect the realease of a button, which makes it easer to control 
  if (buttonState1 == HIGH && digitalRead(sw1Pin) == LOW) {
    right = true;
    left = false;
  }
  if (buttonState2 == HIGH && digitalRead(sw2Pin) == LOW) {
    left = true;
    right = false;
  }
  buttonState1 = digitalRead(sw1Pin);
  buttonState2 = digitalRead(sw2Pin);

  if (speed_count >= speed) {
    bool eaten = step(right, left);

    if (eaten) {
      speed = speed * 0.92;
    }

    right = false;
    left = false;
    speed_count = 0;
  }

  // blinks the food
  if (blink % 32 < 16) {
    display(false);
  } else {
    display(true);
  }

  blink++; // will overflow but that is no problem 
  speed_count++;
}



















