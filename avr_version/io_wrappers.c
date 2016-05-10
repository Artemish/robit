#include "include/io_wrappers.h"
#include "include/pins.h"

void pin_mode(struct pin *pin, input_mode mode) {
   if (mode == INPUT) {
     *(pin->pin_control) &= ~(1 << (pin->pin_control_number));
   } else if (mode == OUTPUT) {
     *(pin->pin_control) |= (1 << (pin->pin_control_number));
   }
}

void write_register(register_t reg, uint8_t position, uint8_t val) {
   if (val == 0xff) {
     *reg ^= (1 << position);
   } else if (val) {
     *reg |= (1 << position);
   } else {
     *reg &= ~(1 << position);
   }
}

void write_pin(struct pin *pin, signal_level value) {
   if (value == HIGH) {
     *(pin->pin_bank) |= (1 << pin->pin_number);
   } else if (value == LOW) {
     *(pin->pin_bank) &= ~(1 << pin->pin_number);
   } else if (value == PULLUP) {
     *(pin->pin_bank) |= (1 << pin->pin_number);
   } else if (value == NO_PULLUP) {
     *(pin->pin_bank) &= ~(1 << pin->pin_number);
   } else if (value == FLIP) {
     *(pin->pin_bank) ^= (1 << pin->pin_number);
   }
}

signal_level read_pin(struct pin *pin) {
  uint8_t reading = *(pin->pin_reading) & (1 << pin->pin_number);
  if (reading != 0) {
    return HIGH;
  } else {
    return LOW;
  }
}
