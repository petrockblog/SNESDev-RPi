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

#include "gamepad.h"
#include "GPIO.h"

int16_t gpad_open(GPAD_ST* const gpad) {
	gpad->state = 0;

	gpio_open(gpad->port, gpad->pin_strobe, GPIO_OUTPUT);
	gpio_open(gpad->port, gpad->pin_clock, GPIO_OUTPUT);
	gpio_open(gpad->port, gpad->pin_data, GPIO_INPUT);

	gpio_write_pin(gpad->port, gpad->pin_strobe, GPIO_LOW);
	gpio_write_pin(gpad->port, gpad->pin_clock, GPIO_LOW);
	
	return 0;
}

int16_t gpad_close() {
	return -1;
}

int16_t gpad_ioctrl() {
	return -1;
}

int16_t gpad_read(GPAD_ST* const gpad) {
	int16_t i;

	gpio_write_pin(gpad->port, gpad->pin_strobe, GPIO_HIGH);
	delayMicroseconds(2);
	gpio_write_pin(gpad->port, gpad->pin_strobe, GPIO_LOW);
	delayMicroseconds(2);

	gpad->state = 0;
	switch (gpad->type) {
		case GPAD_TYPE_SNES:
			for (i = 0; i < 16; i++) {

				uint8_t curpin1 = gpio_read_pin(gpad->port, gpad->pin_data);
				gpio_write_pin(gpad->port, gpad->pin_clock, GPIO_HIGH);
				delayMicroseconds(2);
				gpio_write_pin(gpad->port, gpad->pin_clock, GPIO_LOW);
				delayMicroseconds(2);

				if (curpin1 == GPIO_LOW) {
					gpad->state |= (1 << i);
				}
			}

			// set to 0 if the controller is not connected
			if ((gpad->state & 0xFFF) == 0xFFF) {
				gpad->state = 0;
			}
		break;
	case GPAD_TYPE_NES:
			for (i = 0; i < 8; i++) {

				uint8_t curpin1 = gpio_read_pin(gpad->port, gpad->pin_data);
				gpio_write_pin(gpad->port, gpad->pin_clock, GPIO_HIGH);
				delayMicroseconds(2);
				gpio_write_pin(gpad->port, gpad->pin_clock, GPIO_LOW);
				delayMicroseconds(2);

				if (curpin1 == GPIO_LOW) {
					gpad->state |= (1 << i);
				}
			}

			// set to 0 if the controller is not connected
			if ((gpad->state & 0xFFF) == 0xFFF) {
				gpad->state = 0;
			}
		break;
	default:
		break;
	}
	return gpad->state;
}

