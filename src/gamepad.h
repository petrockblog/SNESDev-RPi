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

#ifndef GAMEPAD_H_
#define GAMEPAD_H_

#include "types.h"

/* bit masks for checking the button states for SNES controllers */
#define GPAD_SNES_B       0x01
#define GPAD_SNES_Y       0x02
#define GPAD_SNES_SELECT  0x04
#define GPAD_SNES_START   0x08
#define GPAD_SNES_UP      0x10
#define GPAD_SNES_DOWN    0x20
#define GPAD_SNES_LEFT    0x40
#define GPAD_SNES_RIGHT   0x80
#define GPAD_SNES_A       0x100
#define GPAD_SNES_X       0x200
#define GPAD_SNES_L       0x400
#define GPAD_SNES_R       0x800

/* bit masks for checking the button states for NES controllers */
#define GPAD_NES_B       0x01
#define GPAD_NES_SELECT  0x04
#define GPAD_NES_START   0x08
#define GPAD_NES_UP      0x10
#define GPAD_NES_DOWN    0x20
#define GPAD_NES_LEFT    0x40
#define GPAD_NES_RIGHT   0x80
#define GPAD_NES_A       0x100

typedef enum {
	GPAD_TYPE_NES = 0, GPAD_TYPE_SNES = 1
} GPAD_TYPE;

/* holds the GPIO pins for the clock, strobe and data signals */
typedef struct {
	GPAD_TYPE type;
	S16 port;
	S16 pin_clock;
	S16 pin_strobe;
	S16 pin_data;
	U16 state;
} GPAD_ST;

S16 gpad_open(GPAD_ST* const gpad);
S16 gpad_close();
S16 gpad_ioctrl();
S16 gpad_read(GPAD_ST* const gpad);

#endif /* GAMEPAD_H_ */
