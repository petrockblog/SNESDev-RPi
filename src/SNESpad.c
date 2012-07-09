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

/* define to enable debug outputs */
//#define DEBUG 

/* set the GPIO pins as input or output pins */
void initializePads( snespad *pad ) {
  printf("Initializing pad.\n");
  bcm2835_gpio_fsel( pad->strobe, BCM2835_GPIO_FSEL_OUTP );
  bcm2835_gpio_fsel( pad->clock,  BCM2835_GPIO_FSEL_OUTP );
  bcm2835_gpio_fsel( pad->data1,   BCM2835_GPIO_FSEL_INPT );
  bcm2835_gpio_fsel( pad->data2,   BCM2835_GPIO_FSEL_INPT );

  bcm2835_gpio_write( pad->strobe, LOW );
  bcm2835_gpio_write( pad->clock, LOW );
}

/* check the state of each button and each controller */
void updateButtons( snespad *pad, buttonstates* buttons ) {

  int i;

  bcm2835_gpio_write( pad->strobe, HIGH );
  delayMicroseconds(2);
  bcm2835_gpio_write( pad->strobe, LOW );
  delayMicroseconds(2);

  buttons->buttons1 = 0;
  buttons->buttons2 = 0;
  for (i = 0; i < 16; i++) {

    uint8_t curpin1 = bcm2835_gpio_lev(pad->data1);
    uint8_t curpin2 = bcm2835_gpio_lev(pad->data2);
    #ifdef DEBUG
      printf("1: %d    2: %d\n",curpin1,curpin2);
    #endif
    bcm2835_gpio_write( pad->clock, HIGH );
    delayMicroseconds(2);
    bcm2835_gpio_write( pad->clock, LOW );
    delayMicroseconds(2);

    if( curpin1==LOW ) {
      buttons->buttons1 |= (1<<i);
    } 
    if( curpin2==LOW ) {
      buttons->buttons2 |= (1<<i);
    } 
    
  }

}
