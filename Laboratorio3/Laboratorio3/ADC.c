#include "ADC.h"

// Inicializa el ADC con voltaje de referencia en AVCC y prescaler de 128
void init_ADC(void) {
	// Referencia de voltaje en AVCC (5V)
	ADMUX = (1 << REFS0);
	
	// Habilitar ADC y configurar el prescaler a 128 (16MHz/128 = 125kHz)
	// Esto asegura una conversión precisa dentro del rango de 50kHz a 200kHz.
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

// Realiza la lectura de un canal específico (0-7)
uint16_t ADC_read(uint8_t ch) {
	// Selecciona el canal manteniendo la configuración de referencia (limpia bits 0-3)
	ADMUX = (ADMUX & 0xF0) | (ch & 0x07);
	
	// Inicia la conversión
	ADCSRA |= (1 << ADSC);
	
	// Espera a que la conversión termine (ADSC vuelve a 0)
	while (ADCSRA & (1 << ADSC));
	
	// Retorna el valor de 10 bits
	return ADC;
}