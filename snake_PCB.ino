const int latchPin = 5;		// Latch pin of 74HC595 is connected to Digital pin 5
const int clockPin = 6;		// Clock pin of 74HC595 is connected to Digital pin 6
const int dataPin = 4;		// Data pin of 74HC595 is connected to Digital pin 4
const int outputEnablePin = 3;	// OE pin of 74HC595 is connected to PWM pin 3

const int lSwPin = 8;
const int rSwPin = 9;

byte shiftReg1 = 0;
byte shiftReg2 = 0;

String input;
byte num;

byte head; // keeps track of where the head of the snake is. It is equal to the an element in leds[]
// The index values of the snake in leds[], head is to the right, tail is to the left
byte snake[36];
byte len; //length of the snake
String direction;
byte move;
byte food;
bool foodHasPin = false;

int resetMain; // the reset value for mainIter
int mainIter = 0; // keeps track of main loop, used so vary the speed of the snake.
                  // maybe can be changed to other type to save memory.

// The states that all the LEDs should have, the first six is for the first row, the next six is for the second, and so on.
bool leds[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void setup() {
  //Serial.begin(9600);

  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
  pinMode(outputEnablePin, OUTPUT); 

  pinMode(lSwPin, INPUT);
  pinMode(rSwPin, INPUT);

  setBrightness(40);

  gameStart();
}

void loop() {
  mainIter++;

  //read the input
  if(digitalRead(lSwPin) == HIGH) {
    direction = "left";
    //Serial.println("left");
  }
  if(digitalRead(rSwPin) == HIGH) {
    direction = "right";
    //Serial.println("right");
  }
  
  if(mainIter == resetMain){
    moveSnake();
    mainIter = 0;
  } 
  if(mainIter == 0 || mainIter == resetMain/2){
    blinkFood();
  }
  updateLED();  
} 

void setBrightness(byte brightness) {
  analogWrite(outputEnablePin, 255-brightness);
}

void updateShiftReg() {
   digitalWrite(latchPin, LOW);
   shiftOut(dataPin, clockPin, MSBFIRST, shiftReg2);
   shiftOut(dataPin, clockPin, MSBFIRST, shiftReg1);
   digitalWrite(latchPin, HIGH);
}

void updateLED(){
  int k = 5; // k is used to keep track of the last loop. Since i = 0 is the start, the last loop would have been i = 5

  for(int i = 0; i <= 5; i++){
    // turning off the last row at the same time as turning on this row makes sure the LEDs are on for as long as possible.
    // The ground pins are connected to 0, 1, 2, 3, 4, 5 in the second shift register.    
  
    bitSet(shiftReg2, k);
    
    for(int j = 0; j <= 5; j++){
      if(leds[j + (6*i)]){
        bitSet(shiftReg1, j);        
      } else {
        bitClear(shiftReg1, j);
      }
    }

    // The ground pins are connected to 0, 1, 2, 3, 4, 5 in the second shift register.
    bitClear(shiftReg2, i);

    updateShiftReg();
    k = i;
  }
}

void moveSnake() {

  //if direction == "forward" no button has been pressen, do nothing and leave move as the last value.
  if(direction == "right"){
    //Serial.println("direction is right");
    if(move < 4){
      move += 1;
    } else {
      move = 1;
    }
  } 
  if(direction == "left"){
    //Serial.println("direction is left");
    if(move > 1){
      move -= 1;
    } else {
      move = 4;
    }
  } 

  byte tmp[35];
  for(int i = 0; i < len; i++) {
    tmp[i] = snake[i];
  }  
  
  head = snake[len - 1];  

  // up
  if(move == 1) {
    if(head == 0 || head == 6 || head == 12 || head == 18 || head == 24 || head == 30) {
      snake[len - 1] = head + 5;
    } else {
      snake[len - 1] = head - 1;
    }
  }

  //right
  if(move == 2) {
    if(head == 30 || head == 31 || head == 32 || head == 33 || head == 34 || head == 35) {
      snake[len - 1] = head - 30;
    } else {
      snake[len - 1] = head + 6;
    }
  }

  //down  
  if(move == 3) {
    if(head == 5 || head == 11 || head == 17 || head == 23 || head == 29 || head == 35) {
      snake[len - 1] = head - 5;    
    } else {
      snake[len - 1] = head + 1;
    }
  }
  
  //left
  if(move == 4) {
    if(head == 0 || head == 1 || head == 2 || head == 3 || head == 4 || head == 5) {
      snake[len - 1] = head + 30;
    } else {
      snake[len - 1] = head - 6;
    }
  }

  if(!eaten()) {
    for(int i = 0; i < len - 1; i++) {
      snake[i] = tmp[i + 1];
    } 
  } else {
    snake[len] = snake[len - 1];
    for(int i = 0; i < len; i++) {
      snake[i] = tmp[i];
    }
    len++;
    resetMain = resetMain - 6;
    spawnFood();
  }  

  detectHit();  
  resetLED();
  snakeToLED();

  direction = "forward";  
}

void resetLED() {
  for(int i = 0; i <= 35; i++) {
    leds[i] = 0;
  }
}

void snakeToLED() {
  for(int i = 0; i < len; i++) {
    leds[snake[i]] = 1;    
  }
}

void blinkFood() {
  if(!leds[food]) {
    leds[food] = 1;
  } else {
    leds[food] = 0;
  }
}

void detectHit() {
  for(int i = 0; i < len - 1; i++) {
    if(snake[i] == snake[len - 1]) {
      gameStart();
    }
  }
}

bool eaten() {
  if(snake[len - 1] == food) { 
    leds[food] = 0;
    foodHasPin = false;       
    return true;
  } else {
    return false;
  }
}

// this one runs forever now, probably because it is calling itself
void spawnFood() {
  while(!foodHasPin) {
    food = random(0, 35);
    for(int i = 0; i <= 35; i++) {
      if(!leds[food]) {
        foodHasPin = true;
      }
    }
  }
}

void gameStart() {
  // Start the snake in the top left corner.
  resetMain = 300;
  len = 2;
  snake[0] = 12;
  snake[1] = 18;
  snakeToLED();
  spawnFood();
  blinkFood(); 
  move = 2;
}