#include "usart.h"
#include <util/delay.h>

FILE usart_printf = FDEV_SETUP_STREAM(USART_Transmit, NULL, _FDEV_SETUP_WRITE);

void USART_Init(unsigned int ubrr) {
  // Set Baud rate
  UBRR0H = (UBRR >> 8);
  UBRR0L = UBRR;

  // Set USART Configuration | Async - UMSEL0n - 00 |
  UCSR0C = USART_FRAME;

  // Enable TX and RX pins
  UCSR0B = (1 << RXEN0) | (1 << TXEN0);
  // Set USART as standard output
  stdout = &usart_printf;
}

char USART_Recieve(void) {
  // Wait for the Buffer to fill
  loop_until_bit_is_set(UCSR0A, RXC0);

  // Return the char to the stream
  return UDR0;
}

int USART_Available(void) { return (UCSR0A & (1 << RXC0)); }

int USART_Transmit(char c, FILE *stream) {
  if (c == '\n')
    USART_Transmit('\r', stream);
  // Loop until the transmit buffer is available
  loop_until_bit_is_set(UCSR0A, UDRE0);

  // Set 8 bit data to transmit
  UDR0 = c;
  return 0;
}

void USART_Transmit_char(unsigned char data) {
  /* Wait for empty transmit buffer */
  while (!(UCSR0A & (1 << UDRE0)))
    ;
  /* Put data into buffer, sends the data */
  UDR0 = data;
}
