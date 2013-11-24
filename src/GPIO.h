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

#ifndef GPIO_H_
#define GPIO_H_

typedef enum {
	GPIO_SETDIR = 0,
	GPIO_TOOGLE = 1
} GPIO_IOCTRL_CMD;

typedef enum {
	GPIO_OUTPUT = 0,
	GPIO_INPUT  = 1
} GPIO_DIR;

typedef enum {
	GPIO_LOW  = 0,
	GPIO_HIGH = 1
} GPIO_VALUE;

int16_t          gpio_open       (int16_t port, int16_t pin, GPIO_DIR direction);
uint8_t          gpio_read_pin   (int16_t port, int16_t pin);
int16_t          gpio_write_pin  (int16_t port, int16_t pin, GPIO_VALUE val);

#endif /* GPIO_H_ */
