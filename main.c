
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

#define BAUDRATE 9600
#define F_CPU 16000000L

#include "encoder.h"
#include "usart.h"
#include "motor_controller.h"
#include "command.h"
#include "util.h"


// volatile int number_of_turns_motor0 = 0;
// volatile int number_of_turns_motor1 = 0;

void sendOdometry(int, int);

int main(){}
    // debugger
    USART_Init(UBRR);
    sei();

    while (1) {
        USART_ReadString(); // tries to read from serial
         // Ensure null-termination
        received_string[MAX_STRING_LENGTH - 1] = '\0';

        fprintf(&usart_printf, "%s\n", received_string);

        _delay_ms(3000);
    }
    //return 0;
}

int main(){
    // debugger
    USART_Init(UBRR);
    Encoder_Init();
    Motor_Init();
    sei();
    driveMotorForward0();
    // driveMotorForward1();
    while (1) {
        USART_ReadString(); // tries to read from serial
        fprintf(&usart_printf, received_string)
        /* code */
        //sendOdometry(number_of_turns_motor0, number_of_turns_motor1);
        //_delay_ms(10000);
    }
    //return 0;
}

// Interupts
// ISR(INT0_vect){
//     count_encoder0();
// }

// ISR(INT1_vect){
//     count_encoder1();
// }

// void 
// sendOdometry(int left_turns, int right_turns){
//     char count[20];
//     sprintf(count, "%d:%d", left_turns, right_turns);
//     fprintf(&usart_printf, "%s\n", count);
// }
