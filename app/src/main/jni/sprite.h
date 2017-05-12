//
// Created by Alexander Strong on 5/11/17.
//

#ifndef HELLOARMULTITARGET_SINGLETRACKER_NATIVE_SPRITE_H
#define HELLOARMULTITARGET_SINGLETRACKER_NATIVE_SPRITE_H

#include <cstdlib>
#include <climits>
#include <string>

struct sprite {
    float x, y, dir;
    int speed;
    bool alive;
    std::string imgPath;
    sprite(float xIn, float yIn) : x(xIn), y(yIn) {
        dir = (float)rand()/RAND_MAX;
        speed = rand()%10;
        alive = true;
    }
    sprite() : sprite(0,0) {
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
            yAmt = speed*(1-dir);
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
