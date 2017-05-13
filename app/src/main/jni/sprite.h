//
// Created by Alexander Strong on 5/11/17.
//

#ifndef HELLOARMULTITARGET_SINGLETRACKER_NATIVE_SPRITE_H
#define HELLOARMULTITARGET_SINGLETRACKER_NATIVE_SPRITE_H

#include <cstdlib>
#include <climits>
#include <string>

#define TOTAL_SPEED 0.0001
#define BOARD_LENGTH 2
#define BOARD_WIDTH 2

struct sprite {

  enum SpriteState { DEAD, ALIVE, HIDDEN, SMASHED };
  SpriteState state;

  float x, y, dir, speed;
  std::string imgPath;
  int time_out;

  sprite(float xIn, float yIn) : x(xIn), y(yIn), time_out(0) {
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
  /*
    float xAmt = speed*dir,
          yAmt = dir*(TOTAL_SPEED-speed);
    if(x+xAmt <= -BOARD_WIDTH/2 || x+xAmt >= BOARD_WIDTH/2) {
      if(y+yAmt <= -BOARD_LENGTH/2 || y+yAmt >= BOARD_LENGTH/2) {
        //change both the x and y directions
        dir = -dir;
      }
      else {
        //change only the x direction
        dir = -dir;
        speed = 2*TOTAL_SPEED - speed;
      }
    }
    else if(y+yAmt <= -BOARD_LENGTH/2 || y+yAmt >= BOARD_LENGTH/2) {
      //change only the y direction
      speed = 2*TOTAL_SPEED-speed;
    }
    */
    //LOGI("Move function called");
    move(0.1, 0.1);
  }

  void die() {
    state = DEAD;
  }

  void respawn() {
    state = ALIVE;
  }
};


#endif //HELLOARMULTITARGET_SINGLETRACKER_NATIVE_SPRITE_H
