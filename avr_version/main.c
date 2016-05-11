#define MAIN_C 1

#include "include/constants.h"
#include "include/io_wrappers.h"
#include "include/uart.h"
#include "include/init.h"
#include "include/motor.h"
#include "include/i2c_slave.h"
#include "include/ir_sensor.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>

#include "include/global_vars.h"

// Two new functions, 
// uint16_t read_analog(uint8_t)
// uint16_t convert_to_cm(uint16_t)
// read_analog will capture the analog input from ADC#, valid ports are 0, 1,3-7.  Use port 0, it corresponds with PF0
// convert_to_cm will take the returned value from read_analog an convert it to cm.
// No clue where you want it
// Josh

int main() {
  init();
  sei();

  char buf[64];

  snprintf(buf, 64, "Target: %d\r\n", set_target(0, 50));
  sendString((uint8_t *) buf);

  while (1) {
    int ir_signal = read_analog(0);

    char danger_zone = (ir_signal > 380) || (ir_signal < 150);
    char t1 = i2c_rxbuffer[0];
    char t2 = i2c_rxbuffer[1];
    char moving_forward = (t1 > 0) && (t2 > 0);

    if (danger_zone && moving_forward) {
        snprintf(buf, 64, "Stopped! IR Signal: %d\r\n", ir_signal);
        sendString((uint8_t *) buf);

        set_target(0, 0);
        set_target(1, 0);
    } else {
      snprintf(buf, 64, "Going! IR Signal: %d\r\n", ir_signal);
      sendString((uint8_t *) buf);

      set_target(0, t1);
      set_target(1, t2);
    }

    _delay_ms(50);
  }
}
