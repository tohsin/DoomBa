#ifndef _ENCODER_H
#define _ENCODER_H

#ifndef F_CPU
#warning "F_CPU is not defined setting default 16mhz clock <usart.h>"
#define F_CPU 16000000UL
#endif

// #include <xc.h> // include processor files - each processor file is guarded.
#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>

// functions
extern int is_direction_High(int MOTOR);
extern void Encoder_Init(void);
extern void count_encoder(int MOTOR);
int getEncoderTicks(int MOTOR);
void reset_encoders(void);

// variables
extern volatile int number_encoder0_ticks;

void encoder_count_to_rev(int MOTOR);
void encoder_count_to_rev_PAIR(void);
/**
 * @brief Combines number rev and number ticks to a single variable
 *
 * computes number_rev + (number_ticks / ticks_per_rev)
 * @param number_rev
 * @param number_ticks
 */
float compute_total_rev(long number_rev, long number_ticks);

float get_number_rev(int MOTOR);
// left
#endif
