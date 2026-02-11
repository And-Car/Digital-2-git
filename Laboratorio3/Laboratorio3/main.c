#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "UART.h"
#include "SPI.h"

// Variables para almacenar los valores de los registros SPDR [cite: 577]
volatile uint8_t v1 = 0;
volatile uint8_t v2 = 0;
volatile uint8_t paso = 0;
volatile uint8_t listo = 0;

int main(void) {
	DDRB = 0x00;
	char buffer[64];
	
	DDRC = 0x00;
	
	UART_init(9600);
	SPI_Master_Init();   
	

	SPCR |= (1 << SPIE);
	sei();              

	while(1) {
		listo = 0;
		
		// Seleccionar esclavo (SS a LOW) [cite: 449]
		PORTB &= ~(1 << PORTB2);
		
		// Enviar byte dummy para generar SCK e iniciar intercambio [cite: 602]
		SPDR = 0xFF;
		
		// Esperar a que la ISR complete los dos pasos
		while(listo == 0);
		
		// Convertir el valor de 8 bits a Voltaje
		float volt1 = (v1 * 5.0f) / 255.0f;
		float volt2 = (v2 * 5.0f) / 255.0f;
		

		sprintf(buffer, "V1: %dV | V2: %dV\r\n", volt1, volt2);
		UART_print(buffer);
		
		_delay_ms(500);
	}
}


ISR(SPI_STC_vect) {
	if (paso == 0) {
		v1 = SPDR;      // Recibimos el primer byte (Pot 1) 
		paso = 1;
		SPDR = 0xFF;    // Enviamos otro dummy para recibir Pot 2 [cite: 136]
	}
	else {
		v2 = SPDR;      // Recibimos el segundo byte (Pot 2)
		PORTB |= (1 << SS); // Deseleccionar esclavo (SS a HIGH) [cite: 120]
		listo = 1;      // Avisar al main
		paso = 0;
	}
}