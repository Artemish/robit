#include "include/ir_sensor.h"

uint16_t read_analog(uint8_t ch) {
   
   ch=ch&0b00000111;
   ADMUX|=ch;
   ADCSRA|=(1<<ADSC);
   
   while(!(ADCSRA & (1<<ADIF)));
   
   ADCSRA|=(1<<ADIF);
   
   return ADC;
}

uint16_t convert_to_cm(uint16_t signal){
   
   return ((6787 / (signal - 3)) - 4)
}