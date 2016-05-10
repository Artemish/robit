#define F_CPU 16000000ul
#include <avr/interrupt.h>
#include <avr/io.h>
#define MAX_BUFFER_SIZE 512

typedef struct {
	volatile uint8_t *buffer;
	uint16_t length;	// current number of bytes in buffer
	uint8_t front;
	uint8_t back;
} uart_fifo;

void setupUART(void);
void bufferInit(volatile uint8_t *buf, volatile uart_fifo *f);

// return -1 if buffer is full
int8_t sendString(uint8_t *s);
void sendChar(uint8_t c);