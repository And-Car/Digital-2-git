#ifndef UART_H_
#define UART_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

// Variables externas para que el main pueda ver lo que llega por UART
// 'extern' le dice al compilador que estas variables existen en otro archivo (UART.c)
extern volatile char uart_buffer[25];
extern volatile uint8_t uart_ready;

// Prototipos de funciones
void UART_init(uint32_t baud);
void UART_transmit(char data);
void UART_print(char* str);

#endif /* UART_H_ */