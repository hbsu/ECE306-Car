//------------------------------------------------------------------------------
//
//  Description: This file contains the Main Routine - "While" Operating System
//
//  Jim Carlson
//  Jan 2023
//  Built with Code Composer Version: CCS12.4.0.00007_win64
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include  "msp430.h"
#include  <string.h>
#include  "Include\functions.h"
#include  "Include\LCD.h"
#include  "Include\ports.h"
#include  "Include\macros.h"
#include "Include\header.h"

 #define PERCENT_80  (45000)




#define TSOFFS (30)

unsigned volatile int p6act = 0;
unsigned volatile int p6timer = 0;

// Function Prototypes
void main(void);
void Init_Conditions(void);
void Display_Process(void);
void Init_LEDs(void);
void Carlson_StateMachine(void);


  // Global Variables
volatile char slow_input_down;
extern char display_line[4][11];
extern char *display[4];
unsigned char display_mode;
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern volatile unsigned int update_display_count;
extern volatile unsigned int Time_Sequence;
extern unsigned char sw1_position; //button 1 position
extern unsigned char sw2_position; //button 2 position
extern volatile char one_time;
extern unsigned int event;
extern unsigned int START_RUNNING;
unsigned volatile int waittime = 0;
extern unsigned volatile int gotime;



extern unsigned volatile int ADC_Left_Detect; //left light detector
extern unsigned volatile int ADC_Right_Detect; //right light detector
unsigned volatile int p6part = 0;








//void main(void){
void main(void){
//    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

//------------------------------------------------------------------------------
// Main Program
// This is the main routine for the program. Execution of code starts here.
// The operating system is Back Ground Fore Ground.
//
//------------------------------------------------------------------------------
  PM5CTL0 &= ~LOCKLPM5;
// Disable the GPIO power-on default high-impedance mode to activate
// previously configured port settings

  Init_Ports();                        // Initialize Ports
  Init_Clocks();                       // Initialize Clock System
  Init_Conditions();                   // Initialize Variables and Initial Conditions
  Init_Timers();                       // Initialize Timers
  Init_LCD();                          // Initialize LCD
//P2OUT &= ~RESET_LCD;
  // Place the contents of what you want on the display, in between the quotes
// Limited to 10 characters per line
  strcpy(display_line[0], "          ");
  strcpy(display_line[1], " circ     ");
  strcpy(display_line[2], "          ");
  strcpy(display_line[3], "          ");
  display_changed = TRUE;
//  Display_Update(0,0,0,0);

//------------------------------------------------------------------------------
// Begining of the "While" Operating System
//------------------------------------------------------------------------------
  while(ALWAYS) {                      // Can the Operating system run
    Switches_Process();                // Check for switch state change
    Display_Process();                 // Update Display
    P3OUT ^= TEST_PROBE;
    if(START_RUNNING){
        p6cycle();
    }
  }
//------------------------------------------------------------------------------
}

void p6cycle(void){
    switch (p6part){
    case 0: start_case();
    break;
    case 1: run_forward();
    break;
    case 2: reverse();
    break;
    case 3: run_turn();
    break;
    case 4: end_case();
    break;
    }
}

void start_case(void){
    if(waittime == TWOH){
        gotime = 0;
        p6act = 0;
        p6timer = 0;
        p6part = 1;
    }
}


void reverse(void){
    if(p6timer < TSOFFS){
        RIGHT_REVERSE_SPEED = wheel_speed(L, 30);
        LEFT_REVERSE_SPEED = wheel_speed(R, 30);
        strcpy(display_line[3], "   rev    ");

    }else {
        RIGHT_FORWARD_SPEED = WHEEL_OFF;
        LEFT_FORWARD_SPEED = WHEEL_OFF;
        RIGHT_REVERSE_SPEED = WHEEL_OFF;
       LEFT_REVERSE_SPEED = WHEEL_OFF;
        p6part = 3;
      p6act = FALSE;
    }
}

void run_forward(void){
    if(START_RUNNING && ADC_Right_Detect < BLACK && ADC_Left_Detect < BLACK){
            RIGHT_FORWARD_SPEED = wheel_speed(L,70); //this is the left wheel
            LEFT_FORWARD_SPEED = wheel_speed(R,70); //this is actually right wheel
            strcpy(display_line[3], "  forward ");

        } else {
            RIGHT_FORWARD_SPEED = WHEEL_OFF;
            LEFT_FORWARD_SPEED = WHEEL_OFF;
            RIGHT_REVERSE_SPEED = WHEEL_OFF;
            LEFT_REVERSE_SPEED = WHEEL_OFF;
            p6part = 2;
            p6act = TRUE;
        }
}

void run_turn(void){
    strcpy(display_line[3], " turning  ");
    if(START_RUNNING  && (ADC_Right_Detect < BLACK && ADC_Left_Detect < BLACK)){
              RIGHT_FORWARD_SPEED = wheel_speed(L,35); //this is the left wheel
               LEFT_REVERSE_SPEED = wheel_speed(R,35); //this is actually right wheel
                strcpy(display_line[3], " turning  ");

            }
    else if(START_RUNNING  && (ADC_Right_Detect >= BLACK && ADC_Left_Detect < BLACK)){
                    RIGHT_FORWARD_SPEED = wheel_speed(L,35); //this is the left wheel
                    LEFT_REVERSE_SPEED = wheel_speed(R,35); //this is actually right wheel
                    strcpy(display_line[3], " turningR ");

                }
    else if(START_RUNNING  && (ADC_Right_Detect < BLACK && ADC_Left_Detect >= BLACK)){
                        RIGHT_FORWARD_SPEED = wheel_speed(L,35); //this is the left wheel
                        LEFT_FORWARD_SPEED = wheel_speed(R,35); //this is actually right wheel
                        strcpy(display_line[3], " turningL ");
                    }
    else {
                RIGHT_FORWARD_SPEED = WHEEL_OFF;
                LEFT_FORWARD_SPEED = WHEEL_OFF;
                RIGHT_REVERSE_SPEED = WHEEL_OFF;
                LEFT_REVERSE_SPEED = WHEEL_OFF;
                p6part =4;
            }
}

void end_case(void){
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    LEFT_FORWARD_SPEED = WHEEL_OFF;
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED = WHEEL_OFF;
    p6part = 0;
    p6timer = 0;
    p6act = 0;
    START_RUNNING = FALSE;
    strcpy(display_line[3], "          ");


}
