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
 
#include <bcm2835.h> 
#include <stdio.h>
#include <string.h>

#include "GPIO.h"

int16_t gpio_open(int16_t port, int16_t pin, GPIO_DIR direction)
{
	if (direction==GPIO_OUTPUT) {
		bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_OUTP);
	} else if (direction==GPIO_INPUT) {		
		bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_INPT);
	} else {
		return -1;
	}	
	return 0;
}

uint8_t gpio_read_pin (int16_t port, int16_t pin)
{
	uint8_t val = bcm2835_gpio_lev(pin);
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
		bcm2835_gpio_write( pin, HIGH );
	} else if (val==GPIO_LOW) {
		bcm2835_gpio_write( pin, LOW );
	} else {
		return -1;
	}
	return 0;
}
