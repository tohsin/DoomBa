#include "speed_controller.h"
#include "constants.h"
#include "encoder.h"
#include "helper.h"
#include "motor_controller.h"
#include "usart.h"

int compute_control_value(int MOTOR) {
  Motor *motor = (MOTOR == LEFT) ? &motorLeft : &motorRight;

  float error = motor->target_speed - motor->current_speed;
  // fprintf(&usart_printf, "ERR: %d\n", (int)error);
  //  if (MOTOR == RIGHT) {
  //    fprintf(&usart_printf, "ERR: %d\n", (int)error);
  //  }

  // fprintf(&usart_printf, "Err: %d\n", (int)error);
  //  proprtional term
  float proportional = motor->pid.Kp * error;
  // fprintf(&usart_printf, "P: %d\n", (int)proportional);
  //  integral term
  motor->pid.errorSum += error;
  float integral = motor->pid.errorSum * motor->pid.Ki;
  // fprintf(&usart_printf, "I: %d\n", (int)integral);

  if (integral < MIN_I) {
    integral = MIN_I;
  } else if (integral > MAX_I) {
    integral = MAX_I;
  }

  float derivative = (error - motor->pid.previousError) * motor->pid.Kd;
  motor->pid.previousError = error;

  // Output calculation
  float pid = (proportional + integral + derivative) / K_O;
  // fprintf(&usart_printf, "u: %d\n", (int)pi);
  //
  return (int)pid;
}

float compute_motor_speed(int MOTOR) {
  Motor *motor = (MOTOR == LEFT) ? &motorLeft : &motorRight;

  // float curent_ticks =
  //     (float)motor->number_encoder_ticks - motor->prev_encoder_ticks;
  // float curent_revs =
  //     (float)motor->number_rev_encoder - motor->prev_encoder_ticks;
  // fprintf(&usart_printf, "Number Rev  %d\n",
  //         convertFloatForPrint(num_revolutions));
  float num_revolutions =
      compute_total_rev(motor->number_revs, motor->encoder_ticks) -
      motor->prev_number_rev;
  uint32_t elapsed_time =
      getCurrentTime() - motor->prev_time; // in milisecounds

  if (elapsed_time != 0.0f) {
    float motor_speed_rpm = num_revolutions * 60000.0f / elapsed_time;
    motor->prev_time = getCurrentTime();
    motor->prev_speed = motor_speed_rpm;
    motor->prev_number_rev = num_revolutions;
    // motor->prev_encoder_ticks = motor->number_encoder_ticks;
    return motor_speed_rpm;
  } else {
    return motor->prev_speed;
  }
}

void setMotorSpeedTarget(int MOTOR, float speed) {
  Motor *motor = (MOTOR == LEFT) ? &motorLeft : &motorRight;
  motor->target_speed = speed;
}
void setMotorSpeedTargetPair(float left_motor_speed, float right_motor_speed) {
  setMotorSpeedTarget(LEFT, left_motor_speed);
  setMotorSpeedTarget(RIGHT, right_motor_speed);
}

void updateMotorSpeedPair() {
  updateMotorSpeed(LEFT);
  updateMotorSpeed(RIGHT);
}

void updateMotorSpeed_VIA_PID_Pair() {

  updatePWMValue(compute_control_value(RIGHT), RIGHT);
  updatePWMValue(compute_control_value(LEFT), LEFT);
}

void updateMotorSpeed(int MOTOR) {
  float speed = compute_motor_speed(MOTOR);
  setMotorSpeed(MOTOR, speed);
}

void setMotorSpeed(int MOTOR, float speed) {

  float filtered_speed = lowPassFilter(speed, MOTOR);
  (MOTOR == LEFT) ? (motorLeft.current_speed = filtered_speed)
                  : (motorRight.current_speed = filtered_speed);
}

float getMotorSpeed(int MOTOR) {
  return (MOTOR == LEFT) ? motorLeft.current_speed : motorRight.current_speed;
}

float lowPassFilter(float input, int MOTOR) {
  Motor *motor = (MOTOR == LEFT) ? &motorLeft : &motorRight;
  float filteredValue =
      (1 - FILTRERALPHA) * motor->filteredValue + FILTRERALPHA * input;
  return filteredValue;
}

float convert_vel_to_rpm(float vel) {

  //  linear velocity to rpm
  // 0.065
  //  vel(m/s) =PI*D* rpm / 60
  float rpm = (vel * 60 * 100) / (PI * WHEEL_DIAMETER);
  return rpm;
}
