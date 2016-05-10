PORT=/dev/ttyACM0
MCU=atmega32u4
CFLAGS=-g -Wall -mcall-prologues -mmcu=$(MCU) -Os
LDFLAGS=-Wl,-gc-sections -Wl,-relax
CC=avr-gcc
TARGET=main
SOURCE_FILES := $(wildcard *.c)
HEADERS := $(wildcard include/*.h)
OBJECT_FILES := ${SOURCE_FILES:.c=.o}

all: $(TARGET).hex

clean:
	rm -f *.o *.hex *.obj *.hex

%.hex: %.obj
	avr-objcopy -R .eeprom -O ihex $< $@

%.obj: $(OBJECT_FILES) $(HEADERS)
	$(CC) $(CFLAGS) $(OBJECT_FILES) $(LDFLAGS) -o $@ 

program: $(TARGET).hex
	avrdude -p $(MCU) -c avr109 -P $(PORT) -U flash:w:$(TARGET).hex 
