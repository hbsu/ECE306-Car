//===========================================================================
//  File Name : ports.c
//
//  Description: This file contains the Initialization for all port pins
//
//  Author: Henry Su
//  Date: September 2024
//  Compiler: Built with IAR Embedded Workbench Version: V4.10A/W32 (5.40.1)
//===========================================================================

// TB3.1 P6.0 LCD_BACKLITE
 // TB3.2 P6.1 R_FORWARD
 // TB3.3 P6.2 R_REVERSE
 // TB3.4 P6.3 L_FORWARD
// TB3.5 P6.4 L_REVERSE
#include "Include\ports.h"


#include "msp430.h"


void Init_Ports(void){
    Init_Port1();
    Init_Port2();
    Init_Port3();
    Init_Port4();
    Init_Port5();
    Init_Port6();
    Init_ADC();

}

void Init_ADC(void){
    //------------------------------------------------------------------------------
    // V_DETECT_L (0x04) // Pin 2 A2
    // V_DETECT_R (0x08) // Pin 3 A3
    // V_THUMB (0x20) // Pin 5 A5
    //------------------------------------------------------------------------------
    // ADCCTL0 Register
     ADCCTL0 = 0; // Reset
     ADCCTL0 |= ADCSHT_2; // 16 ADC clocks
     ADCCTL0 |= ADCMSC; // MSC
     ADCCTL0 |= ADCON; // ADC ON
    // ADCCTL1 Register\

     ADCCTL1 = 0; // Reset
     ADCCTL1 |= ADCSHS_0; // 00b = ADCSC bit
     ADCCTL1 |= ADCSHP; // ADC sample-and-hold SAMPCON signal from sampling timer.
     ADCCTL1 &= ~ADCISSH; // ADC invert signal sample-and-hold.
     ADCCTL1 |= ADCDIV_0; // ADC clock divider - 000b = Divide by 1
     ADCCTL1 |= ADCSSEL_0; // ADC clock MODCLK
     ADCCTL1 |= ADCCONSEQ_0; // ADC conversion sequence 00b = Single-channel single-conversion
    // ADCCTL1 & ADCBUSY identifies a conversion is in process
    // ADCCTL2 Register

     ADCCTL2 = 0; // Reset
     ADCCTL2 |= ADCPDIV0; // ADC pre-divider 00b = Pre-divide by 1
     ADCCTL2 |= ADCRES_2; // ADC resolution 10b = 12 bit (14 clock cycle conversion time)
     ADCCTL2 &= ~ADCDF; // ADC data read-back format 0b = Binary unsigned.
     ADCCTL2 &= ~ADCSR; // ADC sampling rate 0b = ADC buffer supports up to 200 ksps
    // ADCMCTL0 Register

     ADCMCTL0 |= ADCSREF_0; // VREF - 000b = {VR+ = AVCC and VR– = AVSS }
     ADCMCTL0 |= ADCINCH_2; // V_THUMB (0x20) Pin 5 A5
     ADCIE |= ADCIE0; // Enable ADC conv complete interrupt
     ADCCTL0 |= ADCENC; // ADC enable conversion.
     ADCCTL0 |= ADCSC; // ADC start conversion.




}

void Init_Port1(void){
    P1OUT = 0x00; //P1 set low
    P1DIR = 0x00; //Set p1 direction to ouput.

    //pin 0
    P1SEL0 &= ~RED_LED; //redled opepraton
    P1SEL1 &= ~RED_LED; //redled gpio operation
    P1OUT &= ~RED_LED; //Initial value should be low
    P1DIR |= RED_LED;  //direction = output

    //A1_SEEED
    P1SEL0 |= V_A1_SEEED;
    P1SEL1 |= V_A1_SEEED;
    P1OUT &= ~RED_LED; //initialize to low

    //pin 2
    P1SEL0 |= V_DETECT_L;
    P1SEL1 |= V_DETECT_L;

    //pin 3 V_DETECT_R
    P1SEL0 |= V_DETECT_R; //V_detect gpio operation
    P1SEL1 |= V_DETECT_R;

    //Pin 4 A4_SEEED
    P1SEL0 |= A4_SEEED;
    P1SEL1 |= A4_SEEED;
    P1OUT &= ~A4_SEEED;

    //Pin 5 V_THUMB
    P1SEL0 |= V_THUMB;
    P1SEL1 |= V_THUMB;


    //Pin 6 Back Channel UCA0RXD
    P1SEL0 |= UCA0RXD;
    P1SEL1 &= ~UCA0RXD;
    P1OUT &= ~UCA0RXD; //initialize to low

    //Pin 7 Back Channel UCA0TXD
    P1SEL0 |= UCA0TXD;
    P1SEL1 &= ~UCA0TXD;
    P1OUT &= ~UCA0TXD; //initialize to low

}

void Init_Port2(void){ // Configure Port 2
//------------------------------------------------------------------------------
 P2OUT = 0x00; // P2 set Low
 P2DIR = 0x00; // Set P2 direction to output

 P2SEL0 &= ~SLOW_CLK; // RESET_LCD GPIO operation
 P2SEL1 &= ~SLOW_CLK; // RESET_LCD GPIO operation
 P2OUT &= ~SLOW_CLK; // Initial Value = Low / Off
 P2DIR |= SLOW_CLK; // Direction = output

 P2SEL0 &= ~CHECK_BAT; // CHECK_BAT GPIO operation
 P2SEL1 &= ~CHECK_BAT; // CHECK_BAT GPIO operation
 P2OUT &= ~CHECK_BAT; // Initial Value = Low / Off
 P2DIR |= CHECK_BAT; // Direction = output

 P2SEL0 &= ~IR_LED; // P2_2 GPIO operation
 P2SEL1 &= ~IR_LED; // P2_2 GPIO operation
 P2OUT |= IR_LED; // Initial Value = Low / Off
 P2DIR |= IR_LED; // Direction = input

 P2SEL0 &= ~SW2; // SW2 Operation
 P2SEL1 &= ~SW2; // SW2 Operation
 P2OUT |= SW2; // Configure pullup resistor
 P2DIR &= ~SW2; // Direction = input
 P2REN |= SW2; // Enable pullup resistor
 P2IES |= SW2; //generate interrupt request on falling edge
 P2IFG &= ~SW2; //Interrupt Flag sw1
 P2IE |= SW2; //enable interrupt


 P2SEL0 &= ~IOT_RUN_RED; // IOT_RUN_CPU GPIO operation
 P2SEL1 &= ~IOT_RUN_RED; // IOT_RUN_CPU GPIO operation
 P2OUT &= ~IOT_RUN_RED; // Initial Value = Low / Off
 P2DIR |= IOT_RUN_RED; // Direction = input

 P2SEL0 &= ~DAC_ENB; // DAC_ENB GPIO operation
 P2SEL1 &= ~DAC_ENB; // DAC_ENB GPIO operation
 P2OUT |= DAC_ENB; // Initial Value = High
 P2DIR |= DAC_ENB; // Direction = output

 P2SEL0 &= ~LFXOUT; // LFXOUT Clock operation
 P2SEL1 |= LFXOUT; // LFXOUT Clock operation

 P2SEL0 &= ~LFXIN; // LFXIN Clock operation
 P2SEL1 |= LFXIN; // LFXIN Clock operation
 //------------------------------------------------------------------------------
}


void Init_Port3(void){// Configure port 3
    //initialize p3
    P3OUT = 0x00;
    P3DIR = 0x00;

    //Port 3 pin0 Test Probe
    P3SEL0 &= ~TEST_PROBE;
    P3SEL1 &= ~TEST_PROBE;
    P3OUT &= ~TEST_PROBE;
    P3DIR |= TEST_PROBE;

    //Port 3 pin 1 OA2O
    P3SEL0 |= OA2O;
    P3SEL1 |= OA2O;

    //Port 3 pin 2 OA2N
    P3SEL0 |= OA2N;
    P3SEL1 |= OA2N;

    //Port 3 Pin 3 OA2P
    P3SEL0 |= OA2P;
    P3SEL1 |= OA2P;

    //Port 3 Pin 4 SMCLK
    P3SEL0 &= ~SMCLK;
    P3SEL0 &= ~SMCLK;
    P3DIR &= ~SMCLK;

    //Port 3 Pin 5 DAC_CNTL
    P3SEL0 |= DAC_CNTL;
    P3SEL1 |= DAC_CNTL;
    P3DIR |= DAC_CNTL;
    P3OUT &= ~DAC_CNTL;

    //Port 3 Pin 6 IOT_LINK_CPU
    P3SEL0 &= ~IOT_LINK_CPU;
    P3SEL1 &= ~IOT_LINK_CPU;
    P3DIR |= IOT_LINK_CPU;
    P3OUT &= ~IOT_LINK_CPU;

    //Port 3 Pin 7 IOT_EN_CPU
    P3SEL0 &= ~IOT_EN_CPU;
    P3SEL1 &= ~IOT_EN_CPU;
    P3DIR |= IOT_EN_CPU;
    P3OUT &= ~IOT_EN_CPU;

}




void Init_Port4(void){ // Configure PORT 4
//------------------------------------------------------------------------------
 P4OUT = 0x00; // P4 set Low
 P4DIR = 0x00; // Set P4 direction to output

 P4SEL0 &= ~RESET_LCD; // RESET_LCD GPIO operation
 P4SEL1 &= ~RESET_LCD; // RESET_LCD GPIO operation
 P4OUT &= ~RESET_LCD; // Initial Value = Low / Off
 P4DIR |= RESET_LCD; // Direction = output

 P4SEL0 &= ~SW1; // SW1 Operation
 P4SEL1 &= ~SW1; // SW1 Operation
 P4OUT |= SW1; // Configure pullup resistor
 P4DIR &= ~SW1; // Direction = input
 P4REN |= SW1; // Enable pullup resistor
 P4IES |= SW1; //generate interrupt request on falling edge
 P4IFG &= ~SW1; //Interrupt Flag sw1
 P4IE |= SW1; //enable interrupt

 P4SEL0 |= UCA1TXD; // USCI_A1 UART operation
 P4SEL1 &= ~UCA1TXD; // USCI_A1 UART operation

 P4SEL0 |= UCA1RXD; // USCI_A1 UART operation
 P4SEL1 &= ~UCA1RXD; // USCI_A1 UART operation

 P4SEL0 &= ~UCB1_CS_LCD; // UCB1_CS_LCD GPIO operation
 P4SEL1 &= ~UCB1_CS_LCD; // UCB1_CS_LCD GPIO operation

 P4OUT |= UCB1_CS_LCD; // Set SPI_CS_LCD Off [High]
 P4DIR |= UCB1_CS_LCD; // Set SPI_CS_LCD direction to output

 P4SEL0 |= UCB1CLK; // UCB1CLK SPI BUS operation
 P4SEL1 &= ~UCB1CLK; // UCB1CLK SPI BUS operation

 P4SEL0 |= UCB1SIMO; // UCB1SIMO SPI BUS operation
 P4SEL1 &= ~UCB1SIMO; // UCB1SIMO SPI BUS operation

 P4SEL0 |= UCB1SOMI; // UCB1SOMI SPI BUS operation
 P4SEL1 &= ~UCB1SOMI; // UCB1SOMI SPI BUS operation
 //------------------------------------------------------------------------------
}


void Init_Port5(void){
    P5OUT = 0x00;
    P5DIR = 0x00;

    //P0 V_BAT
    P5SEL0 &= ~V_BAT; //gpio v_bat
    P5SEL1 &= ~V_BAT;  //gpio v_bat
    P5OUT &= ~V_BAT; //low output
    P5DIR |= V_BAT; //set to output

    //P1 V_5
    P5SEL0 &= ~V_5; //GPIO V_5
    P5SEL1 &= ~V_5;
    P5OUT &= ~V_5; //low output
    P5DIR |= V_5; // set to output


    //P2 V_DAC
    P5SEL0 &= ~V_DAC; //GPIO V_DAC
    P5SEL1 &= ~V_DAC; //GPIO V_DAC
    P5OUT &= ~V_DAC; //low output
    P5DIR |= V_DAC; //set to output

    //P3 V_3
    P5SEL0 &= ~V_3; //GPIO V_3.3
    P5SEL1 &= ~V_3; //GPIO V_3.3
    P5OUT &= ~V_3; //low output
    P5DIR |= V_3; //set to output



    //P4 IOT_BOOT_CPU
    P5SEL0 &= ~IOT_BOOT_CPU; //GPIO IOT_BOOT_CPU
    P5SEL1 &= ~IOT_BOOT_CPU; //GPIO IOT_BOOT_CPU
    P5OUT &= ~IOT_BOOT_CPU; // Low output
    P5DIR |= IOT_BOOT_CPU; //Set to output

}



void Init_Port6(void){
    P6OUT = 0x00;
    P6DIR = 0x00;


    //LC_BACKLITE
    P6SEL0 |= LCD_BACKLITE;
    P6SEL1 &= ~LCD_BACKLITE;
    P6OUT |= LCD_BACKLITE;
    P6DIR |= LCD_BACKLITE;


    //R_FORWARD
    P6SEL0 |= R_FORWARD;
    P6SEL1 &= ~R_FORWARD;
    P6DIR |= R_FORWARD;

    //L_FORWARD
    P6SEL0 |= L_FORWARD;
    P6SEL1 &= ~L_FORWARD;
    P6DIR |= L_FORWARD;

    //R_REVERSE
    P6SEL0 |= R_REVERSE;
    P6SEL1 &= ~R_REVERSE;
    P6DIR |= R_REVERSE;


    //L_REVERSE
    P6SEL0 |= L_REVERSE;
    P6SEL1 &= ~L_REVERSE;
    P6DIR |= L_REVERSE;

    //P6_5
    P6SEL0 &= ~P6_5;
    P6SEL1 &= ~P6_5;
    P6OUT &= ~P6_5;
    P6DIR |= P6_5;

    //GRN_LED
    P6SEL0 &= ~GRN_LED;
    P6SEL1 &= ~GRN_LED;
    P6OUT &= ~GRN_LED;
    P6DIR |= GRN_LED;

}
