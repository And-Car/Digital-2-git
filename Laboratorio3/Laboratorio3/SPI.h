#ifndef SPI_H_
#define SPI_H_

#include <avr/io.h>

// Definición de pines para ATmega328P (Puerto B) [cite: 244, 245, 246]
#define MOSI PORTB3
#define MISO PORTB4
#define SCK  PORTB5
#define SS   PORTB2

// Funciones principales
void SPI_Master_Init(void); // Configura como Maestro [cite: 377, 378]
void SPI_Slave_Init(void);  // Configura como Esclavo [cite: 377, 378]
uint8_t SPI_Transfer(uint8_t data); // Envía y recibe simultáneamente [cite: 60, 154]

#endif