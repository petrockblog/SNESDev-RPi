/*
 * SNESDev - Simulates a virtual keyboard for two SNES controllers that are 
 * connected to the GPIO pins of the Raspberry Pi.
 *
 * (c) Copyright 2012  Florian Müller (petrockblog@flo-mueller.com)
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

#ifndef SNESpad_h
#define SNESpad_h

#include <inttypes.h>
#include <bcm2835.h>
#include <unistd.h>
#include <stdio.h>

/* holds the GPIO pins for the clock, strobe and data signals */
typedef struct {
    RPiGPIOPin clock;
    RPiGPIOPin strobe;
    RPiGPIOPin data1;
    RPiGPIOPin data2;
} snespad;

/* holds the button staes for the first and second controller */
typedef struct {
	uint16_t buttons1;
	uint16_t buttons2;
} buttonstates;

/* bit masks for checking the button states */
#define SNES_B       0x01
#define SNES_Y       0x02
#define SNES_SELECT  0x04
#define SNES_START   0x08
#define SNES_UP      0x10
#define SNES_DOWN    0x20
#define SNES_LEFT    0x40
#define SNES_RIGHT   0x80
#define SNES_A       0x100
#define SNES_X       0x200
#define SNES_L       0x400
#define SNES_R       0x800

/* check the state of each button of each controller */
void updateButtons( snespad *pad, buttonstates* buttons );

/* set the GPIO pins as input or output pins */
void initializePads( snespad *pad );

#endif

