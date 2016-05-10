// This file handles initialization of global variables and hw/peripherals

#include <avr/interrupt.h>
#include <avr/io.h>

#include "include/motor.h"
#include "include/uart.h"
#include "include/pins.h"
#include "include/global_vars.h"
#include "include/io_wrappers.h"
#include "include/constants.h"
#include "include/macros.h"
#include "include/i2c_slave.h"

// Function prototypes
inline void init_vars(void);
inline void init_gpio(void);
inline void init_timers(void);

void pause(void);
void unpause(void);

void init() {
  //USBCON = 0; // resets the USB controller --> shotgun solution to bootloader issue,
  // but all we really need is to clear UDIEN bits 3 and 0 to keep the interrupt from firing
  UDIEN &= ~((1 << EORSTE) | (1 << SUSPE));   // Disable USB "End Of Reset" and "Suspend" interrupts
  write_pin(green_led, HIGH);

  init_vars();
  init_gpio();
  init_timers();
  setupUART();
  sendString((uint8_t *) "Reached the PCINT\r\n");
  init_motor();
  init_i2c();
}

void init_timers(void) {
  // Timer 0, 1KHz CTC @ 64 prescaling
  write_register(&TCCR0B, CS00, 0x1);
  write_register(&TCCR0B, CS01, 0x1);
  write_register(&TCCR0B, CS02, 0x0);

  write_register(&TCCR0A, WGM00, 0x0);
  write_register(&TCCR0A, WGM01, 0x1);
  write_register(&TCCR0B, WGM02, 0x0);
  OCR0A = 249;

  write_register(&TIMSK0, OCIE0A, 0x1);

  // Timer 1, FPWM mode
  write_register(&TCCR1A, WGM10, 0x0);
  write_register(&TCCR1A, WGM11, 0x1);
  write_register(&TCCR1B, WGM12, 0x1);
  write_register(&TCCR1B, WGM13, 0x1);

  // Alternating output signal to the motor
  write_register(&TCCR1A, COM1A1, 0x1);
  write_register(&TCCR1A, COM1B1, 0x1);

  // no prescaling
  write_register(&TCCR1B, CS10, 0x1);

  // With ICR1 as the top value
  ICR1 = 10000;

  // Timer 3, 100Hz CTC @ 64 prescaling, controller task
  write_register(&TCCR3B, CS30, 0x1);
  write_register(&TCCR3B, CS31, 0x1);
  write_register(&TCCR3B, CS32, 0x0);

  write_register(&TCCR3A, WGM30, 0x0);
  write_register(&TCCR3A, WGM31, 0x0);
  write_register(&TCCR3B, WGM32, 0x1);
  OCR3A = 4999;

  write_register(&TIMSK3, OCIE3A, 0x1);
}

inline void init_vars() {
  currently_logging = 1;
  // Reset all timer counts
  TCNT0 = TCNT1 = TCNT3 = 0;
}

inline void init_gpio() {
  pin_mode(encoder_1a, INPUT);
  pin_mode(encoder_1b, INPUT);
  pin_mode(encoder_2a, INPUT);
  pin_mode(encoder_2b, INPUT);

  pin_mode(pin_9, OUTPUT);
  pin_mode(pin_10, OUTPUT);

  // Enable pin change interrupts
  write_register(&PCICR, PCIE0, 0x1);

  // Enable pin change interrupts for encoder pins
  write_register(&PCMSK0, PCINT0, 0x1);
  write_register(&PCMSK0, PCINT1, 0x1);
  write_register(&PCMSK0, PCINT2, 0x1);
  write_register(&PCMSK0, PCINT3, 0x1);
}
