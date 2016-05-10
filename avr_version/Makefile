PORT=\\\\.\\USBSER000
MCU=atmega32u4
CFLAGS=-g -Wall -mcall-prologues -mmcu=$(MCU) -Os -save-temps
LDFLAGS=-lm -Wl,-gc-sections -Wl,-relax
CC=avr-gcc
TARGET=main
OBJECT_FILES=main.o uart.o i2cslave.o init.o io_wrappers.o motor.o pins.o 
all: $(TARGET).hex

clean:
	rm -f *.o *.hex *.obj *.hex *.s *.i

%.hex: %.obj
	avr-objcopy -R .eeprom -O ihex $< $@

%.obj: $(OBJECT_FILES)
	$(CC) $(CFLAGS) $(OBJECT_FILES) $(LDFLAGS) -o $@

program: $(TARGET).hex
	avrdude -p $(MCU) -c avr109 -P $(PORT) -U flash:w:$(TARGET).hex