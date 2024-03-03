#ifndef _USART_H
#define _USART_H

#ifndef F_CPU
#warning "F_CPU is not defined setting default 16mhz clock <usart.h>"
#define F_CPU 16000000UL
#endif

// #include <xc.h> // include processor files - each processor file is guarded.
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

// BUADRATE
#ifndef BAUDRATE
#warning "Setting Baudrate to 9600 <usart.h>"
#define BAUDRATE 9600
#endif

#ifndef UBRR
#warning "Using default ubrr <usart.h>"
#define UBRR (((F_CPU / (BAUDRATE * 16UL))) - 1)
#endif

// configurations
// USART MODE
#define ASYNCHRONOUS (0 << UMSEL01) | (0 << UMSEL00) // Set by default
// PARITY MODE
#define PARITY_DISABLED (0 << UPM01) | (0 << UPM00) // Set by default
// STOP BITS
#define ONE_STOP_BIT (0 << USBS0) // Set by defaulf
// Chracter size
#define EIGHT_BIT_FRAME (1 << UCSZ01) | (1 << UCSZ00) // Set by default

#ifndef USART_FRAME
#warning "USART_FRAME not defined <usart.h>"
#define USART_FRAME                                                            \
  ASYNCHRONOUS | PARITY_DISABLED | ONE_STOP_BIT | EIGHT_BIT_FRAME
#endif

// FUNCTIONS
extern void USART_Init(unsigned int ubrr);
extern void USART_Transmit_char(unsigned char data);
extern char USART_Recieve(void);
extern int USART_Available(void);
extern int USART_Transmit(char c, FILE *stream);
extern FILE usart_printf;

#endif
