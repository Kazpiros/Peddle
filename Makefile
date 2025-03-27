# MCU and F_CPU
MCU = atmega328p
F_CPU = 16000000UL

# Compiler and programmer
CC = avr-g++
OBJCOPY = avr-objcopy
AVRDUDE = avrdude
PROGRAMMER = arduino
ISP_PROGRAMMER = stk500v1
PORT = /dev/ttyUSB0  # Change as needed

# Compiler flags
CFLAGS = -Wall -Os -mmcu=$(MCU) -DF_CPU=$(F_CPU)

# Directories
SRC_DIR = src
INCLUDE_DIR = include
TESTBENCH_DIR = Testbench

# Source files
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(SRC_FILES:.cpp=.o)

# Output file
TARGET = main

# Default target: build HEX file
all: $(TARGET).hex

# Compile C++ sources to object files
%.o: %.cpp
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Link object files into ELF
$(TARGET).elf: $(OBJ_FILES)
	$(CC) $(CFLAGS) $^ -o $@

# Generate HEX file from ELF
$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex $< $@

# Flash to ATmega328P using avrdude
flash: $(TARGET).hex
	$(AVRDUDE) -c $(PROGRAMMER) -p $(MCU) -P $(PORT) -b 115200 -U flash:w:$(TARGET).hex:i

lisp: $(TARGET).hex
	$(AVRDUDE) -c $(ISP_PROGRAMMER) -p $(MCU) -P $(PORT) -b 115200 -U flash:w:$(TARGET).hex:i

# Clean project
clean:
	rm -f $(SRC_DIR)/*.o $(TARGET).elf $(TARGET).hex
