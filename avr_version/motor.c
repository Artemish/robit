#include "include/pins.h"
#include "include/macros.h"
#include "include/constants.h"
#include "include/io_wrappers.h"
#include "include/uart.h"
#include "include/i2c_slave.h"

#include <stdlib.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

#include "include/global_vars.h"

static char PE1_FORWARD = 0;
static char PE1_BACKWARD = 0;
static char PE2_FORWARD = 0;
static char PE2_BACKWARD = 0;

static char global_m1a;
static char global_m1b;
static char global_last_m1a_val;
static char global_last_m1b_val;

static char global_m2a;
static char global_m2b;
static char global_last_m2a_val;
static char global_last_m2b_val;

static int16_t counts_m1;
static int16_t counts_m2;
static int16_t last_counts_m1;
static int16_t last_counts_m2;
static int16_t last_speed_m1;
static int16_t last_speed_m2;
static int16_t last_error_m1;
static int16_t last_error_m2;
static int16_t integral_error_m1;
static int16_t integral_error_m2;

static int16_t m1_p_gain;
static int16_t m1_i_gain;
static int16_t m1_d_gain;

static int16_t m2_p_gain;
static int16_t m2_i_gain;
static int16_t m2_d_gain;

static int16_t target_m1;
static int16_t target_m2;

void init_motor();

void set_duty_cycle(int16_t ten_thousandths, uint8_t motor);
void adjust_duty_cycle(int16_t ten_thousandths, uint8_t motor);

void find_direction(void);

int is_busy(void);

int get_target(uint8_t motor);
int get_counts(uint8_t motor);
int16_t set_target(uint8_t motor, int8_t target);

void stop();

void stop() {
  set_target(0, 0);
  set_target(1, 0);
}

void init_motor() {
  m1_p_gain = 30;
  m1_i_gain = 0;
  m1_d_gain = 100;

  m2_p_gain = 50;
  m2_i_gain = 1;
  m2_d_gain = 100;

  global_m1a = 0;
  global_m1b = 0;
  global_m2a = 0;
  global_m2b = 0;

  // initialize the global state
  target_m1 = 0;
  target_m2 = 0;

  counts_m1 = 0;
  counts_m2 = 0;

  last_counts_m1 = 0;
  last_counts_m2 = 0;

  last_speed_m1 = 0;
  last_speed_m2 = 0;

  last_error_m1 = 0;
  last_error_m2 = 0;

  integral_error_m1 = 0;
  integral_error_m2 = 0;

  global_last_m1a_val = read_pin(encoder_1a);
  global_last_m1b_val = read_pin(encoder_1b);
  global_last_m2a_val = read_pin(encoder_2a);
  global_last_m2b_val = read_pin(encoder_2b);

  find_direction();
}

void find_direction() {
  set_duty_cycle(0, 0);
  set_duty_cycle(0, 1);

  int c1_1 = get_counts(0);
  int c2_1 = get_counts(1);

  set_duty_cycle(5000,0);
  set_duty_cycle(5000,1);
  _delay_ms(50);

  int c1_2 = get_counts(0);
  int c2_2 = get_counts(1);

  if (c1_1 >= c1_2) {
    PE1_FORWARD = 1;
    PE1_BACKWARD = 0;
  } else {
    PE1_FORWARD = 0;
    PE1_BACKWARD = 1;
  }

  // Motor 2 is flipped
  if (c2_1 >= c2_2) {
    PE2_FORWARD = 0;
    PE2_BACKWARD = 1;
  } else {
    PE2_FORWARD = 1;
    PE2_BACKWARD = 0;
  }

  set_duty_cycle(0,0);
  set_duty_cycle(0,1);
}

void go_forwards(int motor) {
  if (motor == 0) {
    write_register(&PORTD, DDD6, PE1_FORWARD);
  } else {
    write_register(&PORTE, DDE2, PE2_FORWARD);
  }
}

void go_backwards(int motor) {
  if (motor == 0) {
    write_register(&PORTD, DDD6, PE1_BACKWARD);
  } else {
    write_register(&PORTE, DDE2, PE2_BACKWARD);
  }
}

char get_direction(int motor) {
  if (motor == 0) {
    return (PORTD & (1 << DDD6)) >> DDD6;
  } else {
    return (PORTE & (1 << DDE2)) >> DDE2;
  }
}

void set_duty_cycle(int16_t ten_thousandths, uint8_t motor) {
  uint16_t target;
  // Can't get too crazy
  // OCR1A is 10000 = 100 * 100
  if (ten_thousandths < 100) {
    target = 0;
  } else if (ten_thousandths < 1000) {
    // The motor shouldn't be run at much under this I think
    target = 1000;
  } else if (ten_thousandths > 7500) {
    target = 7500;
  } else {
    target = (uint16_t) ten_thousandths;
  }

  if (motor == 0) {
    OCR1A = target;
  } else {
    OCR1B = target;
  }
}

void adjust_duty_cycle(int16_t ten_thousandths, uint8_t motor) {
  int16_t drive_signal; 

  if (motor == 0) {
    // If we're going backwards
    if (get_direction(0) == PE1_BACKWARD) drive_signal = -1 * OCR1A;
    else drive_signal = OCR1A;
  } else {
    // If we're going backwards
    if (get_direction(1) == PE2_BACKWARD) drive_signal = -1 * OCR1B;
    else drive_signal = OCR1B;
  }

  drive_signal += ten_thousandths;

  uint16_t magnitude;

  if (drive_signal < 0) {
    go_backwards(motor);
    magnitude = (uint16_t) (-1 * drive_signal);
  } else {
    go_forwards(motor);
    magnitude = (uint16_t) drive_signal;
  }

  // Restrict the range of magnitudes
  if (magnitude < 100) {
     magnitude = 0;
  } else if (magnitude < 1000) {
    magnitude = 1000;
  } else if (magnitude > 7500) {
    magnitude = 7500;
  } 

  if (motor == 0) {
    OCR1A = magnitude;
  } else {
    OCR1B = magnitude;
  }
}

int get_counts(uint8_t motor) {
  if (motor == 0) {
    ATOMIC_GET(counts_m1);
  } else {
    ATOMIC_GET(counts_m2);
  }
}

int get_target(uint8_t motor) {
  if (motor == 0) {
    ATOMIC_GET(target_m1);
  } else {
    ATOMIC_GET(target_m2);
  }
}

int16_t set_target(uint8_t motor, int8_t byte) {
  // Map numbers in the range [-128, 127] to numbers in the range +/- [12,90],
  // which correspond to the number of counts per 100Hz sampling period for 1/2
  // a revolution per second to 4 revolutions per second
  int16_t target = (((int16_t) byte) * 90) / 128;

  if ((target < 12) && (target > -12)) target = 0;
  if (target > 90) target = 90;
  if (target < -90) target = -90;

  if (motor == 0) target_m1 = target;
  else target_m2 = target;

  return target;
}

ISR(PCINT0_vect) {
  unsigned char m1a_val = read_pin(encoder_1a);
  unsigned char m1b_val = read_pin(encoder_1b);
  unsigned char m2a_val = read_pin(encoder_2a);
  unsigned char m2b_val = read_pin(encoder_2b);

  char plus_m1 = m1a_val ^ global_last_m1b_val;
  char minus_m1 = m1b_val ^ global_last_m1a_val;
  char plus_m2 = m2a_val ^ global_last_m2b_val;
  char minus_m2 = m2b_val ^ global_last_m2a_val;

  if(plus_m1) counts_m1 += 1;
  if(minus_m1) counts_m1 -= 1;
  if(plus_m2) counts_m2 += 1;
  if(minus_m2) counts_m2 -= 1;

  global_last_m1a_val = m1a_val;
  global_last_m1b_val = m1b_val;
  global_last_m2a_val = m2a_val;
  global_last_m2b_val = m2b_val;
}

// This ISR runs at 1 kHz
ISR(TIMER0_COMPA_vect) {
  time_ms++;
}

static int counter = 0;

// Motor control task runs at 100Hz
ISR(TIMER3_COMPA_vect) {
  // The motor counts are reinitialized to 0 every iteration
  int16_t speed_m1 = counts_m1;
  // Motor 2's reversed
  int16_t speed_m2 = -1 * counts_m2;

  int16_t error_m1 = target_m1 - speed_m1;
  int16_t error_m2 = target_m2 - speed_m2;

  if (integral_error_m1 > 1000)  integral_error_m1 = 1000;
  if (integral_error_m1 < -1000) integral_error_m1 = -1000;
  if (integral_error_m2 > 1000)  integral_error_m1 = 1000;
  if (integral_error_m2 < -1000) integral_error_m1 = -1000;

  int16_t diff_m1, diff_m2;

  diff_m1 = error_m1 - last_error_m1;
  diff_m2 = error_m2 - last_error_m2;

  // Divide and multiply by 10 for finer granularity without floats
  int16_t correction_m1 = (m1_p_gain * error_m1 + m1_d_gain * diff_m1 + integral_error_m1 * m1_i_gain) / 10;
  int16_t correction_m2 = (m2_p_gain * error_m2 + m2_d_gain * diff_m2 + integral_error_m2 * m2_i_gain) / 10;

  counts_m1 = 0;
  counts_m2 = 0;

  adjust_duty_cycle(correction_m1, 0);
  adjust_duty_cycle(correction_m2, 1);

  char log[128];
  if (currently_logging && (((counter++) % 10) == 0)) {
    snprintf(log, 128, "Motor 1: speed: %d, P: %d, I: %d, D: %d, correction: %d\r\n"
                       "Motor 2: speed: %d, P: %d, I: %d, D: %d, correction: %d\r\n",
                       speed_m1, error_m1, integral_error_m1, diff_m1, correction_m1,
                       speed_m2, error_m2, integral_error_m2, diff_m2, correction_m2);
    sendString((uint8_t *) log);
  }
}
