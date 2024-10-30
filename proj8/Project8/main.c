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

int LMS = 15;
int RMS = 15;
extern unsigned  int adjustcounter;
extern  int LvR;
unsigned  int nump = 0;

#define TSOFFS (10)

unsigned int stickyRight = 0;
unsigned int stickyLeft = 0;
unsigned int p6act = 0;
unsigned int p6timer = 0;

// Function Prototypes
void main(void);
void Init_Conditions(void);
void Display_Process(void);
void Init_LEDs(void);
void Carlson_StateMachine(void);
void p6cycle(void);
void start_case(void);
void run_forward(void);
void reverse(void);
void run_turn(void);
void end_case(void);
int wheel_speed(unsigned int side,unsigned int percentage);

extern char USB_Char_Tx[11];
extern char USB_Char_Rx[11];


  // Global Variables
unsigned volatile int lastAction;
volatile char slow_input_down; //
extern char display_line[4][11]; //
extern char *display[4];    //
unsigned char display_mode; //
extern volatile unsigned char display_changed; //
extern volatile unsigned char update_display; //
extern volatile unsigned int update_display_count; //
extern volatile unsigned int Time_Sequence; //
extern unsigned char sw1_position; //button 1 position
extern unsigned char sw2_position; //button 2 position
extern volatile char one_time; //
extern unsigned int event; //
extern unsigned int START_RUNNING; //
unsigned int waittime = 0; //
extern unsigned int gotime; //
extern unsigned volatile int ADC_TW_Detect; //
extern unsigned int turncounter;
extern unsigned int turncounter0;
extern char hexToBCD[11];
extern char labelArr[11];


extern unsigned int circleActive;
extern unsigned int circleTimer;

extern unsigned int button1state;
extern unsigned int blackVal;
extern unsigned int whiteVal;



extern unsigned volatile int ADC_Left_Detect; //left light detector
extern unsigned volatile int ADC_Right_Detect; //right light detector
unsigned int p6part = 0;
unsigned int p8part = 0;

extern unsigned int baudmode;
extern unsigned int charcount;
extern volatile char ncsuArray[11];





//void main(void){
void main(void){
//    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

//------------------------------------------------------------------------------
// Main Program
// This is the main routine for the program. Execution of code starts here.
// The operating system is Back Ground Fore Ground.
//

//TWO CIRCLES IS HEX 40
//------------------------------------------------------------------------------

  PM5CTL0 &= ~LOCKLPM5;
// Disable the GPIO power-on default high-impedance mode to activate
// previously configured port settings

  Init_Ports();                        // Initialize Ports
  Init_Clocks();                       // Initialize Clock System
  Init_Conditions();                   // Initialize Variables and Initial Conditions
  Init_DAC();
  Init_Timers();                       // Initialize Timers
  Init_LCD();                          // Initialize LCD
  Init_Serial_UCA1(); // Initialize Serial Port for USB


//P2OUT &= ~RESET_LCD;
  // Place the contents of what you want on the display, in between the quotes
// Limited to 10 characters per line
  strcpy(display_line[0], "          ");
  strcpy(display_line[1], "          ");
  strcpy(display_line[2], "          ");
  strcpy(display_line[3], "          ");
  display_changed = TRUE;
//  Display_Update(0,0,0,0);

//------------------------------------------------------------------------------
// Beginning of the "While" Operating System
//------------------------------------------------------------------------------
  while(ALWAYS) {                       // Can the Operating system run
//LCD_BACKLITE_DIMING = wheel_speed(R, 20);
    Switches_Process();                 // Check for switch state change
    Display_Process();                  // Update Display
    P3OUT ^= TEST_PROBE;                //
    run_hw8();

//    if(START_RUNNING){
//        p6cycle();
//    }
//  }
//------------------------------------------------------------------------------

  } //end of while always

} //end of main






//homework 8 cases

void run_hw8(void){
    switch(p8part){
    case 0: start_case();
    break;
    case 1: if(waittime >= 200) {//if it has been 2 seconds
        strcpy(display_line[1], "          ");
        p8part += 1;
        gotime = 0;
    } else{
        strcpy(display_line[1], "  buffer  ");
    }
    break;
    case 2:
        if(baudmode > 0){
        strcpy(display_line[0], "  buffer  ");
        if(charcount < 11){
            USB_Char_Tx[charcount] = ncsuArray[charcount];
            charcount += 1;
        } else {
            UCA1IE |= UCTXIE;
            p8part += 1;
        }
        }
        break;
    case 3:
        strcpy(display_line[0], USB_Char_Rx);
default: break;
    }


    }

void start_case(void){
    gotime = 1;
    charcount = 0;
    if(baudmode == 1){
        UCA1BRW = 4;
        UCA1MCTLW = 0x5551 ;
        strcpy(display_line[2], "   baud   ");
        strcpy(display_line[3], "  115200  ");
    } else if(baudmode == 2){
        UCA1BRW = 17;
        UCA1MCTLW = 0x4A00 ;
        strcpy(display_line[2], "   baud   ");
        strcpy(display_line[3], "  460800  ");
    }
    p8part +=1;

}

/*

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
    case 4:
        if(circleTimer % 5 == 0){
            labelArr[0] = ' ';
            labelArr[1] = ' ';
            labelArr[2] = ' ';
            labelArr[3] = ' ';
            labelArr[4] = ' ';
            labelArr[8] = ' ';
            labelArr[9] = ' ';
            HEXtoBCD(labelArr,circleTimer / 5, 5);
            adc_line(labelArr,2);
        }
        nump = circleTimer / 5;
        if(nump < TWOCIRCLE){
            steer_circle();
        } else {
            p6part += 1;
            circleTimer = 0;
        }
    break;
    case 5:
        if(circleTimer < 3){
            run_turn2();
        } else {

            RIGHT_FORWARD_SPEED = WHEEL_OFF;
            LEFT_FORWARD_SPEED = WHEEL_OFF;
            RIGHT_REVERSE_SPEED = WHEEL_OFF;
            LEFT_REVERSE_SPEED = WHEEL_OFF;
            p6part += 1;
            circleTimer = 0;
        };
    break;
    case 6:
        if(circleTimer < 5){
            run_forward2();
        } else {
            RIGHT_FORWARD_SPEED = WHEEL_OFF;
            LEFT_FORWARD_SPEED = WHEEL_OFF;
            RIGHT_REVERSE_SPEED = WHEEL_OFF;
            LEFT_REVERSE_SPEED = WHEEL_OFF;
        }
    break;
    case 7: end_case();
        break;
    default:
        break;
    }
}

void run_forward2(void){
    strcpy(display_line[0], "  ball    ");
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED = WHEEL_OFF;
    RIGHT_FORWARD_SPEED = wheel_speed(L,45);
    LEFT_FORWARD_SPEED = wheel_speed(R,45);
}

void run_turn2(void){

    if(ADC_Right_Detect >= whiteVal && ADC_Left_Detect >= whiteVal){
    strcpy(display_line[3], " turning  ");
    RIGHT_FORWARD_SPEED = wheel_speed(L,45); //this is the left wheel
    LEFT_REVERSE_SPEED = wheel_speed(R,45); //this is actually right wheel

    }

}



void start_case(void){
    if(waittime == TWOH){
        gotime = 0;
        p6act = 0;
        p6timer = 0;
        p6part = 1;
        adjustcounter = 0;
    }
}


//Patch note 0.1:  left turn readjustment for action 1.3 is a little too aggressive
// Fixed by decreasing the turn aggressiveness for that arc.
//should turn with the gradient
//patch 0.2: make the inner wheel slower, not the outer wheel faster
//patch 0.3: stopped the "straight" function and just had it turn on the wheel.
//try a zero point turn
void steer_circle(void){
    if(ADC_Right_Detect > blackVal && ADC_Left_Detect > blackVal){
            RMS = 35; //Right motor speed set to base
            LMS = 20; //left motor speed set to base
            strcpy(display_line[3], " straight "); //go straight if both sensors detect black
            display_changed = TRUE;
            RIGHT_REVERSE_SPEED = WHEEL_OFF;
            LEFT_REVERSE_SPEED = WHEEL_OFF;
            RIGHT_FORWARD_SPEED = wheel_speed(L, 22); //LEFT wheel
            LEFT_FORWARD_SPEED = wheel_speed(R, 16);  //RIGHT wheel
            stickyRight = 0;
            stickyLeft = 0;
        }
    else if((ADC_Right_Detect >= blackVal && ADC_Left_Detect < blackVal) || stickyRight == 1){ //turn right
        steerRight();
    } else if((ADC_Right_Detect <= blackVal && ADC_Left_Detect > blackVal) || stickyLeft == 1){ //turn left
        steerLeft();
    }
     else {
        strcpy(display_line[3], "i give up"); //both sensors dont detect black
        display_changed = TRUE;

        if(lastAction == 1){ //If last action was a right turn, then turn left. 1.3
                    LEFT_REVERSE_SPEED = WHEEL_OFF;
                    RIGHT_REVERSE_SPEED = WHEEL_OFF;
                    LEFT_FORWARD_SPEED = wheel_speed(R, 30); //Right wheel
                    RIGHT_FORWARD_SPEED = wheel_speed(L, 20);  //Left wheel
                    strcpy(display_line[3], "   adj L  "); //both sensors dont detect black
                    display_changed = TRUE;
                    LMS = 27;
                    if(turncounter >= 3){
                        lastAction = 0;
                        turncounter = 0;
                        turncounter0 = 0;
                    }
        } else { //if last action was a left turn, then turn right
            LEFT_REVERSE_SPEED = WHEEL_OFF;
            RIGHT_REVERSE_SPEED = WHEEL_OFF;
            LEFT_FORWARD_SPEED = wheel_speed(R, 15); //Right wheel
            RIGHT_FORWARD_SPEED = wheel_speed(L, 35);  //Left wheel
            strcpy(display_line[3], "  R adj   "); //both sensors dont detect black
            display_changed = TRUE;
            if(turncounter0 >= 5){
                steerRight();
                lastAction = 0;
                lastAction = 3;
                }

        }
    }
}

void steerRight(void){
    strcpy(display_line[3], " steeringR");
        stickyRight = 1;
        stickyLeft = 0;
            display_changed = TRUE;
            LMS = 45;
            lastAction = 1; //swe are turning right
            turncounter = 0;
            RIGHT_REVERSE_SPEED = WHEEL_OFF; //turn off the reverse
            LEFT_FORWARD_SPEED = WHEEL_OFF;//turn off the reverse so i dont burn out my fets
            LEFT_REVERSE_SPEED = wheel_speed(R, 10); //Right wheel (pivot)
            RIGHT_FORWARD_SPEED = wheel_speed(L, LMS);  //Left wheel
}

void steerLeft(void){
    strcpy(display_line[3], "Lsteering ");
            stickyLeft = 1;
            stickyRight = 0;
            display_changed = TRUE;
            RMS = 25; //reset left wheel speed
            lastAction = 0; //we are turning left
            RIGHT_FORWARD_SPEED = WHEEL_OFF;
            LEFT_REVERSE_SPEED = WHEEL_OFF;
            LEFT_FORWARD_SPEED = wheel_speed(R, RMS); //Right wheel
            RIGHT_REVERSE_SPEED = wheel_speed(L, 11);  //Left wheel
}

//patch note 0.1
//have my left turn lower than my right turn to accomodate for the
//gradient of the circle going clockwise.
//0.2, change my left turn to be less steep
//speed up wheels so it doesnt get stuck
//0.3, after cancelling the straight function, slow down the turn here and on the "straight"


void reverse(void){
    if(p6timer < TSOFFS){
        RIGHT_REVERSE_SPEED = wheel_speed(L, 70);
        LEFT_REVERSE_SPEED = wheel_speed(R, 70);
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
    if(START_RUNNING && ADC_Right_Detect < blackVal && ADC_Left_Detect < blackVal){
            RIGHT_FORWARD_SPEED = wheel_speed(L,65); //this is the left wheel
            LEFT_FORWARD_SPEED = wheel_speed(R,65); //this is actually right wheel
            strcpy(display_line[3], "  forward "); //

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
    if(START_RUNNING  && (ADC_Right_Detect < blackVal && ADC_Left_Detect < blackVal)
            ){
              RIGHT_FORWARD_SPEED = wheel_speed(L,40); //this is the left wheel
               LEFT_REVERSE_SPEED = wheel_speed(R,40); //this is actually right wheel
                strcpy(display_line[3], " turning  ");

            }
    else {
                RIGHT_FORWARD_SPEED = WHEEL_OFF;
                LEFT_FORWARD_SPEED = WHEEL_OFF;
                RIGHT_REVERSE_SPEED = WHEEL_OFF;
                LEFT_REVERSE_SPEED = WHEEL_OFF;
                circleTimer = 0;
                circleActive = 1;
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
    adjustcounter = 0;
    p6act = 0;
    START_RUNNING = FALSE;
    strcpy(display_line[3], "          ");
}
**/
