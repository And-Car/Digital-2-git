#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h> 
#include "ADC.h"
#include "LCD1602.h"
#include "UART.h"
#include <avr/interrupt.h>

int main(void) {
	char buffer[16];   //Buffer para imprimir
	char voltajee[8];  // Buffer para hacer de float a string
	uint16_t adcvalor1;

	
	char buffer2[16];   
	char voltajee2[8];
	
	
	char buffcontador[8];  //Buffer para imprimir el contador
	uint16_t adcvalor2;
	
	float voltaje1;
	float voltaje2;
	
	int8_t contador = 0;

	init_ADC();
	LCD_Init();
	UART_init(9600);
	sei();

	while (1) {

		adcvalor1 = ADC_read(6);
		adcvalor2 = ADC_read(7);
		
		voltaje1 = ((float)adcvalor1 * 5.0f / 1023.0f);
		
		voltaje2 = ((float)adcvalor2*5.0f/1023.0f);
		
		
		// Convertimos a string las mediciones
		dtostrf(voltaje1, 4, 2, voltajee);
		
		dtostrf(voltaje2, 4, 2, voltajee2);

		// Metemos al buffer las mediciones
		sprintf(buffer, "V1:%sV ", voltajee);
		sprintf(buffer2, "V2:%sV", voltajee2);
		sprintf(buffcontador, "Cont:%d   ", contador);

		// Actualizamos el LCD
		LCD_SetCursor(1, 0);
		LCD_String(buffer);
		
		LCD_SetCursor(2, 0);
		LCD_String(buffer2);
		

		LCD_SetCursor(1, 9);
		LCD_String(buffcontador);
		
		LCD_SetCursor(2, 9);
		LCD_String("Cardona");
		
		
		//Mandamos al UART los valores de voltaje
		UART_print("V1: ");
		UART_print(buffer);
		UART_print(" | ");
		
		UART_print("V2: ");
		UART_print(buffer2);
		UART_print("\r\n");
		
		
		//Logica del contador, al activarse la bandera de interrupcion, hace la suma resta o nada
		if(uart_ready){
			if(uart_buffer[0] == '+'){
				contador ++;
			}
			
			if(uart_buffer[0] == '-'){
				contador --;
			}
			
			else{
				
			}
		uart_ready = 0; //Limpiamos la bandera de interrupcion
		}

		_delay_ms(100);
		

	}
	return 0;
}