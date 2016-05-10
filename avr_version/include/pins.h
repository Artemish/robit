#ifndef PINS_H
#define PINS_H

#include <avr/io.h>

typedef volatile uint8_t * register_t;

struct pin {
  uint8_t pin_number;
  uint8_t pin_control_number;
  register_t pin_reading;
  register_t pin_bank;
  register_t pin_control;
};

struct pin *pin_0;
struct pin *pin_1;
struct pin *pin_2;
struct pin *pin_3;
struct pin *pin_4;

struct pin *pin_8;
struct pin *pin_9;
struct pin *pin_10;
struct pin *pin_12;

struct pin *encoder_1a;
struct pin *encoder_1b;
struct pin *encoder_2a;
struct pin *encoder_2b;

struct pin *red_led;
struct pin *green_led;
struct pin *yellow_led;

struct pin *button_a;
struct pin *button_b;
struct pin *button_c;

#endif
