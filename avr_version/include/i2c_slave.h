// Borrowed from github.com/g4lvanix/I2C_slave

#ifndef I2C_SLAVE_H
#define I2C_SLAVE_H

#define I2C_BUFLEN 0xFF
#define I2C_DEV_ADDR 0x08

volatile uint8_t i2c_buffer_address;
volatile uint8_t i2c_txbuffer[I2C_BUFLEN];
volatile uint8_t i2c_rxbuffer[I2C_BUFLEN];

void init_i2c(void);
void stop_i2c(void);

#endif // I2C_SLAVE_H
