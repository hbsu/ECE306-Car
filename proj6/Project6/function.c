/*
 * function.c
 *
 * My functions, I have a lot of functions and I need somewhere
 * to put them
 *
 *  Created on: Oct 9, 2024
 *      Author: Henry
 */

#define ASCII_OFFSET (0x030)
#define ASCII_OFFSET2 (65)


extern char hexToBCD[10];


#include  "msp430.h"
#include  <string.h>
#include  "Include\functions.h"
#include  "Include\LCD.h"
#include  "Include\ports.h"
#include  "Include\macros.h"

void HEXtoBCD(unsigned volatile int hexVal){

    int strval = 0;
    int hexB = 0;
    int mask = 15;
    int check = 0;
    hexB = hexVal;
    check = mask & hexB; //mask the hex value with 0x00F.
    if(check < 10){
        strval = check + ASCII_OFFSET;
    } else if(check >= 10){
        strval = check - 10;
        strval = strval + ASCII_OFFSET2;
    }
    hexToBCD[5] = strval;

    hexB = hexB >> 4;
    check = mask & hexB;
    if(check < 10){
            strval = check + ASCII_OFFSET;
        } else if(check >= 10){
            strval = check - 10;
            strval = strval + ASCII_OFFSET2;
        }
    hexToBCD[4] = strval;


    hexB = hexB >> 4;
        check = mask & hexB;
        if(check < 10){
                strval = check + ASCII_OFFSET;
            } else if(check >= 10){
                strval = check - 10;
                strval = strval + ASCII_OFFSET2;
            }

        hexToBCD[3] = strval;

}




