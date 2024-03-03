#include "motor_controller.h"
#include "constants.h"
#include "helper.h"
#include "speed_controller.h"

void initalise_Direction() {

  // left motor
  DDRC |= (1 << PC0);
  DDRC |= (1 << PC1);

  // right motor
  DDRC |= (1 << PC2);
  DDRC |= (1 << PC3);
}
void Motor_Controller_Init() {
  Motor_Init();
  initalise_Direction();
  PWM_Init();
  stopMotor();
}

void _stopMotor(int MOTOR) {
  if (MOTOR == LEFT) {
    PORTC &= ~(1 << PC0);
    PORTC &= ~(1 << PC1);
  } else {
    PORTC &= ~(1 << PC2);
    PORTC &= ~(1 << PC3);
  }
}

void stopMotor() {
  _stopMotor(LEFT);
  _stopMotor(RIGHT);
  // stopMotor1();
}

// motor O direction control
void driveMotorReverse(int MOTOR) {
  if (MOTOR == LEFT) {
    PORTC |= (1 << PC0);
    PORTC &= ~(1 << PC1);
  } else {
    PORTC |= (1 << PC2);
    PORTC &= ~(1 << PC3);
  }
}

void driveMotorForward(int MOTOR) {
  if (MOTOR == LEFT) {
    PORTC |= (1 << PC1);
    PORTC &= ~(1 << PC0);
  } else {
    PORTC |= (1 << PC2);
    PORTC &= ~(1 << PC3);
  }
}

void driveReverse() {
  driveMotorReverse(LEFT);
  driveMotorReverse(RIGHT);
}
void driveForward() {
  driveMotorForward(LEFT);
  driveMotorForward(RIGHT);
}

void driveAClockWise() {
  driveMotorForward(LEFT);
  driveMotorReverse(RIGHT);
}

void driveClockWise() {
  driveMotorForward(RIGHT);
  driveMotorReverse(LEFT);
}
// PWM control conde
void PWM_Init() {
  DDRD |= (1 << PD6);
  DDRD |= (1 << PD5);

  TCCR0A |= (1 << COM0A1) | (1 << COM0B1) | (1 << WGM01) | (1 << WGM00);
  TIMSK0 |= (1 << TOIE0);
  OCR0A = motorLeft.pwm;
  OCR0B = motorRight.pwm;
  TCCR0B |= (1 << CS00); // also starts clock
}

void updatePWMValue(int pwm, int MOTOR) {
  (MOTOR == LEFT) ? (motorLeft.pwm = PWM_bound(pwm))
                  : (motorRight.pwm = PWM_bound(pwm));
}
void updatePWMValuePair(int pwm_left, int pwm_right) {
  updatePWMValue(pwm_left, LEFT);
  updatePWMValue(pwm_right, RIGHT);
}

void updateMotorPWMDutycycle(int MOTOR) {
  // to be called in interrupt
  (MOTOR == LEFT) ? (OCR0A = motorLeft.pwm) : (OCR0B = motorRight.pwm);
}

int getPWM(int MOTOR) {
  return (MOTOR == LEFT) ? motorLeft.pwm : motorRight.pwm;
}

void setLinearVelocity(float vel) {
  if (vel == 0) {
    stopMotor();
    return;
  } else if (vel > 0) {
    driveForward();
  } else {
    vel = -1.0 * vel; // make vel postive
    driveReverse();
  }
  float rpm = convert_vel_to_rpm(vel);
  setMotorSpeedTargetPair(rpm, rpm);
}

void setAngularVelocity(float vel) {

  if (vel == 0) {
    stopMotor();
    return;
  } else if (vel > 0) {
    driveClockWise();
  } else {
    driveAClockWise();
  }
  float rpm = convert_vel_to_rpm(vel);
  setMotorSpeedTargetPair(rpm, rpm);
}
