
// I use these two because it was easier to visualize what the LED matrix would look like. We could also have used snake_pos to directly set
// the LEDs.
bool snake[6][6]; // Where the snake is on the LED matrix 
bool food[6][6];  // where the food is on the LED matrix


void setBrightness(byte brightness) {
  analogWrite(outputEnablePin, 255-brightness);
}

void resetLed(bool led[6][6]) {
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 6; j++) {
      led[i][j] = 0;
    }
  }
}

bool snakeCollision(byte u, byte v) {
  return snake[u][v];
}

bool foodCollision(byte u, byte v) {
  return food[u][v];
}

void setFood(byte u, byte v) {
  resetLed(food);
  food[u][v] = true;
}

// sets all of the snake except the head
// we want to do it this way so we can check if the head will hit any part of the moved snake
void setSnake(byte pos[36][2]) {
  resetLed(snake);
 
  for (int i = 1; i < 36; i++) {
    // 255 indicates that the array is at its initial value. All values after have not been changed, no need to contiunue.
    if (pos[i][0] == 255) {
      break;
    } else {
      snake[pos[i][0]][pos[i][1]] = true;
    }
  }
}

void setSnakeHead(byte u, byte v) {
  snake[u][v] = true;
}

void display(bool blink_food) {

  for (int j = 0; j<6; j++) {
    for (int i = 0; i<6; i++) {
      byte reg1 = B0; 
      if (snake[j][i]) {
        bitSet(reg1, 5 - j); // sets the bit if the LED in the specific row should be on
      }

      if (blink_food) {
        if (food[j][i]) {
          bitSet(reg1, 5 - j);
        }
      }

      // to turn off the LED, give high to both shift registers
      byte reg2 = B11111111; 
      bitClear(reg2, i); // cycles connecting the column to ground
      
      // write to the shift registers
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, reg2);
      shiftOut(dataPin, clockPin, MSBFIRST, reg1);
      digitalWrite(latchPin, HIGH);
    }
  }
}