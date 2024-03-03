#ifndef _MotorController_H
#define _MotorController_H

#include "constants.h"
#include <avr/io.h>

void Motor_Controller_Init(void);
void PWM_Init();
void initalise_Direction(void);

void driveMotorReverse(int MOTOR);
void driveMotorForward(int MOTOR);

void driveReverse();
void driveForward();

void driveClockWise();
void driveAClockWise();

void stopMotor();
void _stopMotor(int MOTOR);

// Speed control and PWM
void updatePWMValue(int pwm, int MOTOR);
/**
 * @brief Udpates duty cycle of PWM Registers
 *
 * updates the duty cycle to the resgters using
 * current calculated pwm value
 */
void updateMotorPWMDutycycle(int MOTOR);

void updatePWMValuePair(int pwm_left, int pwm_right);

int getPWM(int MOTOR);

void setLinearVelocity(float vel);
void setAngularVelocity(float vel);

#endif
