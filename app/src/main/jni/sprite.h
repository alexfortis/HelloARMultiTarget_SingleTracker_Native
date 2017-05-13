//
// Created by Alexander Strong on 5/11/17.
//

#ifndef HELLOARMULTITARGET_SINGLETRACKER_NATIVE_SPRITE_H
#define HELLOARMULTITARGET_SINGLETRACKER_NATIVE_SPRITE_H

#include <cstdlib>
#include <climits>
#include <string>

#define TOTAL_SPEED 0.5
#define BOARD_LENGTH 54
#define BOARD_WIDTH 54
#define MIN_X -BOARD_WIDTH
#define MAX_X 0
#define MIN_Y -BOARD_LENGTH
#define MAX_Y 0
#define NEW_VELOCITY (float)rand()/RAND_MAX * TOTAL_SPEED


struct sprite {

  enum SpriteState { DEAD, ALIVE, HIDDEN, SMASHED };
  SpriteState state;

  float x, y, xVel, yVel;
  std::string imgPath;
  int time_out;

  sprite(float xIn, float yIn) : x(xIn), y(yIn), time_out(0) {
    /* dir = 2*(float)rand()/RAND_MAX-1; */
    //speed = (float)rand()/RAND_MAX * TOTAL_SPEED * 2;
    xVel = NEW_VELOCITY;
    yVel = NEW_VELOCITY;
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
    if(x+xVel <= MIN_X || x+xVel >= MAX_X) {
      if(xVel < 0) {
        xVel = NEW_VELOCITY;
      }
      else {
        xVel = -NEW_VELOCITY;
      }
    }
    if(y+yVel <= MIN_Y || y+yVel >= MAX_Y) {
      if(yVel < 0) {
        yVel = NEW_VELOCITY;
      }
      else {
        yVel = -NEW_VELOCITY;
      }
    }
    move(xVel, yVel);
  }

  void die() {
    state = DEAD;
    imgPath = "blood-stain.png";
  }

  void respawn() {
    state = ALIVE;
    imgPath = "ant.png";
  }

};


#endif //HELLOARMULTITARGET_SINGLETRACKER_NATIVE_SPRITE_H
