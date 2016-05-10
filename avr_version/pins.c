#include "include/pins.h"

struct pin my_red_led = {
  .pin_number = PORTB0,
  .pin_control_number = DDB0,
  .pin_reading = &PINB,
  .pin_bank = &PORTB,
  .pin_control = &DDRB,
};

struct pin my_yellow_led = {
  .pin_number = PORTC7,
  .pin_control_number = DDC7,
  .pin_reading = &PINC,
  .pin_bank = &PORTC,
  .pin_control = &DDRC,
};

struct pin my_green_led = {
  .pin_number = PORTD5,
  .pin_control_number = DDD5,
  .pin_reading = &PIND,
  .pin_bank = &PORTD,
  .pin_control = &DDRD,
};

struct pin my_button_a = {
  .pin_number = PORTB3,
  .pin_control_number = DDB3,
  .pin_reading = &PINB,
  .pin_bank = &PORTB,
  .pin_control = &DDRB,
};

struct pin my_pin_0 = {
  .pin_number = PORTD2,
  .pin_control_number = DDD2,
  .pin_reading = &PIND,
  .pin_bank = &PORTD,
  .pin_control = &DDRD,
};

struct pin my_pin_1 = {
  .pin_number = PORTD3,
  .pin_control_number = DDD3,
  .pin_reading = &PIND,
  .pin_bank = &PORTD,
  .pin_control = &DDRD,
};

struct pin my_pin_2 = {
  .pin_number = PORTD1,
  .pin_control_number = DDD1,
  .pin_reading = &PIND,
  .pin_bank = &PORTD,
  .pin_control = &DDRD,
};

struct pin my_pin_3 = {
  .pin_number = PORTD0,
  .pin_control_number = DDD0,
  .pin_reading = &PIND,
  .pin_bank = &PORTD,
  .pin_control = &DDRD,
};

struct pin my_pin_4 = {
  .pin_number = PORTD4,
  .pin_control_number = DDD4,
  .pin_reading = &PIND,
  .pin_bank = &PORTD,
  .pin_control = &DDRD,
};

struct pin my_pin_8 = {
  .pin_number = PORTB4,
  .pin_control_number = DDB4,
  .pin_reading = &PINB,
  .pin_bank = &PORTB,
  .pin_control = &DDRB,
};

struct pin my_pin_9 = {
  .pin_number = PORTB5,
  .pin_control_number = DDB5,
  .pin_reading = &PINB,
  .pin_bank = &PORTB,
  .pin_control = &DDRB,
};

struct pin my_pin_10 = {
  .pin_number = PORTB6,
  .pin_control_number = DDB6,
  .pin_reading = &PINB,
  .pin_bank = &PORTB,
  .pin_control = &DDRB,
};

struct pin my_pin_12 = {
  .pin_number = PORTD6,
  .pin_control_number = DDD6,
  .pin_reading = &PIND,
  .pin_bank = &PORTD,
  .pin_control = &DDRD,
};

struct pin my_pin_14 = {
  .pin_number = PORTB3,
  .pin_control_number = DDB3,
  .pin_reading = &PINB,
  .pin_bank = &PORTB,
  .pin_control = &DDRB,
};

struct pin my_pin_15 = {
  .pin_number = PORTB1,
  .pin_control_number = DDB1,
  .pin_reading = &PINB,
  .pin_bank = &PORTB,
  .pin_control = &DDRB,
};

struct pin my_pin_16 = {
  .pin_number = PORTB2,
  .pin_control_number = DDB2,
  .pin_reading = &PINB,
  .pin_bank = &PORTB,
  .pin_control = &DDRB,
};

struct pin my_pin_17 = {
  .pin_number = PORTB0,
  .pin_control_number = DDB0,
  .pin_reading = &PINB,
  .pin_bank = &PORTB,
  .pin_control = &DDRB,
};

struct pin *pin_0 = &my_pin_0;
struct pin *pin_1 = &my_pin_1;
struct pin *pin_2 = &my_pin_2;
struct pin *pin_4 = &my_pin_4;

struct pin *pin_8 = &my_pin_8;
struct pin *pin_9 = &my_pin_9;
struct pin *pin_10 = &my_pin_10;
struct pin *pin_12 = &my_pin_12;

struct pin *encoder_1a = &my_pin_14;
struct pin *encoder_1b = &my_pin_15;
struct pin *encoder_2a = &my_pin_16;
struct pin *encoder_2b = &my_pin_17;

struct pin *red_led = &my_red_led;
struct pin *green_led = &my_green_led;
struct pin *yellow_led = &my_yellow_led;

struct pin *button_a = &my_button_a;
struct pin *button_b = &my_green_led;
struct pin *button_c = &my_red_led;
