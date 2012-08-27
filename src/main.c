/*
 * SNESDev - Simulates a virtual keyboard for two SNES controllers that are 
 * connected to the GPIO pins of the Raspberry Pi.
 *
 * (c) Copyright 2012  Florian M√ºller (petrockblog@flo-mueller.com)
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
#include <sys/time.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <bcm2835.h>

#include "SNESpad.h"

/* time to wait after each cycle */
#define FRAMEWAIT 20

/* set the GPIO pins of the button and the LEDs. */
#define BUTTONRESET RPI_GPIO_P1_13

/* Setup the uinput device */
int setup_uinput_device() {
  int uinp_fd = open("/dev/uinput", O_WRONLY | O_NDELAY);
	if (uinp_fd == 0) {
		printf("Unable to open /dev/uinput\n");
		return -1;
	}

	struct uinput_user_dev uinp;
	memset(&uinp, 0, sizeof(uinp)); 
	strncpy(uinp.name, "SNES-to-Keyboard Device", strlen("SNES-to-Keyboard Device"));
	uinp.id.version = 4;
	uinp.id.bustype = BUS_USB;
	uinp.id.product = 1;
	uinp.id.vendor = 1;

	// Setup the uinput device
	ioctl(uinp_fd, UI_SET_EVBIT, EV_KEY);
	ioctl(uinp_fd, UI_SET_EVBIT, EV_REL);
	int i = 0;
	for (i = 0; i < 256; i++) {
		ioctl(uinp_fd, UI_SET_KEYBIT, i);
	}

	/* Create input device into input sub-system */
	write(uinp_fd, &uinp, sizeof(uinp));
	if (ioctl(uinp_fd, UI_DEV_CREATE)) {
		printf("Unable to create UINPUT device.");
		return -1;
	}

	return uinp_fd;
}

/* sends a key event to the virtual device */
void send_key_event(int fd, unsigned int keycode, int keyvalue) {
	struct input_event event;
	gettimeofday(&event.time, NULL);

	event.type = EV_KEY;
	event.code = keycode;
	event.value = keyvalue;

	if (write(fd, &event, sizeof(event)) < 0) {
		printf("simulate key error\n");
	}

	event.type = EV_SYN;
	event.code = SYN_REPORT;
	event.value = 0;
	write(fd, &event, sizeof(event));
	if (write(fd, &event, sizeof(event)) < 0) {
		printf("simulate key error\n");
	}
}


/* checks, if a button on the pad is pressed and sends an event according the button state. */
void processBtn(uint16_t buttons, uint16_t mask, uint16_t key, int uinh) {
	if ( (buttons & mask) == mask ) {
		send_key_event(uinh, key, 1);
	} else {
		send_key_event(uinh, key, 0);
	}
}

void readResetButton(int uinh){
	
	// read the state of the button into a local variable
	uint8_t buttonState = bcm2835_gpio_lev(BUTTONRESET);
	
	if ( buttonState==HIGH ) {
		send_key_event(uinh, KEY_ESC, 1);
	} else {
		send_key_event(uinh, KEY_ESC, 0);
	}
}

int main(int argc, char *argv[]) {

    buttonstates buttons;

    if (!bcm2835_init())
        return 1;

    // Set RPI pin to be an input
    bcm2835_gpio_fsel(BUTTONRESET, BCM2835_GPIO_FSEL_INPT);
    //  with a pullup
    bcm2835_gpio_set_pud(BUTTONRESET, BCM2835_GPIO_PUD_DOWN);

    /* initialize controller structures with GPIO pin assignments */
	snespad pads;
	pads.clock  = RPI_GPIO_P1_18;
	pads.strobe = RPI_GPIO_P1_16;
	pads.data1  = RPI_GPIO_P1_22;
	pads.data2  = RPI_GPIO_P1_15;


	/* set GPIO pins as input or output pins */
	initializePads( &pads );

	/* intialize virtual input device */
	int uinp_fd;
	if ((uinp_fd = setup_uinput_device()) < 0) {
		printf("Unable to find uinput device\n");
		return -1;
	}

	/* enter the main loop */
	while ( 1 ) {

		/* Read the reset button state */
		readResetButton(uinp_fd);

		/* read states of the buttons */
		updateButtons(&pads, &buttons);

		/* send an event (pressed or released) for each button */
		/* key events for first controller */
        processBtn(buttons.buttons1, SNES_A,     KEY_X,          uinp_fd);
        processBtn(buttons.buttons1, SNES_B,     KEY_Z,          uinp_fd);
        //processBtn(buttons.buttons1, SNES_X,     KEY_S,          uinp_fd);
        //processBtn(buttons.buttons1, SNES_Y,     KEY_A,          uinp_fd);
        //processBtn(buttons.buttons1, SNES_L,     KEY_Q,          uinp_fd);
        //processBtn(buttons.buttons1, SNES_R,     KEY_W,          uinp_fd);
        processBtn(buttons.buttons1, SNES_SELECT,KEY_RIGHTSHIFT, uinp_fd);
        processBtn(buttons.buttons1, SNES_START, KEY_ENTER,      uinp_fd);
        processBtn(buttons.buttons1, SNES_LEFT,  KEY_LEFT,       uinp_fd);
        processBtn(buttons.buttons1, SNES_RIGHT, KEY_RIGHT,      uinp_fd);
        processBtn(buttons.buttons1, SNES_UP,    KEY_UP,         uinp_fd);
        processBtn(buttons.buttons1, SNES_DOWN,  KEY_DOWN,       uinp_fd);

		// key events for second controller 
        processBtn(buttons.buttons2, SNES_A,     KEY_E, uinp_fd);
        processBtn(buttons.buttons2, SNES_B,     KEY_R, uinp_fd);
        //processBtn(buttons.buttons2, SNES_X,     KEY_T, uinp_fd);
        //processBtn(buttons.buttons2, SNES_Y,     KEY_Y, uinp_fd);
        //processBtn(buttons.buttons2, SNES_L,     KEY_U, uinp_fd);
        //processBtn(buttons.buttons2, SNES_R,     KEY_I, uinp_fd);
        processBtn(buttons.buttons2, SNES_SELECT,KEY_O, uinp_fd);
        processBtn(buttons.buttons2, SNES_START, KEY_P, uinp_fd);
        processBtn(buttons.buttons2, SNES_LEFT,  KEY_C, uinp_fd);
        processBtn(buttons.buttons2, SNES_RIGHT, KEY_B, uinp_fd);
        processBtn(buttons.buttons2, SNES_UP,    KEY_F, uinp_fd);
        processBtn(buttons.buttons2, SNES_DOWN,  KEY_V, uinp_fd);

		/* wait for some time to keep the CPU load low */
		delay(FRAMEWAIT);
	}

	/* Destroy the input device */
	ioctl(uinp_fd, UI_DEV_DESTROY);
	/* Close the UINPUT device */
	close(uinp_fd);
}
