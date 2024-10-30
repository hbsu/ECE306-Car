//goal is to update every 5 milliseconds
//00 . 01. 10. 11

// TB3.1 P6.0 LCD_BACKLITE
 // TB3.2 P6.1 R_FORWARD
 // TB3.3 P6.2 R_REVERSE
 // TB3.4 P6.3 L_FORWARD
// TB3.5 P6.4 L_REVERSE


unsigned  int circleActive = 0;
unsigned  int circleTimer = 0;
unsigned  int adjustcounter = 0;
extern unsigned volatile int lastAction;
volatile unsigned int Time_Sequence;
unsigned int blink_count = 0;
unsigned  int countersw1 = 0;
unsigned  int countersw2 = 0;
unsigned volatile int ADC_Left_Detect = 0;
unsigned volatile int ADC_Right_Detect = 0;
unsigned volatile int ADC_TW_Detect = 0;
unsigned volatile int ADC_Channel = 0;
unsigned volatile int DAC_data = 4000;
unsigned  int turncounter = 0;
unsigned  int turncounter0 = 0;
extern unsigned  int p6part;




//project 6
extern unsigned  int p6act;
extern unsigned  int p6timer;

extern unsigned char sw1_position;
extern unsigned char sw1_debounce;
extern unsigned char sw2_position;
extern unsigned char sw2_debounce;
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern char display_line[4][11];
extern unsigned int waittime;
unsigned int gotime = 0;

char hexToBCD[11] = {0x20, 0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x00};
char labelArr[11] = {0x20, 0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x00};


//------------------------------------------------------------------------------
#include  "msp430.h"
#include  <string.h>
#include  "Include\functions.h"
#include  "Include\LCD.h"
#include  "Include\ports.h"
#include  "Include\macros.h"
#include "Include\header.h"


void Init_Timers(void){
    Init_Timer_B0();
    Init_Timer_B3();
}

void Init_Timer_B0(void) {
TB0CTL = TBSSEL__SMCLK; // SMLK Source
TB0CTL |= TBCLR; //Continuous up to 0xFFFF and overflow
TB0CTL |= MC__CONTINOUS; //Divide clock by 2
TB0CTL |= ID__2; //Divide clock by an additional 8
TB0EX0 = TBIDEX__8; //Resets Tb0R,

//Capture compare 0
TB0CCR0 = TB0CCR0_INTERVAL; //CCR0
TB0CCTL0 |= CCIE; //CCr0 Enable interrupt

// Capture compare 1
//TB0CCR1 = TB0CCR1_INTERVAL;  // CCR1
//TB0CCTL1 |= CCIE; // CCR1 enable interrupt

//Capture compare 2
//TB0CCR2 = TB0CCR2_INTERVAL;  // CCR2
//TB0CCTL2 |= CCIE; //CCR2 Enable interrupt

//Overflow
TB0CTL &= ~TBIE; //Disable overflow interrupt
TB0CTL &= ~TBIFG; //Clear overflow interrupt flag
}

 //-----------------------------------------------------------------------------


#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR(void){
//-----------------------------------------------------------------------------
// TimerB0 0 Interrupt handler
//---------------------------------------------------------------------------
//...... Add What you need happen in the interrupt ......
TB0CCR0 += TB0CCR0_INTERVAL;//Add interval offset to TACCR0.
Time_Sequence += 1;

if(gotime == 1){
    waittime+= 1;
} else {
    waittime = 0;
}

//if(p6act){
//    p6timer+=1;
//}

if(Time_Sequence % 40 == 0){
    update_display = TRUE;
    Time_Sequence = 0;
//    if(lastAction == 1){
//        turncounter +=1; //if car hasnt found line in 1 second, turn the other direction
//    }
//    if(lastAction == 0){
//        turncounter0 +=1;
//    }
//    if(p6part == 3){
//        adjustcounter += 1;
//    }
//    if(circleActive == 1){
//        circleTimer += 1;
//    }
}

if(Time_Sequence %2 == 0){
    ADCCTL0 |= ADCENC; // Enable Conversions
    ADCCTL0 |= ADCSC;
}




if(DAC_data >= DACLIMIT){
    // The following line should be done in a timer overflow interrupt [after 2 or 3 overflows]
    P2OUT |=  DAC_ENB;                  // Value = High [enabled]
    // Somewhere around 1200 will be about 6v. You will need to measure it.
    DAC_data -= 50;
    SAC3DAT = DAC_data;
}                  // Stepping DAC Output


// Add Offset to TBCCR0
//---------------------------------------------------------------------------
}

//is called whenever the interrupt is flagged
#pragma vector = TIMER0_B1_VECTOR
__interrupt void TIMER0_B1_ISR(void){
    //imerB0 1-2, overflow interrupt vector (TBIV) handler

    switch(__even_in_range(TB0IV, 14)){
    case 0 :break; //no interrupt
    case 2:
        if(countersw1 >= 10){
            //disable timer b0 ccr1,
                    //clear sw1 interrupt flag
                    //enable sw1 interrupt
                    sw1_position = 0; //sw1 is pressed//off
                    sw1_debounce = 0; //off
                    P4IFG &= ~SW1;
                    P4IE |= SW1;
                    TB0CCTL1 &= ~CCIE;
                    if(sw2_debounce == 0){
                    P6OUT |= LCD_BACKLITE;}
                    display_changed = TRUE;
        } else {
            countersw1++;
            TB0CCR1+=ONESEC;
        }

        break;

    case 4:

        if(countersw2 >= 10){ //when a total of one second passes for debounce, turn it off
            //disable timer b0 ccr1,
                    //clear sw1 interrupt flag
                    //enable sw1 interrupt
                    sw2_position = 0; //sw1 is pressed//off
                    sw2_debounce = 0; //off
                    P2IFG &= ~SW2; //Clear the flag again
                    P2IE |= SW2; //Turn on the enable
                    TB0CCTL2 &= ~CCIE; //Turn off the capture control register
                    if(sw1_debounce == 0){
                    P6OUT |= LCD_BACKLITE;}
                    display_changed = TRUE;
        } else {
            countersw2++;
            TB0CCR2+=ONESEC;
        }


        break;

    case 14: //overflow available for greater than 1 second timer
        break;
default: break;
    }
}


#pragma vector=ADC_VECTOR
__interrupt void ADC_ISR(void){
 switch(__even_in_range(ADCIV,ADCIV_ADCIFG)){
     case ADCIV_NONE:
         break;
     case ADCIV_ADCOVIFG: // When a conversion result is written to the ADCMEM0
 // before its previous conversion result was read.
         break;
     case ADCIV_ADCTOVIFG: // ADC conversion-time overflow
         break;
     case ADCIV_ADCHIIFG: // Window comparator interrupt flags
         break;
     case ADCIV_ADCLOIFG: // Window comparator interrupt flag
         break;
     case ADCIV_ADCINIFG: // Window comparator interrupt flag
         break;
     case ADCIV_ADCIFG: // ADCMEM0 memory register with the conversion result
         //Switch statement that tests if 5 or 3

         ADCCTL0 &= ~ADCENC; // Disable ENC bit.
          switch (ADC_Channel++){
          case 0x00: // Channel A2 Interrupt
              ADC_Right_Detect = ADCMEM0; // Move result into Global
              ADC_Right_Detect = ADC_Right_Detect >> 2; // Divide the result by 4
//          HEXtoBCD(hexToBCD ,ADC_Right_Detect,0); // Convert result to String
//          adc_line(hexToBCD,3); // Place String in Display
          ADCMCTL0 &= ~ADCINCH_2; // Disable Last channel A2
          ADCMCTL0 |= ADCINCH_3; // Enable Next channel A3
          ADCCTL0 |= ADCENC; // Enable Conversions
          ADCCTL0 |= ADCSC;
          break;
          case 0x01:
              ADC_Left_Detect = ADCMEM0; // Move result into Global
              ADC_Left_Detect = ADC_Left_Detect >> 2; // Divide the result by 4
//              HEXtoBCD(hexToBCD ,ADC_Left_Detect,6); // Convert result to String
//              adc_line(hexToBCD,3); // Place String in Display
              ADCMCTL0 &= ~ADCINCH_3; // Disable Last channel A2
              ADCMCTL0 |= ADCINCH_5; // Enable Next channel A3
                                      //Replicate for each channel being read
              ADCCTL0 |= ADCENC; // Enable Conversions
              ADCCTL0 |= ADCSC; //
              break; //
          case 0x02:
              ADC_TW_Detect = ADCMEM0; // Move result into Global
              ADC_TW_Detect = ADC_TW_Detect >> 2; // Divide the result by 4
              //HEXtoBCD(ADC_TW_Detect); // Convert result to String
              //hexToBCD[8] = 'T';
              //hexToBCD[9] = 'W';
              //adc_line(0); // Place String in Display
              ADCMCTL0 &= ~ADCINCH_5; // Disable Last channel A2
              ADCMCTL0 |= ADCINCH_2; // Enable Next channel A3
              ADC_Channel = 0;
                        //Replicate for each channel being read
          break;
          default:
          break;
          }
         break;
     default:
         break;
 }
}


void Init_Timer_B3(void) {
//------------------------------------------------------------------------------
// SMCLK source, up count mode, PWM Right Side
// TB3.1 P6.0 LCD_BACKLITE
// TB3.2 P6.1 R_FORWARD
// TB3.3 P6.2 R_REVERSE
// TB3.4 P6.3 L_FORWARD
// TB3.5 P6.4 L_REVERSE
//------------------------------------------------------------------------------
 TB3CTL = TBSSEL__SMCLK; // SMCLK
 TB3CTL |= MC__UP; // Up Mode
 TB3CTL |= TBCLR; // Clear TAR

 PWM_PERIOD = WHEEL_PERIOD; // PWM Period [Set this to 50005]

 TB3CCTL1 = OUTMOD_7; // CCR1 reset/set
 LCD_BACKLITE_DIMING = PERCENT_80; // P6.0 Right Forward PWM duty cycle

 TB3CCTL2 = OUTMOD_7; // CCR2 reset/set
 RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.1 Right Forward PWM duty cycle

 TB3CCTL3 = OUTMOD_7; // CCR3 reset/set
 LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.2 Left Forward PWM duty cycle

 TB3CCTL4 = OUTMOD_7; // CCR4 reset/set
 RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Right Reverse PWM duty cycle

 TB3CCTL5 = OUTMOD_7; // CCR5 reset/set
 LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.4 Left Reverse PWM duty cycle
//------------------------------------------------------------------------------
}
