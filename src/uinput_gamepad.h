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

#ifndef UINPUT_GAMEPAD_H_
#define UINPUT_GAMEPAD_H_

#include "types.h"

typedef enum {
	UINPUT_GPAD_TYPE_NES  = 0,
	UINPUT_GPAD_TYPE_SNES = 1
} UINPUT_GPAD_TYPE_E;

typedef struct  {
	S16 fd;
	S16 state;
} UINP_GPAD_DEV;

S16 uinput_gpad_open   (UINP_GPAD_DEV* const gpad, UINPUT_GPAD_TYPE_E type);
S16 uinput_gpad_close  (UINP_GPAD_DEV* const gpad);
S16 uinput_gpad_write  (UINP_GPAD_DEV* const gpad, U16 keycode, S16 keyvalue, U16 evtype);

#endif /* UINPUT_GAMEPAD_H_ */
