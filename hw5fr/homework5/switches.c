/*
 * switches.c
 *
 *  Created on: Sep 23, 2024
 *      Author: happy
 */
//-----------------------------------------------------------------------------
#define PRESSED (1)
#define RELEASED (0)

#define CIRCLE (0)
#define FIGUREEIGHT (1)
#define TRIANGLE (2)
#define PFIVE (3)

#include  "msp430.h"
#include  "Include\ports.h"
#include  "Include\functions.h"

unsigned char sw1_position = RELEASED;
unsigned char sw2_position  = RELEASED;
unsigned int event =  CIRCLE;
unsigned int START_RUNNING = RELEASED;

extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern char display_line[4][11];
extern unsigned int smclk_select;



void Switches_Process(void){
 //-----------------------------------------------------------------------------
// This function calls the individual Switch Functions
 //-----------------------------------------------------------------------------
    Switch1_Process();
    Switch2_Process();
 }


void Switch1_Select(void){
    strcpy(display_line[2], "   GPIO   ");
    display_changed = TRUE;
    smclk_select = 0;
    Init_Port3();


}




void Switch2_Select(void){
    strcpy(display_line[2], "  SMCLK   ");
    display_changed = TRUE;
    smclk_select = 1;
    Init_Port3();


}

 //----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void Switch1_Process(void){
 //-----------------------------------------------------------------------------
// Switch 1 Configurations
 //-----------------------------------------------------------------------------

    if(sw1_position != PRESSED){
        if (!(P4IN & SW1)){
        sw1_position = PRESSED;
        Switch1_Select();
        }
    }

    if(sw1_position == PRESSED){
        if(P4IN & SW1){
            sw1_position = RELEASED;
        }
    }
 }
 //-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void Switch2_Process(void){
 //-----------------------------------------------------------------------------
// Switch 2 Configurations
 //-----------------------------------------------------------------------------
    if(sw2_position != PRESSED){
        if (!(P2IN & SW2)){
            sw2_position = PRESSED;
            Switch2_Select();
        }
        }

        if(sw2_position == PRESSED){
            if(P2IN & SW2){
                sw2_position = RELEASED;
                strcpy(display_line[3], "          ");
                display_changed = TRUE;
            }
        }
 }
 //-----------------------------------------------------------------------------
