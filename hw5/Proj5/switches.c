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



void Switches_Process(void){
 //-----------------------------------------------------------------------------
// This function calls the individual Switch Functions
 //-----------------------------------------------------------------------------
    Switch1_Process();
    Switch2_Process();
 }


void Switch1_Select(void){
    switch(event){
    case CIRCLE:
        event = FIGUREEIGHT;
        strcpy(display_line[1], " fig8     ");
        break;
    case FIGUREEIGHT:
        event = TRIANGLE;
        strcpy(display_line[1], " tria     ");
        break;
    case TRIANGLE:
        event = PFIVE;
        strcpy(display_line[1], " PFIVE     ");
        break;
    case PFIVE:
        event = CIRCLE;
        strcpy(display_line[1], "   circ  ");
        break;
    default: break;
    }
    display_changed = TRUE;

}




void Switch2_Select(void){
    START_RUNNING = PRESSED;
    strcpy(display_line[3], "starting  ");
    display_changed = TRUE;
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
