#define MAIN_C 1

#include "include/constants.h"
#include "include/io_wrappers.h"
#include "include/uart.h"
#include "include/init.h"
#include "include/motor.h"
#include "include/i2c_slave.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>

#include "include/global_vars.h"

int main() {
  init();
  sei();

  char buf[64];

  snprintf(buf, 64, "Target: %d\r\n", set_target(0, 50));
  sendString((uint8_t *) buf);

  // snprintf(buf, 64, "First: [%d,%d,%d,%d,%d]\r\n", i2c_rxbuffer[0], i2c_rxbuffer[1], i2c_rxbuffer[2], i2c_rxbuffer[3], i2c_rxbuffer[4]);

  while (1) {
    snprintf(buf, 64, "Drive signal: [%d,%d]\r\n", OCR1A, OCR1B);
    sendString((uint8_t *) buf);
    set_target(0, i2c_rxbuffer[0]);
    set_target(1, i2c_rxbuffer[1]);
    _delay_ms(200);
  }
}
