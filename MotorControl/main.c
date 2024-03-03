#include "constants.h"
#define BAUDRATE 9600
#define F_CPU 16000000L

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h> // For atoi function
#include <util/delay.h>

#include "command.h"
#include "encoder.h"
#include "helper.h"
#include "motor_controller.h"
#include "speed_controller.h"
#include "usart.h"

char args0[20]; // requires access outside function
char args1[20];
void parse_command();

void handleCommands(void);
void Init_(void);

int main() {
  Init_(); // call my initalisers
  sei();
  uint32_t current_time = 0;
  uint32_t current_time_pid = 0;
  uint32_t prev_command_time = 0;

  while (1) {
    if (USART_Available() == TRUE_) {
      prev_command_time = getCurrentTime();
      handleCommands();
    }

    if (run_HZ(current_time_pid, hz_to_period(1)) ==
        TRUE_) { // broad cast odomtery then wipe

      sendOdometry();
      // reset_encoders();
    }

    if (run_HZ(current_time_pid, hz_to_period(PID_UPDATE_FREQ)) ==
        TRUE_) { // run pid controller every 3sec
      encoder_count_to_rev_PAIR();
      updateMotorSpeedPair(); // also uses time window to update

      fprintf(&usart_printf, "RIGHT: %d\n", (int)getMotorSpeed(RIGHT));
      fprintf(&usart_printf, " ");
      current_time_pid = getCurrentTime();
      updateMotorSpeed_VIA_PID_Pair();
    }

    if (run_HZ(prev_command_time, 1000)) { // broad cast odo

      stopMotor();
      reset_encoders();

      // stop everything set motor speed to zero
    }
  }
}

void handleCommands() {

  USART_ReadString();                // tries to read from serial
  char command = received_string[0]; // first chracter is command
  setLinearVelocity(0.1);            // reverse at 0.1m/s
  switch (command) {
    // open loop pwm control
  case SET_PWM0: // command p
  {
    parse_command();
    updatePWMValuePair(atoi(args0), atoi(args1));
    break;
  }
  case SET_SPEED: // to receive motor speed s<Linear speed:angular speed>
  {
    parse_command();
    float linear_vel = atoi(args0);
    if (linear_vel != 0) {
      setLinearVelocity(linear_vel);
    } else {
      setAngularVelocity(atoi(args1));
    }

    break;
  }

  default:
    fprintf(&usart_printf, "Unknown command: %c\n", command);
    break;
  }
}

void parse_command() {

  int args0_pntr = 0;
  int args1_pntr = 0;
  // break after receiving > to end command
  int argidx = 0;
  int command_length = MAX_STRING_LENGTH;
  int idx = 2; // after command char[0] followed by <[1] reading should start
               // from idx 2
  while (idx < command_length && received_string[idx] != '>') {
    if (argidx == 0) {
      if (received_string[idx] != ':') {
        args0[args0_pntr] = received_string[idx];
        idx++;
        args0_pntr++;
      } else { // move to the secoud argument
        argidx = 1;
      }

    } else {
      args1[args1_pntr] = received_string[idx];
      args1_pntr++;
    }
    idx++;
  }
  args0[args0_pntr] = '\0';
  args1[args1_pntr] = '\0';
}
void Init_() {
  // debugger
  USART_Init(UBRR);
  Motor_Controller_Init();
  Encoder_Init();
  initSystemTimer();
}

// System clock interupts and PWM
ISR(TIMER0_OVF_vect) { // dynamically updates PWM
  setMotorPWM(LEFT);
  setMotorPWM(RIGHT);
}
ISR(INT0_vect) { count_encoder(LEFT); }

ISR(INT1_vect) { count_encoder(RIGHT); }

ISR(TIMER2_COMPA_vect) { updateSystemTime(); }
