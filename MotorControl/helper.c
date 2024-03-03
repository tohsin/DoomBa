#include "helper.h"
#include "constants.h"
#include "encoder.h"
#include "motor_controller.h"
#include "usart.h"

int string_index = 0;
char received_string[MAX_STRING_LENGTH];
volatile uint32_t millis = 0;

Motor motorRight; // 1
Motor motorLeft;  // 1

void PID_Init(PID_t *pid) {
  pid->Kp = K_P;
  pid->Ki = K_I;
  pid->Kd = K_D;
  pid->errorSum = 0.0;
  pid->previousError = 0.0;
}

void Motor_Init_(Motor *motor, int id) {
  PID_Init(&motor->pid);
  motor->id = id;
  motor->number_encoder_ticks = 0;
  motor->prev_speed = 0;
  motor->prev_encoder_ticks = 0;
  motor->prev_time = 0;
  motor->pwm = 0;
  motor->number_encoder_ticks = 0;
  motor->target_speed = 0;
  motor->current_speed = 0;
}

void Motor_Init() {
  Motor_Init_(&motorLeft, LEFT);
  Motor_Init_(&motorRight, RIGHT);
}

int PWM_bound(int value) {
  if (value > MAXPWM) {
    return MAXPWM;
  } else if (value < MINPWM) {
    return MINPWM;
  } else {
    return value;
  }
}
void USART_ReadString(void) {
  char c;

  while ((c = USART_Recieve()) != '\n' &&
         string_index < MAX_STRING_LENGTH - 1) {
    received_string[string_index++] = c;
  }

  received_string[string_index] = '\0';

  string_index = 0; // reset buffer idx
}

void sendOdometry() {
  float number_rev_left = get_number_rev(LEFT);
  float number_rev_right = get_number_rev(RIGHT);
  long l_number_rev_left = 100 * number_rev_left;
  long l_number_rev_right = 100 * number_rev_right;
  fprintf(&usart_printf, "<%ld:%ld>\n", l_number_rev_left, l_number_rev_right);
}

// clock stuff
void initSystemTimer() {
  TCCR2A = (1 << WGM21); // ctc mode
  OCR2A = 250;           // 1 millisecs
  TIMSK2 = (1 << OCIE0A);
  TCCR2B |= (1 << CS22); // 64 pre sclar to get 1 milli sec
}
void updateSystemTime() { millis++; }

uint32_t getCurrentTime() { return millis; }

void resetSystemClock() { millis = 0; }

int run_HZ(uint32_t current_time, uint32_t interval) {
  return ((getCurrentTime() - current_time) >= interval) ? 1 : 0;
}
// printing functions for floats
int convertFloatForPrint(float value) { return (int)value * 10; }

uint32_t convertFloatForPrintPositive(float value) {
  return (uint32_t)value * 100;
}
float abs(float value) {
  if (value < 0.0) {
    return -1.0 * value;
  } else {
    return value;
  }
}

Motor *getMotorPointer(int MOTOR) {
  return (MOTOR == LEFT) ? &motorLeft : &motorRight;
}

float hz_to_period(float frequency) { return 1000 / frequency }
