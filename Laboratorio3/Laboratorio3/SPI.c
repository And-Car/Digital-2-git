#include "SPI.h"

void SPI_Master_Init(void) {
	// 1. Configurar MOSI, SCK y SS como salidas [cite: 243]
	DDRB |= (1 << MOSI) | (1 << SCK) | (1 << SS);
	// MISO se configura automáticamente como entrada al activar SPI [cite: 243]

	// 2. Configurar SPCR [cite: 367, 368]
	// SPE = 1 (Habilita SPI) [cite: 371]
	// MSTR = 1 (Modo Maestro) [cite: 377]
	// SPR0 = 1 (Velocidad fosc/16, por ejemplo) [cite: 523]
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0) | (1<<SPIE) | (0<< CPOL);
}

void SPI_Slave_Init(void) {
	// 1. Configurar MISO como salida [cite: 243]
	DDRB |= (1 << MISO);
	// MOSI, SCK y SS son entradas por defecto [cite: 243]

	// 2. Configurar SPCR [cite: 367, 368]
	// SPE = 1 (Habilita SPI) [cite: 371]
	// MSTR = 0 (Modo Esclavo por defecto al estar en 0) [cite: 378]
	SPCR = (1 << SPE);
}

uint8_t SPI_Transfer(uint8_t data) {
	// 1. Cargar el dato al registro SPDR para iniciar transmisión [cite: 577, 601]
	SPDR = data;

	// 2. Esperar a que el hardware termine (Chequear bandera SPIF) [cite: 527, 611]
	// El bit SPIF en SPSR se pone en 1 cuando termina el intercambio [cite: 611]
	while(!(SPSR & (1 << SPIF)));

	// 3. Retornar el dato recibido que ahora está en SPDR [cite: 577, 615]
	return SPDR;
}