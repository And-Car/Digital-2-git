#ifndef LCD1602_H_
#define LCD1602_H_

#ifndef F_CPU
#define F_CPU 16000000UL // Define la velocidad del cristal (16MHz)
#endif

#include <avr/io.h>
#include <util/delay.h>

// Pines de Control asignados para PB2 y PB3
#define E   PB2
#define RS  PB3

// Prototipos de las funciones para tu librería
void LCD_Init(void);
void LCD_Command(unsigned char cmd);
void LCD_Char(unsigned char data);
void LCD_String(char *str);
void LCD_Port_Write(unsigned char byte); // Agregamos este prototipo
void LCD_SetCursor(unsigned char fila, unsigned char columna);

#endif