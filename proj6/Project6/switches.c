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

#define ONESEC (50000)


#include  "msp430.h"
#include  "Include\ports.h"
#include  "Include\functions.h"

unsigned char sw1_position = RELEASED;
unsigned char sw1_debounce = RELEASED;
unsigned char sw2_position  = RELEASED;
unsigned char sw2_debounce = RELEASED;
unsigned int START_RUNNING = RELEASED;

extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern char display_line[4][11];
extern volatile unsigned int countersw1;
extern volatile unsigned int countersw2;
extern unsigned volatile int gotime;





void Switches_Process(void){
 //-----------------------------------------------------------------------------
// This function calls the individual Switch Functions
 //-----------------------------------------------------------------------------
 }


void Switch1_Select(void){
}




void Switch2_Select(void){
    START_RUNNING = ~START_RUNNING;
    gotime = 1;
}

 //----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
//Prevents unintentionally clicking multiple times, sets a grace period of
//1 second so user
//doesn't accidentally input many times and break something.
//For switch 1
//-----------------------------------------------------------------------------
#pragma vector = PORT4_VECTOR
__interrupt void switch1_interrupt(void){
 //-----------------------------------------------------------------------------
// Switch 1 Configurations
 //-----------------------------------------------------------------------------

    if(P4IFG & SW1){
        //disable switch1
        P4IFG &= ~SW1; //IFG SW1 Cleared
        sw1_position = PRESSED; //sw1 is pressed
        sw1_debounce = PRESSED; //we are debouncing
        P4IE &= ~SW1;
        //clear sw1
        //Clear timerb0 interrupt flag for ccr1;
        //200 is = 1 second
        countersw1 = 0;
        TB0CCR1 = TB0R + ONESEC; //add interval to TB0R for TB0CCR1//tb0r is current time value
        TB0CCTL1 |= CCIE;
        P6OUT &= ~LCD_BACKLITE;
        strcpy(display_line[3], "fakebutton");
        display_changed = TRUE;
    } else {
        sw1_position = RELEASED;
    }
 }
 //-----------------------------------------------------------------------------
//Prevents unintentionally clicking multiple times, sets a grace period of
//1 second so user
//doesn't accidentally input many times and break something.
//For switch 2
//-----------------------------------------------------------------------------
#pragma vector = PORT2_VECTOR
__interrupt void switch2_interrupt(void){
 //-----------------------------------------------------------------------------
// Switch 2 Configurations
 //-----------------------------------------------------------------------------

    if(P2IFG & SW2){
        //disable switch1
        P2IFG &= ~SW2; //IFG SW1 Cleared
        sw2_position = PRESSED; //sw1 is pressed
        sw2_debounce = PRESSED; //we are debouncing
        P2IE &= ~SW2;
        //clear sw1
        //Clear timerb0 interrupt flag for ccr1;
        //200 is = 1 second
        countersw2= 0;
        TB0CCR2 = TB0R + ONESEC; //add interval to TB0R for TB0CCR1
        TB0CCTL2 |= CCIE;
        P6OUT &= ~LCD_BACKLITE;
        strcpy(display_line[3], "  running ");
        Switch2_Select();
        display_changed = TRUE;
    } else {
        sw2_position = RELEASED;
    }
 }
 //-----------------------------------------------------------------------------
