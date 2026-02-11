#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>

// Prototipos de funciones
void init_ADC(void);
uint16_t ADC_read(uint8_t ch);

#endif /* ADC_H_ */