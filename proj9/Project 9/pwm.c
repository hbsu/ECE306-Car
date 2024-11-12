/*
 * pwm.c
 *
 *  Created on: Sep 23, 2024
 *      Author: Henry
 */
//-----------------------------------------------------------------------------

//------------------------------------------------------------------------------

unsigned int per; //percentage for wheel speed

extern char display_line[4][11]; //

extern unsigned int proj9;
extern unsigned int execute_timer;
extern unsigned int start_execute;

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

void run_forward(void){
    if(execute_timer < 400){
        RIGHT_REVERSE_SPEED = wheel_speed(L,0);
        LEFT_REVERSE_SPEED = wheel_speed(R,0);
        RIGHT_FORWARD_SPEED = wheel_speed(L,25); //left wheel
        LEFT_FORWARD_SPEED = wheel_speed(R,25); //right wheel

        strcpy(display_line[1], " forward  ");
        display_changed = TRUE;
    } else {
        proj9 = 2;
        start_execute = 0;
        execute_timer = 0;
        popqueue();

    }
}

void run_backward(void){
    if(execute_timer < 200){
        RIGHT_REVERSE_SPEED = wheel_speed(L,25);
        LEFT_REVERSE_SPEED = wheel_speed(R, 25);
        RIGHT_FORWARD_SPEED = wheel_speed(L,0);
        LEFT_FORWARD_SPEED = wheel_speed(R,0);

        strcpy(display_line[1], "  back   ");
        display_changed = TRUE;
    } else {
        proj9 = 2;
        start_execute = 0;
        execute_timer = 0;
        popqueue();

    }
}
void run_45(void){
    if(execute_timer < 75){
            RIGHT_REVERSE_SPEED = wheel_speed(L,0);
            LEFT_REVERSE_SPEED = wheel_speed(R, 25);
            RIGHT_FORWARD_SPEED = wheel_speed(L,25);
            LEFT_FORWARD_SPEED = wheel_speed(R,0);
            strcpy(display_line[1], "   45    ");
            display_changed = TRUE;

        } else {
            proj9 = 2;
            start_execute = 0;
            execute_timer = 0;
            popqueue();
        }
}

void run_90(void){
    if(execute_timer < 120){
        RIGHT_REVERSE_SPEED = wheel_speed(L,0);
        LEFT_REVERSE_SPEED = wheel_speed(R, 25);
        RIGHT_FORWARD_SPEED = wheel_speed(L,25);
        LEFT_FORWARD_SPEED = wheel_speed(R,0);
        strcpy(display_line[1], "   90    ");
        display_changed = TRUE;
    } else {
        proj9 = 2;
        start_execute = 0;
        execute_timer = 0;
        popqueue();
    }
}

void run_stop(void){
    RIGHT_REVERSE_SPEED = wheel_speed(L,0);
    LEFT_REVERSE_SPEED = wheel_speed(R, 0);
    RIGHT_FORWARD_SPEED = wheel_speed(L,0);
    LEFT_FORWARD_SPEED = wheel_speed(R,0);
}
