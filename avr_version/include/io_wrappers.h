#pragma once

#include <avr/io.h>
#include "pins.h"

typedef enum {INPUT, OUTPUT} input_mode;
typedef enum {LOW, HIGH, PULLUP, NO_PULLUP, FLIP} signal_level;

void pin_mode(struct pin *pin, input_mode mode);
void write_pin(struct pin *pin, signal_level value);
void write_register(register_t reg, uint8_t position, uint8_t val);

signal_level read_pin(struct pin *pin);
