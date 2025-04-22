
byte direction; // the direction the snake is moving in; 0=right, 1=up, 2=left, 3=down

byte snake_pos[36][2];

void setupGame() {
  // Sets the initail values to 255. This gives us the ability to check if they have been changed
  for (int i = 0; i < 36; i++) {
    for (int j = 0; j < 2; j++) {
      snake_pos[i][j] = 255;
    }
  }

  snake_pos[0][0] = 5;
  snake_pos[0][1] = 1;
  snake_pos[1][0] = 5;
  snake_pos[1][1] = 0;
  
  direction = 0;

  speed = 90;

  spawnFood();

  setSnake(snake_pos);
  setSnakeHead(snake_pos[0][0], snake_pos[0][1]);
}

void spawnFood() {
  // spawn a food on a ranodme spot where there is no snake
  bool spawn_success = false;
  
  while (!spawn_success) {
    byte u = random(6);
    byte v = random(6);

    if (!snakeCollision(u, v)) {
      setFood(u, v);
      spawn_success = true;
    }  
  } 
}

void stepSnake(bool grow) {
  byte next[2];
  byte now[2];

  next[0] = snake_pos[0][0];
  next[1] = snake_pos[0][1];

  for (int i = 0; i < 36; i++) {
    now[0] = next[0];
    now[1] = next[1];
    next[0] = snake_pos[i + 1][0];
    next[1] = snake_pos[i + 1][1];

    if (grow) {
      if (now[0] == 255) {
        break;
      }
    } else if (next[0] == 255) {
        break;
    }

    snake_pos[i + 1][0] = now[0];
    snake_pos[i + 1][1] = now[1]; 
  }
}

bool step(bool right, bool left) {
  turn(right, left);

  byte head_u;
  byte head_v;

  // we want to move the head in the direction of movement, the rest of will just move one step forward
  // if the snake is at the edge, it should wrap around to the other side
  // we also need to check if the new head position is in the same as a food or as another part of the snake. If we eat the snake needs to grow
  switch (direction) {
    // right
    case 0:
      head_u = snake_pos[0][0];
      if (snake_pos[0][1] < 5) {
        head_v = snake_pos[0][1] + 1;
      } else {
        head_v = 0;
      }
      break;

    // up
    case 1:
    head_v = snake_pos[0][1];
      if (snake_pos[0][0] > 0) {
        head_u = snake_pos[0][0] - 1;
      } else {
        head_u = 5;
      }
      break;

    // left
    case 2:
      head_u = snake_pos[0][0];
      if (snake_pos[0][1] > 0) {
        head_v = snake_pos[0][1] - 1;
      } else {
        head_v = 5;
      }
      break;

    // down
    case 3:
    head_v = snake_pos[0][1];
    if (snake_pos[0][0] < 5) {
      head_u = snake_pos[0][0] + 1;
    } else {
      head_u = 0;
    }
    break;
  }

  bool eaten = foodCollision(head_u, head_v);

  stepSnake(eaten);

  setSnake(snake_pos);

  if (snakeCollision(head_u, head_v)) {
    setupGame();
    eaten = false;
  } else {
    snake_pos[0][0] = head_u;
    snake_pos[0][1] = head_v;
    setSnakeHead(head_u, head_v);
  }

  if (eaten) {
    spawnFood();
  }
  return eaten;
} 

void turn(bool right, bool left) {
  if (right) {
    if (direction == 0) {
      direction = 3;
    } else {
      direction--;
    }
  } else if(left) {
    if (direction == 3) {
      direction = 0;
    } else {
      direction++;
    }
  }
}













