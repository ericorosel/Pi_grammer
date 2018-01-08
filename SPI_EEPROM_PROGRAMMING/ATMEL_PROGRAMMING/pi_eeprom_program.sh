#!/bin/bash

echo "Programming EEPROM..."

# get firmware file name
eeprom_set=$(find /home/pi/eeprom/*.hex)
$eeprom_set .= "/home/pi/eeprom/$eeprom_set"

#CHIP
DEVICE=atmega328p

# hard toggle of reset line, necessary to see successful programming on fresh ICs
sudo gpio -g mode 26 output
sudo gpio -g write 26 0
sleep 0.1
sudo gpio -g write 26 1
sleep 0.1

#program eeprom
sudo avrdude -p $DEVICE -C /home/pi/avrdude_gpio.conf  -c linuxspi -P /dev/spidev0.0 -b 1000000 -D -v -u -U eeprom:w:$eeprom_set:i 2>/home/pi/eeprom_results.txt
