#include <avr/interrupt.h>

#define ATOMIC_GET(x) int tmp; cli(); tmp = x; sei(); return tmp;
#define ATOMIC_SET(x, v) cli(); x = v; sei();

// Easier to read interrupt enable/disable macros
#define disable_serial_rx_interrupt       (UCSR1B &= ~(1 << RXCIE1))
#define enable_serial_rx_interrupt        (UCSR1B |=  (1 << RXCIE1))
#define disable_serial_tx_ready_interrupt (UCSR1B &= ~(1 << UDRIE1))
#define enable_serial_tx_ready_interrupt  (UCSR1B |=  (1 << UDRIE1))

// These two are handy for managing the head & tail pointers/indices for circular buffers
#define increment_and_wrap(x,size) do { x++; if (x >= size) { x = 0; } } while (0)
#define decrement_and_unwrap(x,size) do { if (x == 0) { x = size - 1; } else { x--; } } while (0)
