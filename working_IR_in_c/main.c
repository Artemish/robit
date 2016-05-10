#define F_CPU 16000000ul
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "uart.h"

#define FALSE 0
#define TRUE 1

char buffer[64];

void init();
uint16_t read_adc(uint8_t);

// Runs off of PF0 (A5 on board)
// left number is raw sensor data, right number is estimated distance in cm.

int main(void)
{
   UDIEN &= ~((1 << EORSTE) | (1 << SUSPE));
   uint16_t adc_value; 
   uint16_t sensor_cm = 0;
   init();
   while(1)
   {
      adc_value = read_adc(0);
      sensor_cm = (6787 / (adc_value - 3)) - 4;
      snprintf(buffer,64,"%" PRIu16 "\t%" PRIu16 "\r\n", adc_value, sensor_cm);
      sendString((uint8_t *)buffer);
      _delay_ms(1000);
      
   }
   return 0;
}

void init(){

   ADMUX = (1<<REFS0);
   ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
   
   setupUART();
}

uint16_t read_adc(uint8_t ch) {
   
   ch=ch&0b00000111;
   ADMUX|=ch;
   ADCSRA|=(1<<ADSC);
   
   while(!(ADCSRA & (1<<ADIF)));
   
   ADCSRA|=(1<<ADIF);
   
   return ADC;
}

