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

unsigned  int button1state = 0;
unsigned  int blackVal = 0;
unsigned  int whiteVal = 0;

extern unsigned volatile int ADC_Left_Detect; //left light detector
extern unsigned volatile int ADC_Right_Detect; //right light detector

extern char hexToBCD[10];


#include  "msp430.h"
#include  <string.h>
#include  "Include\functions.h"
#include  "Include\LCD.h"
#include  "Include\ports.h"
#include  "Include\macros.h"
#include "Include\header.h"

void HEXtoBCD(char* str,unsigned volatile int hexVal,unsigned volatile int pos){

    int strval = 0;
    int hexB = 0;
    int mask = 15; //x00F bitmask
    int check = 0;
    str[11] = 0x0;
    hexB = hexVal;
    check = mask & hexB; //mask the hex value with 0x00F.
    if(check < 10){
        strval = check + ASCII_OFFSET;
    } else if(check >= 10){
        strval = check - 10;
        strval = strval + ASCII_OFFSET2;
    }
    str[pos+2] = strval;

    hexB = hexB >> 4;
    check = mask & hexB;
    if(check < 10){
            strval = check + ASCII_OFFSET;
        } else if(check >= 10){
            strval = check - 10;
            strval = strval + ASCII_OFFSET2;
        }
    str[pos+1] = strval;


    hexB = hexB >> 4;
        check = mask & hexB;
        if(check < 10){
                strval = check + ASCII_OFFSET;
            } else if(check >= 10){
                strval = check - 10;
                strval = strval + ASCII_OFFSET2;
            }

        str[pos] = strval;

}

int loadColor(unsigned volatile int color){
    if(color == BLACK){
        return ((ADC_Right_Detect + ADC_Left_Detect) / 2 ) - 100;
    } else {
        return ((ADC_Right_Detect + ADC_Left_Detect) / 2) + 100;
    }
}


