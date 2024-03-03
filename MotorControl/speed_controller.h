// PID controller and speed measure
#ifndef _SPEED_CONTROLLER_H
#define _SPEED_CONTROLLER_H

#ifndef F_CPU
#warning "F_CPU is not defined setting default 16mhz clock <usart.h>"
#define F_CPU 16000000UL
#endif

// #include <xc.h> // include processor files - each processor file is guarded.
#include "helper.h"
#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>

// FUNCTIONS

void setMotorSpeedTarget(int MOTOR, float speed);
void setMotorSpeedTargetPair(float left_motor_speed, float right_motor_speed);
void setMotorSpeed(int MOTOR, float speed);

float getMotorSpeed(int MOTOR);

/**
 * @brief updates the current motor speed
 *
 *calls the compute compute motor speed function then
 * calls the set Motor speed function
 *
 *@param MOTOR The motor identifier (LEFT or RIGHT).
 */
void updateMotorSpeed(int MOTOR);
/**
 * @brief Set the speed of both motors calling the setMotorSpeed function
 *
 *Set the speed of both motors calling the setMotorSpeed function
 *
 */
void updateMotorSpeedPair();

/**
 * @brief Computes control value and assigns to PWM
 *
 * For both motor it computes the u with PID controller
 * then assigns that value to as a duty cycle
 */
void updateMotorSpeed_VIA_PID_Pair();

int compute_control_value(int MOTOR);

// speed measure
float _compute_motor_speed(Motor *motor);
float compute_motor_speed(int MOTOR);
float lowPassFilter(float input, int MOTOR);

float convert_vel_to_rpm(float vel);
#endif
