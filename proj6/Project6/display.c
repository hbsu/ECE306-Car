/*
 * display.c
 *
 *  Created on: Sep 18, 2024
 *      Author: happy
 */

#include  "msp430.h"
#include  <string.h>
#include  "Include\functions.h"
#include  "Include\LCD.h"
#include  "Include\ports.h"
#include  "Include\macros.h"

extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern char display_line[4][11];
extern char hexToBCD[10];


void Display_Process(void){
  if(update_display){
    update_display = 0;
    if(display_changed){
      display_changed = 0;
      Display_Update(0,0,0,0);
    }
  }
}


void adc_line4(int linenum){
    strcpy(display_line[linenum], hexToBCD);
    display_changed = TRUE;


}


