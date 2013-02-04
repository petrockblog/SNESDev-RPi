/*
 * SNESDev - Simulates a virtual keyboard for two SNES controllers that are 
 * connected to the GPIO pins of the Raspberry Pi.
 *
 * (c) Copyright 2012  Florian Müller (petrockblock@gmail.com)
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
#include <stdlib.h>
#include <unistd.h>
#include <bcm2835.h>
#include <signal.h>
#include <time.h>

#include "SNESpad.h"
#include "cpuinfo.h"

/* time to wait after each cycle */
#define FRAMEWAIT 20
#define FRAMEWAITLONG 100

/* set the GPIO pins of the button and the LEDs. */
#define BUTTONPIN     RPI_GPIO_P1_11
#define BUTTONPIN_V2  RPI_V2_GPIO_P1_11
#define BTNSTATE_IDLE 0
#define BTNSTATE_PRESS 1
#define BTNSTATE_RELEASE 2


int uinp_kbd, uinp_gp1, uinp_gp2;
int doRun, pollButton, pollPads;
time_t btnLastTime;
uint8_t btnState;
uint8_t btnPressCtr;
int buttonPin;

/* Signal callback function */
void sig_handler(int signo) {
  if ((signo == SIGINT) | (signo == SIGKILL) | (signo == SIGQUIT) | (signo == SIGABRT)) {
  	printf("Releasing SNESDev-Rpi device(s).\n");
  	pollButton = 0;
  	pollPads = 0;
	/* Destroy the input devices */
	ioctl(uinp_kbd, UI_DEV_DESTROY);
	ioctl(uinp_gp1, UI_DEV_DESTROY);
	ioctl(uinp_gp2, UI_DEV_DESTROY);

	/* Close the UINPUT devices */
	close(uinp_kbd);  	
	close(uinp_gp1);  	
	close(uinp_gp2);  	
	doRun = 0;
  }
}

/* sends a key event to the virtual device */
void send_key_event(int fd, unsigned int keycode, int keyvalue, unsigned int evtype) {
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
int setup_uinput_keyboard_device() {
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
	// keyboard
	ioctl(uinp_fd, UI_SET_EVBIT, EV_KEY);
	ioctl(uinp_fd, UI_SET_EVBIT, EV_REL);
	int i = 0;
	for (i = 0; i < 256; i++) {
		ioctl(uinp_fd, UI_SET_KEYBIT, i);
	}

	/* Create input device into input sub-system */
	write(uinp_fd, &uinp, sizeof(uinp));
	if (ioctl(uinp_fd, UI_DEV_CREATE)) {
		printf("[SNESDev-Rpi] Unable to create UINPUT device.");
		return -1;
	}

	return uinp_fd;
}

/* Setup the uinput device */
int setup_uinput_gamepad_device() {
	int uinp_fd = open("/dev/uinput", O_WRONLY | O_NDELAY);
	if (uinp_fd == 0) {
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
	ioctl(uinp_fd, UI_SET_EVBIT, EV_KEY);
	ioctl(uinp_fd, UI_SET_EVBIT, EV_REL);

	// gamepad, buttons
	ioctl(uinp_fd, UI_SET_KEYBIT, BTN_A);
	ioctl(uinp_fd, UI_SET_KEYBIT, BTN_B);
	ioctl(uinp_fd, UI_SET_KEYBIT, BTN_X);
	ioctl(uinp_fd, UI_SET_KEYBIT, BTN_Y);
	ioctl(uinp_fd, UI_SET_KEYBIT, BTN_TL);
	ioctl(uinp_fd, UI_SET_KEYBIT, BTN_TR);
	ioctl(uinp_fd, UI_SET_KEYBIT, BTN_SELECT);
	ioctl(uinp_fd, UI_SET_KEYBIT, BTN_START);
	// gamepad, directions
	ioctl(uinp_fd, UI_SET_EVBIT, EV_ABS);
	ioctl(uinp_fd, UI_SET_ABSBIT, ABS_X);
	ioctl(uinp_fd, UI_SET_ABSBIT, ABS_Y);
	uinp.absmin[ABS_X] = 0;
	uinp.absmax[ABS_X] = 4;
	uinp.absmin[ABS_Y] = 0;
	uinp.absmax[ABS_Y] = 4;

	/* Create input device into input sub-system */
	write(uinp_fd, &uinp, sizeof(uinp));
	if (ioctl(uinp_fd, UI_DEV_CREATE)) {
		printf("[SNESDev-Rpi] Unable to create UINPUT device.");
		return -1;
	}

	send_key_event(uinp_fd, ABS_X, 2, EV_ABS);
	send_key_event(uinp_fd, ABS_Y, 2, EV_ABS);

	return uinp_fd;
}

/* checks the state of the button and decides for a short or long button press */
void checkButton(int uinh) {
  
  	// read the state of the button into a local variable
	uint8_t buttonState = bcm2835_gpio_lev(buttonPin);
  
  	// three-state machine:
  	// - press and hold: send "r" key (for rewind function of RetroArch)
  	// - press and release three times: send "ESC"
  	// - press and release five times: shutdown
	switch ( btnState ) {
		case BTNSTATE_IDLE:
			if (buttonState==HIGH ) {
				btnLastTime=time(NULL);
				btnState = BTNSTATE_PRESS;
				btnPressCtr += 1;
			}
			break;
		case BTNSTATE_PRESS:
			if (buttonState==LOW ) {
				btnLastTime=time(NULL);
				btnState = BTNSTATE_RELEASE;
			} else if (buttonState==HIGH && btnPressCtr==1 && difftime(time(NULL),btnLastTime)>1) {
				send_key_event(uinh, KEY_R,1,EV_KEY);
			}
			break;
		case BTNSTATE_RELEASE:
		 	if (buttonState==LOW && difftime(time(NULL),btnLastTime)>1 ) {

			 	if (btnPressCtr==1) {
					send_key_event(uinh, KEY_R,0,EV_KEY);
			 	} else if (btnPressCtr==3) {
					// Sending ESC
					send_key_event(uinh, KEY_ESC,1,EV_KEY);
					usleep(50000);
					send_key_event(uinh, KEY_ESC,0,EV_KEY);
				} else if ( btnPressCtr==5 ) {
					// shutting down
					pollButton = 0;
			 		pollPads = 0;
					send_key_event(uinh, KEY_F4,1,EV_KEY);
					usleep(50000);
					send_key_event(uinh, KEY_F4,0,EV_KEY);

					system("shutdown -t 3 -h now");
				}

				btnLastTime=time(NULL);
				btnState = BTNSTATE_IDLE;
				btnPressCtr = 0;
			} else if (buttonState==HIGH  ) {
				btnLastTime=time(NULL);
				btnState = BTNSTATE_PRESS;
				btnPressCtr += 1;
			}
			break;
	}
	// printf("State: %d, ctr: %d\n",btnState,btnPressCtr );
}

/* checks, if a button on the pad is pressed and sends an event according the button state. */
void processPadBtn(uint16_t buttons, uint16_t evtype, uint16_t mask, uint16_t key, int uinh) {
	if ( (buttons & mask) == mask ) {
		send_key_event(uinh, key, 1, evtype);
	} else {
		send_key_event(uinh, key, 0, evtype);
	}
}


int main(int argc, char *argv[]) {

    buttonstates padButtons;
	snespad pads;
	pollButton = 1;
	pollPads = 1;
	doRun = 1;

	btnState = BTNSTATE_IDLE;
	btnPressCtr = 0;
    if (get_rpi_revision()==1) {
    	buttonPin = BUTTONPIN;
    } else {
		buttonPin = BUTTONPIN_V2;
    }


	// check command line arguments
	if (argc > 1) {
		// argv[1]==1 poll controllers only
		// argv[1]==2 poll button only
		// argv[1]==3 poll controllers and button
		switch ( atoi(argv[argc-1]) ) {
			case 1:
				printf("[SNESDev-Rpi] Polling only controllers.\n");
				pollButton = 0;
				pollPads = 1;
			break;
			case 2:
				printf("[SNESDev-Rpi] Polling button only.\n");
				pollButton = 1;
				pollPads = 0;
			break;
			case 3:
				printf("[SNESDev-Rpi] Polling controllers and button.\n");
				pollButton = 1;
				pollPads = 1;
			break;
			default:
				return -1;
		}
    } else {
    	printf("[SNESDev-Rpi] Polling controllers and button.\n");
    }

    if (!bcm2835_init())
        return 1;

	// initialize button and LEDs
    bcm2835_gpio_fsel(buttonPin,  BCM2835_GPIO_FSEL_INPT);

    /* initialize controller structures with GPIO pin assignments */

    // pin out used in SNESDev article on blog
	// pads.clock  = RPI_GPIO_P1_18;
	// pads.strobe = RPI_GPIO_P1_16;
	// pads.data1  = RPI_GPIO_P1_22;
	// pads.data2  = RPI_GPIO_P1_15;

    // check board revision and set pins to be used
    // these are acutally also used by the gamecon driver
    if (get_rpi_revision()==1)
    {
		pads.clock  = RPI_GPIO_P1_19;
		pads.strobe = RPI_GPIO_P1_23;
		pads.data1  = RPI_GPIO_P1_07;
		pads.data2  = RPI_GPIO_P1_05;
    } else {
		pads.clock  = RPI_V2_GPIO_P1_19;
		pads.strobe = RPI_V2_GPIO_P1_23;
		pads.data1  = RPI_V2_GPIO_P1_07;
		pads.data2  = RPI_V2_GPIO_P1_05;
    }

	/* set GPIO pins as input or output pins */
	initializePads( &pads );

	/* intialize virtual input devices */
	if ((uinp_kbd = setup_uinput_keyboard_device()) < 0) {
		printf("[SNESDev-Rpi] Unable to create uinput keyboard device\n");
		return -1;
	}
	if ((uinp_gp1 = setup_uinput_gamepad_device()) < 0) {
		printf("[SNESDev-Rpi] Unable to create uinput gamepad device 1\n");
		return -1;
	}
	if ((uinp_gp2 = setup_uinput_gamepad_device()) < 0) {
		printf("[SNESDev-Rpi] Unable to create uinput gamepad device 2\n");
		return -1;
	}

	/* Register signal handlers  */
	if (signal(SIGINT, sig_handler) == SIG_ERR)	printf("\n[SNESDev-Rpi] Cannot catch SIGINT\n");
	if (signal(SIGQUIT, sig_handler) == SIG_ERR) printf("\n[SNESDev-Rpi] Cannot catch SIGQUIT\n");
	if (signal(SIGABRT, sig_handler) == SIG_ERR) printf("\n[SNESDev-Rpi] Cannot catch SIGABRT\n");

	/* enter the main loop */
	while ( doRun ) {

		if (pollButton) {
			/* Check state of button. */
			checkButton(uinp_kbd);
		}

		if (pollPads) {
			/* read states of the buttons */
			updateButtons(&pads, &padButtons);

			/* send an event (pressed or released) for each button */
			/* key events for first controller */
	        processPadBtn(padButtons.buttons1, EV_KEY, SNES_A,     BTN_A,          uinp_gp1);
	        processPadBtn(padButtons.buttons1, EV_KEY, SNES_B,     BTN_B,          uinp_gp1);
	        processPadBtn(padButtons.buttons1, EV_KEY, SNES_X,     BTN_X,          uinp_gp1);
	        processPadBtn(padButtons.buttons1, EV_KEY, SNES_Y,     BTN_Y,          uinp_gp1);
	        processPadBtn(padButtons.buttons1, EV_KEY, SNES_L,     BTN_TL,          uinp_gp1);
	        processPadBtn(padButtons.buttons1, EV_KEY, SNES_R,     BTN_TR,          uinp_gp1);
	        processPadBtn(padButtons.buttons1, EV_KEY, SNES_SELECT,BTN_SELECT, uinp_gp1);
	        processPadBtn(padButtons.buttons1, EV_KEY, SNES_START, BTN_START,      uinp_gp1);

			if ( (padButtons.buttons1 & SNES_LEFT) == SNES_LEFT ) {
				send_key_event(uinp_gp1, ABS_X, 0, EV_ABS);
			} else if ( (padButtons.buttons1 & SNES_RIGHT) == SNES_RIGHT ) {
				send_key_event(uinp_gp1, ABS_X, 4, EV_ABS);
			} else {
				send_key_event(uinp_gp1, ABS_X, 2, EV_ABS);
			}	        
			if ( (padButtons.buttons1 & SNES_UP) == SNES_UP ) {
				send_key_event(uinp_gp1, ABS_Y, 0, EV_ABS);
			} else if ( (padButtons.buttons1 & SNES_DOWN) == SNES_DOWN ) {
				send_key_event(uinp_gp1, ABS_Y, 4, EV_ABS);
			} else {
				send_key_event(uinp_gp1, ABS_Y, 2, EV_ABS);
			}	        

			// key events for second controller 
	        processPadBtn(padButtons.buttons2, EV_KEY, SNES_A,     BTN_A, uinp_gp2);
	        processPadBtn(padButtons.buttons2, EV_KEY, SNES_B,     BTN_B, uinp_gp2);
	        processPadBtn(padButtons.buttons2, EV_KEY, SNES_X,     BTN_X, uinp_gp2);
	        processPadBtn(padButtons.buttons2, EV_KEY, SNES_Y,     BTN_Y, uinp_gp2);
	        processPadBtn(padButtons.buttons2, EV_KEY, SNES_L,     BTN_TL, uinp_gp2);
	        processPadBtn(padButtons.buttons2, EV_KEY, SNES_R,     BTN_TR, uinp_gp2);
	        processPadBtn(padButtons.buttons2, EV_KEY, SNES_SELECT,BTN_SELECT, uinp_gp2);
	        processPadBtn(padButtons.buttons2, EV_KEY, SNES_START, BTN_START, uinp_gp2);

			if ( (padButtons.buttons2 & SNES_LEFT) == SNES_LEFT ) {
				send_key_event(uinp_gp2, ABS_X, 0, EV_ABS);
			} else if ( (padButtons.buttons2 & SNES_RIGHT) == SNES_RIGHT ) {
				send_key_event(uinp_gp2, ABS_X, 4, EV_ABS);
			} else {
				send_key_event(uinp_gp2, ABS_X, 2, EV_ABS);
			}	        
			if ( (padButtons.buttons2 & SNES_UP) == SNES_UP ) {
				send_key_event(uinp_gp2, ABS_Y, 0, EV_ABS);
			} else if ( (padButtons.buttons2 & SNES_DOWN) == SNES_DOWN ) {
				send_key_event(uinp_gp2, ABS_Y, 4, EV_ABS);
			} else {
				send_key_event(uinp_gp2, ABS_Y, 2, EV_ABS);
			}	        
		}

		/* wait for some time to keep the CPU load low */
		if (pollButton && !pollPads) {
			delay(FRAMEWAITLONG);
		} else {
			delay(FRAMEWAIT);
		}
	}

	return 0;
}
