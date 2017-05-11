//
// Created by Alexander Strong on 5/11/17.
//

#ifndef HELLOARMULTITARGET_SINGLETRACKER_NATIVE_SPRITE_H
#define HELLOARMULTITARGET_SINGLETRACKER_NATIVE_SPRITE_H


struct sprite {
    float x, y;
    sprite(float xIn, float yIn) : x(xIn), y(yIn) {
        //do nothing
    }
    void moveX(float amt) { x += amt; }
    void moveY(float amt) { y += amt; }
    void move(float xAmt, float yAmt, float zAmt) {
        moveX(xAmt);
        moveY(yAmt);
    }
};


#endif //HELLOARMULTITARGET_SINGLETRACKER_NATIVE_SPRITE_H
