/*
 * pwm.c
 *
 *  Created on: Sep 23, 2024
 *      Author: Henry
 */
//-----------------------------------------------------------------------------

//------------------------------------------------------------------------------

unsigned int per; //percentage for wheel speed


#include  "msp430.h"
#include  <string.h>
#include  "Include\functions.h"
#include  "Include\LCD.h"
#include  "Include\ports.h"
#include  "Include\macros.h"
#include "Include\header.h"



//THESE ARE THE VALUES YOU NEED FOR IT TO GO STRAIGHT
//RIGHT_FORWARD_SPEED = wheel_speed(57); //this is the left wheel
//LEFT_FORWARD_SPEED = wheel_speed(80); //this is actually right wheel

unsigned int wheel_speed(unsigned int side,unsigned int percentage){
    if(side == L){ //adjusts the left wheel so it runs a little slower to account for the right wheel speed.
        per = WHEEL_PERIOD * percentage / 100 * FIFTYSEVEN / EIGHTY;
        return per;

    } else if(side == R){
        per = WHEEL_PERIOD * percentage / 100;
        return per;
    }

}
