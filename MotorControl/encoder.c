
#include "encoder.h"
#include "constants.h"
#include "helper.h"
#include "usart.h"

void Encoder_Init(void) {

  EICRA |= (1 << ISC00) | (1 << ISC01) | (1 << ISC10) | (1 << ISC11);
  EIMSK |= (1 << INT0) | (1 << INT1);

  DDRC &= ~(1 << PC5); // Enocder direction  as input
  DDRC &= ~(1 << PC4);
}

// check direction of the encoder
int is_direction_High(int MOTOR) {
  return (MOTOR == LEFT) ? ((PINC & (1 << PC5)) ? 1 : 0)
                         : ((PINC & (1 << PC4)) ? 1 : 0);
}

// To be called in interrupts to count
void count_encoder(int MOTOR) {
  Motor *motor = getMotorPointer(MOTOR);
  motor->number_encoder_ticks += is_direction_High(MOTOR) ? 1 : -1;
}

void reset_encoders() {
  // number_of_turns_motor1 = 0;
  motorRight.encoder_ticks = 0;
  motorLeft.encoder_ticks = 0;
}

int getEncoderTicks(int MOTOR) {
  Motor *motor = getMotorPointer(MOTOR);
  return motor->encoder_ticks;
}

void encoder_count_to_rev(int MOTOR) {
  Motor *motor = getMotorPointer(MOTOR);
  motor->number_revs += ((float)motor->encoder_ticks / TICK_PER_REV);
  motor->encoder_ticks = 0;
}

void encoder_count_to_rev_PAIR(void) {
  encoder_count_to_rev(LEFT);
  encoder_count_to_rev(RIGHT);
}

float compute_total_rev(long number_rev, long number_ticks) {
  return number_rev + ((float)number_ticks / TICK_PER_REV);
}
float get_number_rev(int MOTOR) { // odometry number of revolution
  Motor *motor = getMotorPointer(MOTOR);
  return compute_total_rev(motor->number_revs, motor->encoder_ticks);
}
