//goal is to update every 5 milliseconds
//00 . 01. 10. 11
#define TB0CCR0_INTERVAL (2500)
#define ONESEC (50000)
#define FIFTY_MS_COUNT (100)

volatile unsigned int Time_Sequence;
extern unsigned int start_p5;
unsigned int p5timer = 0;
unsigned int blink_count = 0;
unsigned volatile int countersw1 = 0;
unsigned volatile int countersw2 = 0;



extern unsigned char sw1_position;
extern unsigned char sw1_debounce;
extern unsigned char sw2_position;
extern unsigned char sw2_debounce;
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern char display_line[4][11];



#include  "msp430.h"
#include  <string.h>
#include  "Include\functions.h"
#include  "Include\LCD.h"
#include  "Include\ports.h"
#include  "Include\macros.h"

void Init_Timers(void) {
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
blink_count +=1;

//LCD Backlight
if(blink_count > FIFTY_MS_COUNT){
    if(sw1_debounce != 1 && sw2_debounce != 1){
        P6OUT ^= LCD_BACKLITE;
    }
    blink_count = 0;
}

if(Time_Sequence % 40 == 0){
    update_display = TRUE;
    Time_Sequence = 0;
    if(start_p5 == 1){
        p5timer += 1;
    }
}

if(Time_Sequence %2 == 0){
    ADCCTL0 |= ADCENC; // Enable Conversions
    ADCCTL0 |= ADCSC;
}


// Add Offset to TBCCR0
//---------------------------------------------------------------------------
}


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
                    strcpy(display_line[3], "          ");
                    display_changed = TRUE;
        } else {
            countersw1++;
            TB0CCR1+=ONESEC;
        }

        break;

    case 4:

        if(countersw2 >= 10){
            //disable timer b0 ccr1,
                    //clear sw1 interrupt flag
                    //enable sw1 interrupt
                    sw2_position = 0; //sw1 is pressed//off
                    sw2_debounce = 0; //off
                    P2IFG &= ~SW2;
                    P2IE |= SW2;
                    TB0CCTL2 &= ~CCIE;
                    if(sw1_debounce == 0){
                    P6OUT |= LCD_BACKLITE;}
                    strcpy(display_line[3], "          ");
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
          ADC_Left_Detect = ADCMEM0; // Move result into Global
          ADC_Left_Detect = ADC_Left_Detect >> 2; // Divide the result by 4
          HEXtoBCD(ADC_Left_Detect); // Convert result to String
          adc_line4(0); // Place String in Display
          ADCMCTL0 &= ~ADCINCH_2; // Disable Last channel A2
          ADCMCTL0 |= ADCINCH_3; // Enable Next channel A3
          ADCCTL0 |= ADCENC; // Enable Conversions
         ADCCTL0 |= ADCSC;
          break;
          case 0x01:
              ADC_Right_Detect = ADCMEM0; // Move result into Global
              ADC_Right_Detect = ADC_Right_Detect >> 2; // Divide the result by 4
              HEXtoBCD(ADC_Right_Detect); // Convert result to String
              adc_line4(0); // Place String in Display
              ADCMCTL0 &= ~ADCINCH_3; // Disable Last channel A2
              ADCMCTL0 |= ADCINCH_5; // Enable Next channel A3
          //Replicate for each channel being read
              ADCCTL0 |= ADCENC; // Enable Conversions
              ADCCTL0 |= ADCSC;
          case 0x02:
              Thumb_Wheel = ADCMEM0; // Move result into Global
              Thumb_Wheel = Thumb_Wheel >> 2; // Divide the result by 4
              HEXtoBCD(Thumb_Wheel); // Convert result to String
              adc_line4(0); // Place String in Display
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
