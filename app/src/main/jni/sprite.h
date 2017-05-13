//
// Created by Alexander Strong on 5/11/17.
//

#ifndef HELLOARMULTITARGET_SINGLETRACKER_NATIVE_SPRITE_H
#define HELLOARMULTITARGET_SINGLETRACKER_NATIVE_SPRITE_H

#include <cstdlib>
#include <climits>
#include <string>

#define TOTAL_SPEED 0.0025
#define BOARD_LENGTH 6
#define BOARD_WIDTH 6

struct sprite {

  enum SpriteState { DEAD, ALIVE, HIDDEN, SMASHED };
  SpriteState state;

  float x, y, dir, speed;
  std::string imgPath;
  sprite(float xIn, float yIn) : x(xIn), y(yIn) {
    dir = 2*(float)rand()/RAND_MAX-1;
    speed = (float)rand()/RAND_MAX * TOTAL_SPEED;
    state = ALIVE;
    imgPath="ant.png";
  }

  sprite() : sprite((int)rand()%4-2, (int)rand()%4-2) {
    //do nothing
  }
  void moveX(float amt) { x += amt; }
  void moveY(float amt) { y += amt; }
  void move(float xAmt, float yAmt) {
    moveX(xAmt);
    moveY(yAmt);
  }

  void move() {
    float xAmt = speed*dir,
          yAmt = speed*(TOTAL_SPEED-dir);
    move(xAmt, yAmt);
  }

  void die() {
    state = DEAD;
  }

  void respawn() {
    state = ALIVE;
  }
};


#endif //HELLOARMULTITARGET_SINGLETRACKER_NATIVE_SPRITE_H
