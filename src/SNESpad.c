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

#include "SNESpad.h"

/* set the GPIO pins as input or output pins */
void initializePad(snespad *pad) {
  printf("Initializing pad.\n");
  bcm2835_gpio_fsel( pad->strobe, BCM2835_GPIO_FSEL_OUTP );
  bcm2835_gpio_fsel( pad->clock,  BCM2835_GPIO_FSEL_OUTP );
  bcm2835_gpio_fsel( pad->data,   BCM2835_GPIO_FSEL_INPT );

  bcm2835_gpio_write( pad->strobe, LOW );
  bcm2835_gpio_write( pad->clock, LOW );
}

/* check the state of each button */
void updateButtons(snespad *pad, uint16_t* buttons) {
  bcm2835_gpio_write( pad->strobe, HIGH );
  delayMicroseconds(2);
  bcm2835_gpio_write( pad->strobe, LOW );
  delayMicroseconds(2);

  *buttons = 0;
  int i;
  for (i = 0; i < 16; i++) {

    uint8_t curpin = bcm2835_gpio_lev(pad->data);
    bcm2835_gpio_write( pad->clock, HIGH );
    delayMicroseconds(2);
    bcm2835_gpio_write( pad->clock, LOW );
    delayMicroseconds(2);

    if( curpin==LOW ) {
      *buttons |= (1<<i);
    } 
    
  }
}
