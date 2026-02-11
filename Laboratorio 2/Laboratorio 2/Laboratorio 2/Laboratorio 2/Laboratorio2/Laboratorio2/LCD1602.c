#include "LCD1602.h"

void LCD_Port_Write(unsigned char byte) {
	// Bits 0-5 a PC0-PC5
	PORTC = (PORTC & 0xC0) | (byte & 0x3F);
	// Bits 6-7 a PB0-PB1
	PORTB = (PORTB & 0xFC) | ((byte >> 6) & 0x03);
}

void LCD_EnablePulse() {
	PORTB |= (1 << E);   // Sube E (PB2)
	_delay_us(1);
	PORTB &= ~(1 << E);  // Baja E (Aquí el LCD lee)
	_delay_us(50);
}

void LCD_Command(unsigned char cmd) {
	LCD_Port_Write(cmd);
	PORTB &= ~(1 << RS); // RS = 0 (PB3)
	LCD_EnablePulse();
	if (cmd == 0x01 || cmd == 0x02) _delay_ms(2);
	else _delay_us(50);
}

void LCD_Char(unsigned char data) {
	LCD_Port_Write(data);
	PORTB |= (1 << RS);  // RS = 1 (PB3)
	LCD_EnablePulse();
	_delay_us(50);
}

void LCD_Init(void) {
	DDRC |= 0x3F; // Salidas Datos
	DDRB |= 0x0F; // Salidas Datos y Control
	_delay_ms(20);
	LCD_Command(0x38); // 8-bit, 2 líneas
	LCD_Command(0x0C); // Display ON
	LCD_Command(0x06); // Incremento
	LCD_Command(0x01); // Limpiar
	_delay_ms(2);
}

void LCD_String(char *str) {
	while(*str) LCD_Char(*str++);
}

void LCD_SetCursor(unsigned char fila, unsigned char columna) {
	unsigned char direccion;

	if (fila == 1) {
		direccion = 0x00 + columna; 
		} else {
		direccion = 0x40 + columna; 
	}


	LCD_Command(0x80 | direccion);
}