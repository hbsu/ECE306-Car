//goal is to update every 5 milliseconds
//00 . 01. 10. 11
#define TB0CCR0_INTERVAL (2500)

volatile unsigned int Time_Sequence;
extern unsigned int start_p5;
unsigned int p5timer = 0;


#include  "msp430.h"
#include  <string.h>
#include  "Include\functions.h"
#include  "Include\LCD.h"
#include  "Include\ports.h"
#include  "Include\macros.h"

void Init_Timers(void) {
TB0CTL = TBSSEL__SMCLK;
TB0CTL |= TBCLR;
TB0CTL |= MC__CONTINOUS;
TB0CTL |= ID__2;
TB0EX0 = TBIDEX__8;
TB0CCR0 = TB0CCR0_INTERVAL;
TB0CCTL0 |= CCIE;

//
// SMCLK source
 // Resets TB0R, clock divider, count direction
 // Continuous up
 // Divide clock by 2
 // Divide clock by an additional 8
 // CCR0
 // CCR0 enable interrupt
// TB0CCR1 = TB0CCR1_INTERVAL;  // CCR1
//
//
//
//TB0CCTL1 |= CCIE;
// CCR1 enable interrupt
// TB0CCR2 = TB0CCR2_INTERVAL;  // CCR2
//TB0CCTL2 |= CCIE;


TB0CTL &= ~TBIE;
TB0CTL &= ~TBIFG;
}
 // CCR2 enable interrupt
 // Disable Overflow Interrupt
 // Clear Overflow Interrupt flag
 //-----------------------------------------------------------------------------


#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR(void){
//-----------------------------------------------------------------------------
// TimerB0 0 Interrupt handler
//---------------------------------------------------------------------------
//...... Add What you need happen in the interrupt ......
TB0CCR0 += TB0CCR0_INTERVAL;
Time_Sequence += 1;

if(Time_Sequence % 40 == 0){
    update_display = TRUE;
    Time_Sequence = 0;
    if(start_p5 == 1){
        p5timer += 1;
    }
}





// Add Offset to TBCCR0
//---------------------------------------------------------------------------
}
