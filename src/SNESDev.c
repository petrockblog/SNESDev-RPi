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

#include <linux/uinput.h>
#include <sys/time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <bcm2835.h>
#include <signal.h>
#include <time.h>
#include <stddef.h>
#include <stdint.h>

 #include "confuse.h"

#include "signal.h"
#include "btn.h"
#include "gamepad.h"
#include "uinput_kbd.h"
#include "uinput_gamepad.h"
#include "cpuinfo.h"

#define CFGFILENAME "/etc/snesdev.cfg"
#define BUTTONPIN     RPI_GPIO_P1_11
#define BUTTONPIN_V2  RPI_V2_GPIO_P1_11

/* time to wait after each cycle */
#define GPADSNUM 2 /* number of game pads to poll */
#define FRAMEWAIT 20
#define FRAMEWAITLONG 100

int16_t doRun, pollButton, pollPads;
UINP_KBD_DEV uinp_kbd;
UINP_GPAD_DEV uinp_gpads[GPADSNUM];

/* structure for the configuration parameters */
typedef struct {
	cfg_bool_t button_enabled;
	cfg_bool_t gamepad1_enabled;
	char *gamepad1_type;
	cfg_bool_t gamepad2_enabled;
	char *gamepad2_type;
} configuration_st;

cfg_t *cfg;
configuration_st confres;

/* Signal callback function */
void sig_handler(int signo) {
	int16_t ctr = 0;

	if ((signo == SIGINT) | (signo == SIGQUIT) | (signo == SIGABRT) | (signo =
			SIGTERM)) {
		printf("Releasing SNESDev-Rpi device(s).\n");
		pollButton = 0;
		pollPads = 0;
		uinput_kbd_close(&uinp_kbd);
		for (ctr = 0; ctr < GPADSNUM; ctr++) {
			uinput_gpad_close(&uinp_gpads[ctr]);
		}
	    cfg_free(cfg);
	    free(confres.gamepad1_type);
	    free(confres.gamepad2_type);

		doRun = 0;
	}
}

void register_signalhandlers() {
	/* Register signal handlers  */
	if (signal(SIGINT, sig_handler) == SIG_ERR )
		printf("\n[SNESDev-Rpi] Cannot catch SIGINT\n");
	if (signal(SIGQUIT, sig_handler) == SIG_ERR )
		printf("\n[SNESDev-Rpi] Cannot catch SIGQUIT\n");
	if (signal(SIGABRT, sig_handler) == SIG_ERR )
		printf("\n[SNESDev-Rpi] Cannot catch SIGABRT\n");
	if (signal(SIGTERM, sig_handler) == SIG_ERR )
		printf("\n[SNESDev-Rpi] Cannot catch SIGTERM\n");
}

/* checks, if a button on the pad is pressed and sends an event according the button state. */
inline void processPadBtn(uint16_t buttons, uint16_t evtype, uint16_t mask, uint16_t key,
		UINP_GPAD_DEV* uinp_gpad) {
	if ((buttons & mask) == mask) {
		uinput_gpad_write(uinp_gpad, key, 1, evtype);
	} else {
		uinput_gpad_write(uinp_gpad, key, 0, evtype);
	}
}

// void checkCommandLineArguments(int16_t argc, char *argv[]) {
int readConfigurationfile() {
    cfg_opt_t opts[] = {
        CFG_SIMPLE_INT("button_enabled", &confres.button_enabled),
        CFG_SIMPLE_INT("gamepad1_enabled", &confres.gamepad1_enabled),
        CFG_SIMPLE_STR("gamepad1_type", &confres.gamepad1_type),
        CFG_SIMPLE_INT("gamepad2_enabled", &confres.gamepad2_enabled),
        CFG_SIMPLE_STR("gamepad2_type", &confres.gamepad2_type),
        CFG_END()
    };
    cfg = cfg_init(opts, 0);
    if ( access(CFGFILENAME, F_OK) == -1 ) {
    	printf("[SNESDev-Rpi] Error: Cannot find /etc/snesdev.cfg\n");
		return 1;
    }
    if (cfg_parse(cfg, CFGFILENAME) == CFG_PARSE_ERROR) {
    	printf("[SNESDev-Rpi] Error: Cannot parse /etc/snesdev.cfg\n");
		return 1;
    }
    return 0;
}

int main(int argc, char *argv[]) {

	uint8_t ctr = 0;
	GPAD_ST gpads[GPADSNUM];
	BTN_DEV_ST button;
	
	if (!bcm2835_init())
		return 1;
	
	if (readConfigurationfile() != 0 ) {
		return 1;
	}

	if (confres.gamepad1_enabled || confres.gamepad2_enabled) {

		gpads[0].port = 1;
		gpads[1].port = 1;
		// check board revision and set pins to be used
		// these are acutally also used by the gamecon driver
		if (get_rpi_revision()==1)
		{
			gpads[0].pin_clock = RPI_GPIO_P1_19;
			gpads[0].pin_strobe = RPI_GPIO_P1_23;
			gpads[0].pin_data = RPI_GPIO_P1_05;
			gpads[1].pin_clock = RPI_GPIO_P1_19;
			gpads[1].pin_strobe = RPI_GPIO_P1_23;
			gpads[1].pin_data = RPI_GPIO_P1_07;		
		} else {
			gpads[0].pin_clock = RPI_V2_GPIO_P1_19;
			gpads[0].pin_strobe = RPI_V2_GPIO_P1_23;
			gpads[0].pin_data = RPI_V2_GPIO_P1_05;
			gpads[0].type = GPAD_TYPE_SNES;
			gpads[1].pin_clock = RPI_V2_GPIO_P1_19;
			gpads[1].pin_strobe = RPI_V2_GPIO_P1_23;
			gpads[1].pin_data = RPI_V2_GPIO_P1_07;		
		}
		if (strcmp(confres.gamepad1_type,"nes")==0) {
			gpads[0].type = GPAD_TYPE_NES;
		} else {
			gpads[0].type = GPAD_TYPE_SNES;
		}
		if (strcmp(confres.gamepad2_type,"nes")==0) {
			gpads[1].type = GPAD_TYPE_NES;
		} else {
			gpads[1].type = GPAD_TYPE_SNES;
		}

		if (confres.gamepad1_enabled) {
			printf("[SNESDev-Rpi] Enabling game pad 1 with type '%s'.\n", confres.gamepad1_type);
			gpad_open(&gpads[0]);
			switch (gpads->type) {
			case GPAD_TYPE_SNES:
				uinput_gpad_open(&uinp_gpads[0], UINPUT_GPAD_TYPE_SNES);
				break;
			case GPAD_TYPE_NES:
				uinput_gpad_open(&uinp_gpads[0], UINPUT_GPAD_TYPE_NES);
				break;
			default:
				break;
			}
		}
		if (confres.gamepad2_enabled) {
			printf("[SNESDev-Rpi] Enabling game pad 2 with type '%s'.\n", confres.gamepad2_type);
			gpad_open(&gpads[1]);
			switch (gpads->type) {
			case GPAD_TYPE_SNES:
				uinput_gpad_open(&uinp_gpads[1], UINPUT_GPAD_TYPE_SNES);
				break;
			case GPAD_TYPE_NES:
				uinput_gpad_open(&uinp_gpads[1], UINPUT_GPAD_TYPE_NES);
				break;
			default:
				break;
			}
		}
	}

	if (confres.button_enabled) {
		printf("[SNESDev-Rpi] Enabling button.\n");
		button.port = 1;
		if (get_rpi_revision()==1) {
			button.pin = BUTTONPIN;
		} else {
			button.pin = BUTTONPIN_V2;
		}		
		btn_open(&button);
		uinput_kbd_open(&uinp_kbd);
	}

	register_signalhandlers();

	///* enter the main loop */
	doRun = 1;
	while (doRun) {
		if (confres.gamepad1_enabled || confres.gamepad2_enabled) {						
			/* read states of the buttons */
			for (ctr = 0; ctr < GPADSNUM; ctr++) {
				gpad_read(&gpads[ctr]);
			}

			///* send an event (pressed or released) for each button */
			for (ctr = 0; ctr < GPADSNUM; ctr++) {
				if ((ctr==0 && !(confres.gamepad1_enabled)) || 
					(ctr==1 && !(confres.gamepad2_enabled))) {
					continue;
				}

				gpad_read(&gpads[ctr]);
				processPadBtn(gpads[ctr].state, EV_KEY, GPAD_SNES_A, BTN_A,
						&uinp_gpads[ctr]);
				processPadBtn(gpads[ctr].state, EV_KEY, GPAD_SNES_B, BTN_B,
						&uinp_gpads[ctr]);
				processPadBtn(gpads[ctr].state, EV_KEY, GPAD_SNES_X, BTN_X,
						&uinp_gpads[ctr]);
				processPadBtn(gpads[ctr].state, EV_KEY, GPAD_SNES_Y, BTN_Y,
						&uinp_gpads[ctr]);
				processPadBtn(gpads[ctr].state, EV_KEY, GPAD_SNES_L, BTN_TL,
						&uinp_gpads[ctr]);
				processPadBtn(gpads[ctr].state, EV_KEY, GPAD_SNES_R, BTN_TR,
						&uinp_gpads[ctr]);
				processPadBtn(gpads[ctr].state, EV_KEY, GPAD_SNES_SELECT,
						BTN_SELECT, &uinp_gpads[ctr]);
				processPadBtn(gpads[ctr].state, EV_KEY, GPAD_SNES_START,
						BTN_START, &uinp_gpads[ctr]);

				if ((gpads[ctr].state & GPAD_SNES_LEFT) == GPAD_SNES_LEFT) {
					uinput_gpad_write(&uinp_gpads[ctr], ABS_X, 0, EV_ABS);
				} else if ((gpads[ctr].state & GPAD_SNES_RIGHT) == GPAD_SNES_RIGHT) {
					uinput_gpad_write(&uinp_gpads[ctr], ABS_X, 4, EV_ABS);
				} else {
					uinput_gpad_write(&uinp_gpads[ctr], ABS_X, 2, EV_ABS);
				}
				if ((gpads[ctr].state & GPAD_SNES_UP) == GPAD_SNES_UP) {
					uinput_gpad_write(&uinp_gpads[ctr], ABS_Y, 0, EV_ABS);
				} else if ((gpads[ctr].state & GPAD_SNES_DOWN) == GPAD_SNES_DOWN) {
					uinput_gpad_write(&uinp_gpads[ctr], ABS_Y, 4, EV_ABS);
				} else {
					uinput_gpad_write(&uinp_gpads[ctr], ABS_Y, 2, EV_ABS);
				}
			}
		}

		if (confres.button_enabled) {
			btn_read(&button);

			switch (button.state) {
			case BTN_STATE_IDLE:
				break;
			case BTN_STATE_PRESSED:
				if (button.pressedCtr == 1 && button.duration >= 1) {
					uinput_kbd_write(&uinp_kbd, KEY_ESC, 1, EV_KEY);
				}
				break;
			case BTN_STATE_RELEASED:
					
					if (button.pressedCtr == 1 && button.duration >= 1) {
					 	uinput_kbd_write(&uinp_kbd, KEY_ESC, 0, EV_KEY);
					} else if (button.pressedCtr == 2 && button.duration >= 1) {
						uinput_kbd_write(&uinp_kbd, KEY_F4, 1, EV_KEY);
						usleep(50000);
						uinput_kbd_write(&uinp_kbd, KEY_F4, 0, EV_KEY);
						// shutting down
						system("shutdown -t 3 -h now");
						
						}
				break;
			}
		}

		/* wait for some time to keep the CPU load low */
		if (confres.button_enabled && !(confres.gamepad1_enabled || confres.gamepad2_enabled)) {
			delay(FRAMEWAITLONG);
		} else {
			delay(FRAMEWAIT);
		}
	}

	return 0;

}
