#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define Binicio PB5
#define BJ1 PC4
#define BJ2 PC5

void actdisplay(uint8_t valor);
void actleds(void);
void puertos(void);

volatile int8_t cuenta = 5;
volatile uint8_t contando = 0;
volatile uint8_t puntaje1 = 0;
volatile uint8_t puntaje2 = 0;
volatile uint8_t finalizado = 0;


const uint8_t seg7[6] = {
	0b01110111, 
	0b01000100, 
	0b01101011, 
	0b01101110, 
	0b01011100, 
	0b00111110  
};

void conteo1seg() {
	TCCR1A = 0; TCCR1B = 0; TCNT1 = 0;
	OCR1A = 62499;
	TCCR1B |= (1 << WGM12) | (1 << CS12);
	TIMSK1 |= (1 << OCIE1A);
}

void puertos() {
	DDRD = 0b11111100; 
	DDRB &= ~(1 << Binicio);
	PORTB |= (1 << Binicio);
	DDRB |= 0b00011111; 
	DDRC &= ~((1 << BJ1) | (1 << BJ2));
	PORTC |= ((1 << BJ1) | (1 << BJ2));
	DDRC |= 0x0F; 
	PCICR |= (1 << PCIE1);
	PCMSK1 |= (1 << PCINT12) | (1 << PCINT13);
}

void actdisplay(uint8_t valor) {
	uint8_t mascara = seg7[valor];
	PORTD = (PORTD & 0b00000011) | (mascara << 2);
	if (mascara & (1 << 6)) PORTB |= (1 << PB0);
	else PORTB &= ~(1 << PB0);
}

void actleds() {
	uint8_t bitsJ1 = 0;
	uint8_t bitsJ2 = 0;

	// JUGADOR 1: Seleccionar solo una posición
	if (puntaje1 == 1)      bitsJ1 = 0b0001;
	else if (puntaje1 == 2) bitsJ1 = 0b0010;
	else if (puntaje1 == 3) bitsJ1 = 0b0100;
	else if (puntaje1 == 4) bitsJ1 = 0b1000;

	// JUGADOR 2: Seleccionar solo una posición
	if (puntaje2 == 1)      bitsJ2 = 0b0001;
	else if (puntaje2 == 2) bitsJ2 = 0b0010;
	else if (puntaje2 == 3) bitsJ2 = 0b0100;
	else if (puntaje2 == 4) bitsJ2 = 0b1000;

	// Aplicar a los puertos respetando la máscara
	PORTC = (PORTC & 0xF0) | bitsJ1;
	PORTB = (PORTB & 0xE1) | (bitsJ2 << 1);
}

ISR(TIMER1_COMPA_vect) {
	if (contando) {
		if (cuenta > 0) {
			cuenta--;
			actdisplay(cuenta);
			} else {
			contando = 0;
		}
	}
}

ISR(PCINT1_vect) {
	if (cuenta == 0 && finalizado == 0) {

		if (!(PINC & (1 << BJ1))) {
			_delay_ms(80); 
			if (!(PINC & (1 << BJ1))) {
				puntaje1++;
				if (puntaje1 >= 4) {
					puntaje1 = 4; puntaje2 = 0;
					finalizado = 1; 
					actdisplay(1);
				}
			}
			while(!(PINC & (1 << BJ1)));
		}

		if (!(PINC & (1 << BJ2))) {
			_delay_ms(30);
			if (!(PINC & (1 << BJ2))) {
				puntaje2++;
				if (puntaje2 >= 4) {
					puntaje2 = 4; puntaje1 = 0;
					finalizado = 1; 
					actdisplay(2);
				}
			}
			while(!(PINC & (1 << BJ2)));
		}
		actleds();
	}
}

int main(void) {
	puertos();
	conteo1seg();
	sei();
	actdisplay(5);
	actleds();

	while (1) {
		if (!(PINB & (1 << Binicio))) {
			_delay_ms(50);
			if (!(PINB & (1 << Binicio))) {
				if (contando == 0 || finalizado == 1) {
					cuenta = 5;
					puntaje1 = 0; puntaje2 = 0;
					finalizado = 0;
					actdisplay(5);
					actleds();
					contando = 1;
				}
				while (!(PINB & (1 << Binicio)));
			}
		}
	}
}