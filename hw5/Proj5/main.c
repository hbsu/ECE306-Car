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

// STATES ======================================================================

unsigned int trinum = 0; //need 12 rotations to make a triangle 0,2,4,6,8,10 are straight lines. 1,3,5,7,9,11 are rotate 45 degrees
unsigned int p5num = 0;
#define TRAVEL_DISTANCE3 (4)
#define RIGHT_COUNT_TIME3  (10)
#define LEFT_COUNT_TIME3  (10)
#define WHEEL_COUNT_TIME3 (10)
#define TRAVEL_DISTANCE45 (5)
#define RIGHT_COUNT_TIME45 (15)
#define WHEEL_COUNT_TIME45 (20)



#define CIRCLE (0)
#define FIGUREEIGHT (1)
#define TRIANGLE (2)
#define PFIVE (3)
#define WAITING2START (200)

#define TRAVEL_DISTANCE (60)
#define RIGHT_COUNT_TIME (20)
#define LEFT_COUNT_TIME (4)
#define WHEEL_COUNT_TIME (20)
#define WAIT (0)
#define START (1)
#define RUN (2)
#define END (3)

#define TRAVEL_DISTANCE5 (25)
#define TRAVEL_DISTANCE1 (20)
#define RIGHT_COUNT_TIME1 (20)
#define LEFT_COUNT_TIME1 (2)
#define WHEEL_COUNT_TIME1 (20)
#define RIGHT_COUNT_TIME2 (2)
#define LEFT_COUNT_TIME2 (20)

unsigned int state = WAIT;
unsigned int segment_count;
unsigned int right_motor_count;
unsigned int left_motor_count;

// Function Prototypes
void main(void);
void Init_Conditions(void);
void Display_Process(void);
void Init_LEDs(void);
void Carlson_StateMachine(void);
void Forward_On(void);
void Stop_Car(void);
void Turn_Left(void);
void Run_Circle(void);
void pwm_Circle(void);
void start_case(void);
void wait_case(void);
void end_case(void);
void end_case(void);

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
unsigned int prev_timeseq;
unsigned int time_change;
unsigned int delay_start = 0;
unsigned int cycle_time;

unsigned int start_p5 = 0;
extern unsigned int p5timer;

#define RUN1 (4)
#define RUN2 (5)
#define RUN3 (6)
unsigned int figeightnum = 0;



char chosen_direction;
char change;

unsigned int wheel_move;
char forward;




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

  wheel_move = 0;
  forward = TRUE;

//------------------------------------------------------------------------------
// Begining of the "While" Operating System
//------------------------------------------------------------------------------
  while(ALWAYS) {                      // Can the Operating system run
    //Carlson_StateMachine();            // Run a Time Based State Machine
    Switches_Process();                // Check for switch state change
    Display_Process();                 // Update Display
    P3OUT ^= TEST_PROBE;               // Change State of TEST_PROBE OFF
    if(prev_timeseq != Time_Sequence){ //if
        time_change = TRUE;
        prev_timeseq = Time_Sequence;
    }

    if(START_RUNNING == TRUE){
        switch(event){
        case CIRCLE:
            run_Circle();
            break;
        case TRIANGLE:
            run_Tri();
            break;
        case FIGUREEIGHT:
            run_Fig();
            break;
        case PFIVE:
            start_p5 = 1;
            run_p5();
            break;
        default: break;
        }
    }
  }
//------------------------------------------------------------------------------

}

//travel forward 1 seconds, travel reverse 2 seconds,
//travel forward for 1 second
//spin clockwise for 3 second
//spin counterclockwise for 3 second
//pause 1 second in between movements
//waiting to start = 200;
//

void CC_On(void){
    P6OUT &= ~R_REVERSE;
    P6OUT &= ~L_FORWARD;
    P6OUT |= R_FORWARD;
    P6OUT |= L_REVERSE;
}

void C_On(void){

    P6OUT &= ~R_FORWARD;
    P6OUT &= ~L_REVERSE;
    P6OUT |= R_REVERSE;
    P6OUT |= L_FORWARD;
}

void Reverse_On(void){
    P6OUT &= ~R_FORWARD;
    P6OUT &= ~L_FORWARD;
    P6OUT |= R_REVERSE;
    P6OUT |= L_REVERSE;
}

void stop(void){
    P6OUT &= ~R_FORWARD;
    P6OUT &= ~L_FORWARD;
    P6OUT &= ~R_REVERSE;
    P6OUT &= ~L_REVERSE;
}

void run_p5(void){
    start_p5 = 1;
    strcpy(display_line[3], " pfiv     ");
    if(p5timer < 5){
        //wait
        stop();
        strcpy(display_line[2], "   wait   ");
        strcpy(display_line[3], "   1sec   ");
    }
    if(p5timer >= 5 && p5timer < 10){
            //forward 1 sec
        Forward_On();
        strcpy(display_line[2], "  forward ");
        strcpy(display_line[3], "   1sec   ");
        }
    if(p5timer >= 10 && p5timer < 15){
            //wait
        stop();
        strcpy(display_line[2], "   wait   ");
        strcpy(display_line[3], "   1sec   ");
        }
    if(p5timer >= 15 && p5timer < 25){
            //backward 2 sec
        Reverse_On();
        strcpy(display_line[2], " backward ");
        strcpy(display_line[3], "   2sec   ");
        }
    if(p5timer >= 25 && p5timer < 30){
            //wait
        stop();
        strcpy(display_line[2], "   wait   ");
        strcpy(display_line[3], "   1sec   ");
        }
    if(p5timer >= 30 && p5timer < 35){
            //forward 1 sec
        Forward_On();
        strcpy(display_line[2], "  forward ");
        strcpy(display_line[3], "   1sec   ");
        }
    if(p5timer >= 35 && p5timer < 40){
            //wait
        stop();
        strcpy(display_line[2], "   wait   ");
        strcpy(display_line[3], "   1sec   ");
        }
    if(p5timer >= 40 && p5timer < 55){
            //clockwise
        CC_On();
        strcpy(display_line[2], "clockwise ");
        strcpy(display_line[3], "   3sec   ");
        }
    if(p5timer >= 55 && p5timer < 60){
            //wait
        stop();
        strcpy(display_line[2], "   wait   ");
        strcpy(display_line[3], "   1sec   ");

        }
    if(p5timer >= 60 && p5timer < 75){
            //counterclockwise
        C_On();
        strcpy(display_line[2], "countclock");
        strcpy(display_line[3], "   3sec   ");
        }
    if(p5timer > 75){
        end_case();
    }


}




void wait_case(void){
    if(time_change){
        time_change = 0;
        if(delay_start++ >= WAITING2START){
            delay_start = 0;
            p5num += 1;
            state = RUN;
        }
    }
}

void run_Tri(void){
    strcpy(display_line[2], "boingboing");
    display_changed = TRUE;
    switch(state){
    case WAIT:
        wait_case();
        break;
        case START:
            start_case();
            break;
        case RUN:
            pwm_tri();
            break;
        case END:
            end_case();
            break;
        default:break;
    }
}


void pwm_tri(void){
    if(trinum % 2 == 0){
        if(time_change){
                time_change = 0;
                if(segment_count <= TRAVEL_DISTANCE3){
                    if(right_motor_count++ >= RIGHT_COUNT_TIME3){
                        P6OUT &= ~R_FORWARD;
                    }
                    if(left_motor_count++ >= LEFT_COUNT_TIME3){
                        P6OUT &= ~L_FORWARD;
                    }
                    if(cycle_time++ >= WHEEL_COUNT_TIME3){
                        cycle_time = 0;
                        right_motor_count = 0;
                        left_motor_count = 0;
                        segment_count++;
                        Forward_On();
                    }
                } else {
                    if(trinum == 10){
                        state = END;
                    }else {

                        state = START;
                        trinum++;
                    }

                }
            }
    } else if (trinum % 2 == 1){ //45 degrees
        if(time_change){
                time_change = 0;
                if(segment_count <= TRAVEL_DISTANCE45){
                    if(right_motor_count++ >= RIGHT_COUNT_TIME45){
                        P6OUT &= ~R_FORWARD;
                    }
                    if(left_motor_count++ >= CIRCLE){
                        P6OUT &= ~L_FORWARD;
                    }
                    if(cycle_time++ >= WHEEL_COUNT_TIME45){
                        cycle_time = 0;
                        right_motor_count = 0;
                        left_motor_count = 0;
                        segment_count++;
                        Forward_On();
                    }
                } else {
                    if(trinum == 11){
                        state = END;
                    }else{
                        state = START;
                        trinum++;
                    }
                }
            }
    }
}


void run_Fig(void){
    strcpy(display_line[2], "pooppoopss");
    display_changed = TRUE;
    switch(state){
    case WAIT:
        wait_case();
        break;
    case START:
        start_case();
        break;
    case RUN:
        pwm_Fig8();
        break;
    case END:
        end_case();
        break;
    default: break;
    }

}


void pwm_Fig8(void){
    if (figeightnum == 0 || figeightnum == 2){
        if(time_change){
                time_change = 0;
                if(segment_count <= TRAVEL_DISTANCE1){
                    if(right_motor_count++ >= RIGHT_COUNT_TIME1){
                        P6OUT &= ~R_FORWARD;
                    }
                    if(left_motor_count++ >= LEFT_COUNT_TIME1){
                        P6OUT &= ~L_FORWARD;
                    }
                    if(cycle_time++ >= WHEEL_COUNT_TIME1){
                        cycle_time = 0;
                        right_motor_count = 0;
                        left_motor_count = 0;
                        segment_count++;
                        Forward_On();
                    }
                } else {
                    state = START;
                    figeightnum++;
                }
            }
    }


    else if(figeightnum == 1 || figeightnum == 3){
        if(time_change){
                time_change = 0;
                if(segment_count <= TRAVEL_DISTANCE5){
                    if(right_motor_count++ >= RIGHT_COUNT_TIME2){
                        P6OUT &= ~R_FORWARD;
                    }
                    if(left_motor_count++ >= LEFT_COUNT_TIME2){
                        P6OUT &= ~L_FORWARD;
                    }
                    if(cycle_time++ >= WHEEL_COUNT_TIME1){
                        cycle_time = 0;
                        right_motor_count = 0;
                        left_motor_count = 0;
                        segment_count++;
                        Forward_On();
                    }
                } else {
                    if(figeightnum == 3){
                        state = END;
                    }else {
                        figeightnum++;
                        state = START;
                    }
                }
            }
    }
}



void run_Circle(void){
    strcpy(display_line[2], "1234567890");
    display_changed = TRUE;
    switch(state){
    case WAIT: //wait
        wait_case();
        break;
    case START:
        start_case();
        break;
    case RUN:
        pwm_Circle();
        break;
    case END:
        end_case();
        break;
default: break;
    }
}

void Forward_On(void){
    P6OUT |= R_FORWARD;
    P6OUT |= L_FORWARD;
    P6OUT &= ~R_REVERSE;
    P6OUT &= ~L_REVERSE;
}


void start_case(void){
    if(p5num == 3){ //backwards
        cycle_time = 0;
        right_motor_count = 0;
        left_motor_count = 0;
        Reverse_On();
        segment_count = 0;
        state = RUN;
    } else if(p5num == 7){ //clockwise
        cycle_time = 0;
        right_motor_count = 0;
        left_motor_count = 0;
        C_On();
        segment_count = 0;
        state = RUN;
    } else if(p5num == 9){ //counterclockwise
        cycle_time = 0;
        right_motor_count = 0;
        left_motor_count = 0;
        CC_On();
        segment_count = 0;
        state = RUN;
    } else{ //forward
        cycle_time = 0;
        right_motor_count = 0;
        left_motor_count = 0;
        Forward_On();
        segment_count = 0;
        state = RUN;
    }

}


void end_case(void){
    P6OUT &= ~R_FORWARD;
    P6OUT &= ~L_FORWARD;
    P6OUT &= ~R_REVERSE;
    P6OUT &= ~L_REVERSE;
    state = WAIT;
    START_RUNNING = FALSE;
}


void pwm_Circle(void){
    if(time_change){
        time_change = 0;
        if(segment_count <= TRAVEL_DISTANCE){
            if(right_motor_count++ >= RIGHT_COUNT_TIME){
                P6OUT &= ~R_FORWARD;
            }
            if(left_motor_count++ >= LEFT_COUNT_TIME){
                P6OUT &= ~L_FORWARD;
            }
            if(cycle_time++ >= WHEEL_COUNT_TIME){
                cycle_time = 0;
                right_motor_count = 0;
                left_motor_count = 0;
                segment_count++;
                Forward_On();
            }
        } else {
            state = END;
        }
    }
}


void run_Triangle(void){


}





void Carlson_StateMachine(void){
    switch(Time_Sequence){
      case 250:                        //
        if(one_time){
          Init_LEDs();
          lcd_BIG_mid();
          display_changed = 1;
          one_time = 0;
        }
        Time_Sequence = 0;             //
        break;
      case 200:                        //
        if(one_time){
//          P1OUT &= ~RED_LED;            // Change State of LED 4
          P6OUT |= GRN_LED;            // Change State of LED 5
          one_time = 0;
        }
        break;
      case 150:                         //
        if(one_time){
          P1OUT |= RED_LED;            // Change State of LED 4
          P6OUT &= ~GRN_LED;            // Change State of LED 5
          one_time = 0;
        }
        break;
      case 100:                         //
        if(one_time){
//          lcd_4line();
          lcd_BIG_bot();
          P6OUT |= GRN_LED;            // Change State of LED 5
          display_changed = 1;
          one_time = 0;
        }
        break;
      case  50:                        //
        if(one_time){
          one_time = 0;
        }
        break;                         //
      default: break;
    }
}






