#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile uint8_t cuenta = 5;
volatile uint8_t contando =0;

const uint8_t seg7[10] = {
	0b00111111, 
	0b00000110, 
	0b01011011,
	0b01001111, 
	0b01100110, 
	0b01101101  
};


void conteo1seg() {
	TCCR1A = 0;                // Limpiar registro
	TCCR1B = 0;                // Limpiar registro
	TCNT1  = 0;                // Reiniciar contador

	OCR1A = 62499;

	TCCR1B |= (1 << WGM12);
	TCCR1B |= (1 << CS12);
	TIMSK1 |= (1 << OCIE1A);
}

void puertos(){
	DDRD 0xFF
	DDRC & = ~(1 <<DDC6)
	PORTC | = (1 << PORTC6)
	PORTB = seg7[5]
}

ISR(TIMER1_COMPA_vect) {
	if (contando) {
		if (cuenta > 0) {
			cuenta--;
			PORTB = seg7[cuenta];
			} else {
			contando = 0;   
			cuenta = 5;     
		}
	}
}

int main void(){
	puertos();
	conteo1seg();
	sei();
}