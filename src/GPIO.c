/*
 * SNESDev - User-space driver for the RetroPie GPIO Adapter for the Raspberry Pi.
 *
 * (c) Copyright 2012-2013  Florian Müller (contact@petrockblock.com)
 *
 * SNESDev homepage: https://github.com/petrockblog/SNESDev-RPi
 *
 * Permission to use, copy, modify and distribute SNESDev in both binary and
 * source form, for non-commercial purposes, is hereby granted without fee,
 * providing that this license information and copyright notice appear with
 * all copies and any derived work.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event shall the authors be held liable for any damages
 * arising from the use of this software.
 *
 * SNESDev is freeware for PERSONAL USE only. Commercial users should
 * seek permission of the copyright holders first. Commercial use includes
 * charging money for SNESDev or software derived from SNESDev.
 *
 * The copyright holders request that bug fixes and improvements to the code
 * should be forwarded to them so everyone can benefit from the modifications
 * in future versions.
 *
 * Raspberry Pi is a trademark of the Raspberry Pi Foundation.
 */
 
#include <wiringPi.h> 
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "GPIO.h"

int16_t gpio_open(int16_t port, int16_t pin, GPIO_DIR direction)
{
	if (direction==GPIO_OUTPUT) {
		pinMode(pin, OUTPUT);
	} else if (direction==GPIO_INPUT) {		
		pinMode(pin, INPUT);
	} else {
		return -1;
	}	
	return 0;
}

uint8_t gpio_read_pin (int16_t port, int16_t pin)
{
	uint8_t val = digitalRead(pin);
	if (val==HIGH) {
		return GPIO_HIGH;
	} else if (val==LOW) {
		return GPIO_LOW;
	} else {
		return -1;
	}
}

int16_t gpio_write_pin  (int16_t port, int16_t pin, GPIO_VALUE val)
{
	if (val==GPIO_HIGH) {
		digitalWrite( pin, HIGH );
	} else if (val==GPIO_LOW) {
		digitalWrite( pin, LOW );
	} else {
		return -1;
	}
	return 0;
}
