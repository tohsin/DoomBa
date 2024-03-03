#!/bin/bash

# if [ $# -eq 0 ]; then
#     echo "No file path provided. Usage: ./compile.sh"
#     exit 1
# fi

# filepath=$1
output="a.out"
hexfile="a.hex"

# Get the directory and filename
# dirname=$(dirname "$filepath")
# filename=$(basename "$filepath")

# Compile the source code
avr-gcc -mmcu=atmega328 -c   usart.c helper.c encoder.c motor_controller.c speed_controller.c  main.c
# avr-gcc -mmcu=atmega328 -c  encoder.c  motor_controller.c  usart.c  util.c main.c

# link object files
avr-gcc -mmcu=atmega328   usart.o helper.o encoder.o motor_controller.o speed_controller.o  main.o
# avr-gcc -mmcu=atmega328  encoder.o motor_controller.o usart.o  util.o main.o
# Convert to Intel HEX format
avr-objcopy -O ihex -j .text -j .data "$output" "$hexfile"

# Upload the HEX file to the microcontroller using avrdude
avrdude -C /Applications/Arduino.app/Contents/Java/hardware/tools/avr/etc/avrdude.conf -v -p atmega328p -c USBasp -U flash:w:"$hexfile"

# Clean up temporary files
# rm "$output" "$hexfile"  encoder.o  motor_controller.o usart.o main.o
# rm "$output" "$hexfile" encoder.o usart.o main.o util.o
rm "$output" "$hexfile"  usart.o helper.o main.o encoder.o  motor_controller.o speed_controller.o
