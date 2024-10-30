/*
 * header.h
 *
 *  Created on: Sep 23, 2024
 *      Author: happy
 */

#ifndef INCLUDE_HEADER_H_
#define INCLUDE_HEADER_H_

#define BLACK (850)
#define BLACK2 (850)

#define WHEEL_OFF (0)
#define SLOW (35000)
#define FAST (50000)
 #define PERCENT_100 (50000)
 #define PERCENT_80  (45000)

#define WHEEL_PERIOD (50000)
#define TB0CCR0_INTERVAL (2500)
#define ONESEC (50000)
#define FIFTY_MS_COUNT (100)
#define PWM_PERIOD (TB3CCR0)
#define TWOH (200)



//pwm.c
#define L (0)
#define R (1)
#define EIGHTY (80)
#define FIFTYSEVEN (57)

#define LCD_BACKLITE_DIMING (TB3CCR1) //PWM
#define RIGHT_FORWARD_SPEED (TB3CCR2)//PWM
#define LEFT_FORWARD_SPEED (TB3CCR3)//PWM
#define RIGHT_REVERSE_SPEED (TB3CCR4)//PWM
#define LEFT_REVERSE_SPEED (TB3CCR5)//PWM


#define WHEEL_OFF (0)
#define SLOW (35000)
#define FAST (50000)
#define PERCENT_100 (50000)
#define PERCENT_80 (45000)


#endif /* INCLUDE_HEADER_H_ */
