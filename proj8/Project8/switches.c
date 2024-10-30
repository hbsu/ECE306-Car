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
#include  <string.h>
#include  "Include\functions.h"
#include  "Include\LCD.h"
#include  "Include\ports.h"
#include  "Include\macros.h"
#include "Include\header.h"

unsigned char sw1_position = RELEASED;
unsigned char sw1_debounce = RELEASED;
unsigned char sw2_position  = RELEASED;
unsigned char sw2_debounce = RELEASED;
unsigned int START_RUNNING = RELEASED;
extern char hexToBCD[11];
extern char labelArr[11];


extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern char display_line[4][11];
extern  unsigned int countersw1;
extern  unsigned int countersw2;
extern unsigned  int gotime;
extern unsigned int p8part;
extern char USB_Char_Rx[11];

extern unsigned int usb_rx_ring_wr;
extern unsigned int usb_tx_ring_wr;


extern unsigned  int button1state;
extern unsigned  int blackVal;
extern unsigned  int whiteVal;

volatile char ncsuArray[11] = {'N', 'C', 'S' ,'U', ' ', ' ', '#' , '1', ' ' , ' ', 0x00};
volatile char lowbaudr[11] = {' ',' ','4','6','0','8','0','0',' ',' ',0x00}; //460800
volatile char highbaudr[11] = {' ',' ','1','1','5','2','0','0',' ',' ',0x00}; //115200
unsigned int baudmode = 0;
unsigned int charcount = 0;
//        strcpy(display_line[3], "          ");



void Switches_Process(void){
 //-----------------------------------------------------------------------------
// This function calls the individual Switch Functions
 //-----------------------------------------------------------------------------
}


void Switch1_Select(void){
    usb_rx_ring_wr = 0; //reset ring buffer
    usb_tx_ring_wr = 0; //reset ring buffer
    USB_Char_Rx[0] = ' ';
    USB_Char_Rx[1] = ' ';
    USB_Char_Rx[2] = ' ';
    USB_Char_Rx[3] = ' ';
    USB_Char_Rx[4] = ' ';
    USB_Char_Rx[5] = ' ';
    USB_Char_Rx[6] = ' ';
    USB_Char_Rx[7] = ' ';
    USB_Char_Rx[8] = ' ';
    USB_Char_Rx[9] = ' ';
    USB_Char_Rx[10] = 0x00;
    p8part = 0;
    gotime = 1;
    baudmode = 1;
}


void Switch2_Select(void){
    usb_rx_ring_wr = 0; //reset ring buffer
    usb_tx_ring_wr = 0; //reset ring buffer
    USB_Char_Rx[0] = ' ';
    USB_Char_Rx[1] = ' ';
    USB_Char_Rx[2] = ' ';
    USB_Char_Rx[3] = ' ';
    USB_Char_Rx[4] = ' ';
    USB_Char_Rx[5] = ' ';
    USB_Char_Rx[6] = ' ';
    USB_Char_Rx[7] = ' ';
    USB_Char_Rx[8] = ' ';
    USB_Char_Rx[9] = ' ';
    USB_Char_Rx[10] = 0x00;
    p8part = 0;
    gotime = 1;
    baudmode = 2;
}


/**
//Case Switch statement
void Switch1_Select(void){
    switch (button1state){
    case 0: //announce scan for black
        strcpy(display_line[2], "scan4black");
        button1state += 1;
        break;
    case 1: //scan for black
        blackVal = loadColor(BLACK); //print the value onto screen
        labelArr[0] = 'B';
        labelArr[1] = ':';
        HEXtoBCD(labelArr ,blackVal,2);
        adc_line(labelArr ,0);
        strcpy(display_line[2], " scanned  ");
        button1state += 1;
        break;
    case 2: //scan black
        strcpy(display_line[2], "scan4white");
        button1state += 1;
        break;
    case 3: //announce we scanning white
        whiteVal = loadColor(WHITE);
        labelArr[5] = 'W';
        labelArr[6] = ':';
        HEXtoBCD(labelArr ,whiteVal,7);
        adc_line(labelArr ,0);
        strcpy(display_line[2], " scanned  ");
        button1state += 1;
        break;
    case 4: //scan white
        strcpy(display_line[2], "          ");
        button1state = 0;

        break;
    }
}
**/


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
        countersw1 = 0;
        TB0CCR1 = TB0R + ONESEC; //add interval to TB0R for TB0CCR1//tb0r is current time value
        TB0CCTL1 |= CCIE;//sets this interval to be available for reading
        //ONESEC is actualy 1/10 of a secomd
        P6OUT &= ~LCD_BACKLITE;
        Switch1_Select();
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
        TB0CCTL2 |= CCIE;//ONESEC is 1/10 of a second
        P6OUT &= ~LCD_BACKLITE;
        Switch2_Select();
        display_changed = TRUE;
    } else {
        sw2_position = RELEASED;
    }
 }
 //-----------------------------------------------------------------------------
