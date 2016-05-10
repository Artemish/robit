#include <Wire.h>
#include <AStar32U4.h>

#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

AStar32U4Motors motors;
int M1Speed = 0;
int M2Speed = 0;
int newM1Speed = 0;
int newM2Speed = 0;

void setup()
{
  Wire.begin(8);
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
  // Uncomment to flip a motor's direction:
  // motors.flipM1(true);
  motors.flipM2(true);
}

void loop(){

  if (newM1Speed > M1Speed){
    M1Speed++;
  }
  else if (newM1Speed < M1Speed){
    M1Speed--;
  }
  if (newM2Speed > M2Speed){
    M2Speed++;
  }
  else if (newM2Speed < M2Speed){
    M2Speed--;
  }

  motors.setM1Speed(M1Speed);
  motors.setM2Speed(M2Speed);
  delay(2);
}

void receiveEvent(int howMany) {
 
  Wire.read(); // Purge off the leading zero
  
  newM1Speed = Wire.read();
  newM2Speed = Wire.read();
  int mult = Wire.read();
  if (mult == 0){
    mult = 1;
  }
  int divisor = Wire.read();
  if (divisor == 0){
    divisor = 1;
  }
  
  newM1Speed = newM1Speed - 128;
  newM2Speed = newM2Speed - 128;

  newM1Speed = (newM1Speed * mult) / divisor;
  newM2Speed = (newM2Speed * mult) / divisor;
  
  Serial.println(newM1Speed);
  Serial.println(newM2Speed);
}


// This ISR runs at 1 kHz
ISR(TIMER0_COMPA_vect) {
  time_ms++;
}

int currently_running, current_target, global_counts, last_error;
int motor_p_gain, motor_i_gain, motor_d_gain;

ISR(TIMER3_COMPA_vect) {
  if (!currently_running) return;

  int error = current_target - global_counts;
  int diff;

  if (last_error) {
    diff = error - last_error;
  } else {
    diff = 0;
  }

  int32_t correction = (motor_p_gain * error + motor_d_gain * diff) / 10;

  last_error = error;

  if (correction < 0) {
    write_register(&PORTE, DDE2, PE2_BACKWARD);
    correction = -1 * correction;
  } else {
    write_register(&PORTE, DDE2, PE2_FORWARD);
  }

  set_duty_cycle(correction);

  if (abs(error) < ACCEPTABLE_BOUND) {
    reached_position = 1;
  }

  char log[32];
  if (currently_logging) {
    snprintf(log, 32, "%d:%d:%li\r\n", global_counts, current_target, correction);
    xmit(log);
  }
}
