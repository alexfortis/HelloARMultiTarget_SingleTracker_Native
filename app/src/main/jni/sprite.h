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
    float x, y, dir, speed;
    bool alive;
    std::string imgPath;
    sprite(float xIn, float yIn) : x(xIn), y(yIn) {
        dir = 2*(float)rand()/RAND_MAX-1;
        speed = (float)rand()/RAND_MAX * TOTAL_SPEED*2;
        alive = true;
        imgPath="ant.png";
    }
    sprite() : sprite((int)rand()%4-2, (int)rand()%4-2) {
        //do nothing
    }
  private:
    void moveX(float amt) { x += amt; }
    void moveY(float amt) { y += amt; }
    void move(float xAmt, float yAmt) {
        moveX(xAmt);
        moveY(yAmt);
    }
  public:
    void move() {
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
        move(xAmt, yAmt);
    }
    void die() {
        alive = false;
    }
    void respawn() {
        alive = true;
    }
};


#endif //HELLOARMULTITARGET_SINGLETRACKER_NATIVE_SPRITE_H
