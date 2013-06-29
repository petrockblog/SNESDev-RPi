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
 
#include <linux/input.h>
#include <linux/uinput.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "uinput_gamepad.h"

/* sends a key event to the virtual device */
static void send_key_event(int fd, unsigned int keycode, int keyvalue, unsigned int evtype) {
	struct input_event event;
	gettimeofday(&event.time, NULL);

	event.type = evtype;
	event.code = keycode;
	event.value = keyvalue;

	if (write(fd, &event, sizeof(event)) < 0) {
		printf("[SNESDev-Rpi] Simulate key error\n");
	}

	event.type = EV_SYN;
	event.code = SYN_REPORT;
	event.value = 0;
	write(fd, &event, sizeof(event));
	if (write(fd, &event, sizeof(event)) < 0) {
		printf("[SNESDev-Rpi] Simulate key error\n");
	}
}

/* Setup the uinput device */
S16 uinput_gpad_open(UINP_GPAD_DEV* const gpad, UINPUT_GPAD_TYPE_E type) {
	S16 uinp_fd;
	gpad->fd = open("/dev/uinput", O_WRONLY | O_NDELAY);
	if (gpad->fd == 0) {
		printf("Unable to open /dev/uinput\n");
		return -1;
	}

	struct uinput_user_dev uinp;
	memset(&uinp, 0, sizeof(uinp));
	strncpy(uinp.name, "SNES-to-Gamepad Device", strlen("SNES-to-Gamepad Device"));
	uinp.id.version = 4;
	uinp.id.bustype = BUS_USB;
	uinp.id.product = 1;
	uinp.id.vendor = 1;

	// Setup the uinput device
	ioctl(gpad->fd, UI_SET_EVBIT, EV_KEY);
	ioctl(gpad->fd, UI_SET_EVBIT, EV_REL);

	// gamepad, buttons
	ioctl(gpad->fd, UI_SET_KEYBIT, BTN_A);
	ioctl(gpad->fd, UI_SET_KEYBIT, BTN_B);
	ioctl(gpad->fd, UI_SET_KEYBIT, BTN_X);
	ioctl(gpad->fd, UI_SET_KEYBIT, BTN_Y);
	ioctl(gpad->fd, UI_SET_KEYBIT, BTN_TL);
	ioctl(gpad->fd, UI_SET_KEYBIT, BTN_TR);
	ioctl(gpad->fd, UI_SET_KEYBIT, BTN_SELECT);
	ioctl(gpad->fd, UI_SET_KEYBIT, BTN_START);
	// gamepad, directions
	ioctl(gpad->fd, UI_SET_EVBIT, EV_ABS);
	ioctl(gpad->fd, UI_SET_ABSBIT, ABS_X);
	ioctl(gpad->fd, UI_SET_ABSBIT, ABS_Y);
	uinp.absmin[ABS_X] = 0;
	uinp.absmax[ABS_X] = 4;
	uinp.absmin[ABS_Y] = 0;
	uinp.absmax[ABS_Y] = 4;

	/* Create input device into input sub-system */
	write(gpad->fd, &uinp, sizeof(uinp));
	if (ioctl(gpad->fd, UI_DEV_CREATE)) {
		printf("[SNESDev-Rpi] Unable to create UINPUT device.");
		return -1;
	}

	send_key_event(gpad->fd, ABS_X, 2, EV_ABS);
	send_key_event(gpad->fd, ABS_Y, 2, EV_ABS);
	
	return uinp_fd;
}

S16 uinput_gpad_close(UINP_GPAD_DEV* const gpad) {
	ioctl(gpad->fd, UI_DEV_DESTROY);
	return close(gpad->fd);
}

/* sends a key event to the virtual device */
S16 uinput_gpad_write(UINP_GPAD_DEV* const gpad, U16 keycode, S16 keyvalue, U16 evtype) {
	struct input_event event;
	gettimeofday(&event.time, NULL);

	event.type = evtype;
	event.code = keycode;
	event.value = keyvalue;

	if (write(gpad->fd, &event, sizeof(event)) < 0) {
		printf("[SNESDev-Rpi] Simulate key error\n");
	}

	event.type = EV_SYN;
	event.code = SYN_REPORT;
	event.value = 0;
	write(gpad->fd, &event, sizeof(event));
	if (write(gpad->fd, &event, sizeof(event)) < 0) {
		printf("[SNESDev-Rpi] Simulate key error\n");
	}
	return 0;
}

