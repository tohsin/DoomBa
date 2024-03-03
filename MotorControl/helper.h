#ifndef _HELPER_H
#define _HELPER_H

#include <avr/io.h>
#include <stdint.h>

void USART_ReadString(void);
void sendOdometry();
void initSystemTimer(void);
void updateSystemTime(void);
/**
 * @brief To run code at certain frquencies
 *
 * Checks system clocks and compare with current time if matches interval
 *
 */
int run_HZ(uint32_t current_time, uint32_t interval);
uint32_t getCurrentTime(void);
void resetSystemClock(void);
int convertFloatForPrint(float);
uint32_t convertFloatForPrintPositive(float);
extern int string_index;
extern char received_string[MAX_STRING_LENGTH];
float abs(float value);

// clock stuff
extern volatile uint32_t millis;

// motor stuff
typedef struct {
  float Kp; // Proportional gain
  float Ki; // Integral gain
  float Kd; // Derivative gain

  float errorSum;      // Integral term accumulator
  float previousError; // Previous error for derivative term
} PID_t;

typedef struct {
  int id;
  PID_t pid;
  float prev_speed;

  uint32_t prev_time;
  int pwm;
  volatile long encoder_ticks;
  long prev_encoder_ticks;

  float number_revs;
  float prev_number_rev;

  float current_speed;
  float target_speed;
  float filteredValue;
} Motor;

extern Motor motorLeft;  // 0
extern Motor motorRight; // 1

void Motor_Init_(Motor *, int);
void Motor_Init();

void PID_Init(PID_t *);

int PWM_bound(int);
Motor *getMotorPointer(int MOTOR);

float hz_to_period(float frequency);

#endif
