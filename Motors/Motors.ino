#include <Wire.h>
#include <AStar32U4.h>

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

