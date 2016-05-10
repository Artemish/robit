// Borrowed from github.com/g4lvanix/I2C_slave

#include <avr/io.h>
#include <util/twi.h>
#include <avr/interrupt.h>

#include "include/i2c_slave.h"

void i2c_stop(void){
  // clear acknowledge and enable bits
  TWCR &= ~( (1<<TWEA) | (1<<TWEN) );
}

ISR(TWI_vect){
  // temporary stores the received data
  uint8_t data;

  // own address has been acknowledged
  if( (TWSR & 0xF8) == TW_SR_SLA_ACK ) {  
    i2c_buffer_address = I2C_BUFLEN;
    // clear TWI interrupt flag, prepare to receive next byte and acknowledge
    TWCR |= (1<<TWIE) | (1<<TWINT) | (1<<TWEA) | (1<<TWEN); 
  } else if ( (TWSR & 0xF8) == TW_SR_DATA_ACK ) { // data has been received in slave receiver mode
    // save the received byte inside data 
    data = TWDR;

    // check wether an address has already been transmitted or not
    if(i2c_buffer_address == I2C_BUFLEN){

      i2c_buffer_address = data; 
      // clear TWI interrupt flag, prepare to receive next byte and acknowledge
      TWCR |= (1<<TWIE) | (1<<TWINT) | (1<<TWEA) | (1<<TWEN); 

    } else { // if a databyte has already been received
      // store the data at the current address
      i2c_rxbuffer[i2c_buffer_address] = data;

      // increment the buffer address
      i2c_buffer_address++;

      // if there is still enough space inside the buffer
      if(i2c_buffer_address < I2C_BUFLEN) {
        // clear TWI interrupt flag, prepare to receive next byte and acknowledge
        TWCR |= (1<<TWIE) | (1<<TWINT) | (1<<TWEA) | (1<<TWEN); 
      } else{
        // clear TWI interrupt flag, prepare to receive last byte and don't acknowledge
        TWCR |= (1<<TWIE) | (1<<TWINT) | (0<<TWEA) | (1<<TWEN); 
      }
    }

  } else if( (TWSR & 0xF8) == TW_ST_DATA_ACK ){ // device has been addressed to be a transmitter

    // copy data from TWDR to the temporary memory
    data = TWDR;

    // if no buffer read address has been sent yet
    if( i2c_buffer_address == 0xFF ){
      i2c_buffer_address = data;
    }

    // copy the specified buffer address into the TWDR register for transmission
    TWDR = i2c_txbuffer[i2c_buffer_address];
    // increment buffer read address
    i2c_buffer_address++;

    // if there is another buffer address that can be sent
    if(i2c_buffer_address < 0xFF){
      // clear TWI interrupt flag, prepare to send next byte and receive acknowledge
      TWCR |= (1<<TWIE) | (1<<TWINT) | (1<<TWEA) | (1<<TWEN); 
    } else{
      // clear TWI interrupt flag, prepare to send last byte and receive not acknowledge
      TWCR |= (1<<TWIE) | (1<<TWINT) | (0<<TWEA) | (1<<TWEN); 
    }

  } else{
    // if none of the above apply prepare TWI to be addressed again
    TWCR |= (1<<TWIE) | (1<<TWEA) | (1<<TWEN);
  } 
}

void init_i2c(void) {
  // load address into TWI address register
  TWAR = I2C_DEV_ADDR;
  // set the TWCR to enable address matching and enable TWI, clear TWINT, enable TWI interrupt
  TWCR = (1<<TWIE) | (1<<TWEA) | (1<<TWINT) | (1<<TWEN);
}
