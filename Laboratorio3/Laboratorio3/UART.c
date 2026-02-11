#include "UART.h"

// Variables volátiles para la recepción (se usan en la interrupción)
volatile char uart_buffer[25];
volatile uint8_t uart_ready = 0;
static uint8_t uart_idx = 0;

void UART_init(uint32_t baud) {
	// Cálculo del valor UBRR para la velocidad deseada
	uint16_t ubrr = (F_CPU/16/baud - 1);
	UBRR0H = (ubrr >> 8);
	UBRR0L = (ubrr & 0xFF);
	
	// Habilitar Transmisión (TX) y Recepción (RX)
	// También habilitamos la interrupción de recepción (RXCIE0)
	UCSR0B = (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0);
	
	// Configurar formato: 8 bits de datos, 1 bit de parada
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void UART_transmit(char data) {
	// Esperar a que el buffer de transmisión esté vacío
	while (!(UCSR0A & (1 << UDRE0)));
	// Poner el dato en el registro para enviarlo
	UDR0 = data;
}

void UART_print(char* str) {
	// Enviar cada caracter de la cadena hasta encontrar el nulo
	while (*str) {
		UART_transmit(*str++);
	}
}

// Rutina de Interrupción de Recepción
ISR(USART_RX_vect) {
	char c = UDR0; // Leer el caracter recibido del registro de datos
	
	// Si detecta un salto de línea, marca el mensaje como listo
	if (c == '\n' || c == '\r') {
		if (uart_idx > 0) { // Solo si hay algo en el buffer
			uart_buffer[uart_idx] = '\0'; // Cerrar la cadena de texto
			uart_ready = 1;
			uart_idx = 0;
		}
		} else {
		// Guardar el caracter si hay espacio en el buffer
		if (uart_idx < 24) {
			uart_buffer[uart_idx++] = c;
		}
	}
}