// Potential Constants and Pins configurations
#ifndef _CONSTANTS_H
#define _CONSTANTS_H

#define K_P 3.0
#define K_I 0.5
#define K_D 0.0
#define K_O 1.0

#define PID_UPDATE_FREQ 30 // in HZ
#define MAX_I 170
#define MIN_I 70.0

#define MAXPWM 255
#define MINPWM 110 // motor doesnt move till 100+

#define LEFT 0  // motor doesnt move till 100+
#define RIGHT 1 // motor doesnt move till 100+

// PINS AND PORTS
#define ENCODOER_DIRECTION_PIN0 PC5
#define ENCODOER_DIRECTION_PIN1 PC4

// sensor filter
#define FILTRERALPHA 0.5

// wheel and velcocity parameters
#define WHEEL_DIAMETER 6.5 // in cm
#define PI 3.142
#define TICK_PER_REV 512
#define TRUE_ 1
#define FALSE_ 1

#define MAX_STRING_LENGTH 100

#endif
