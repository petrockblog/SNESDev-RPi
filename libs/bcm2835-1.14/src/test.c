// Test program for bcm2835 library
// You can only expect this to run correctly
// as root on Raspberry Pi hardware
//
// Author: Mike McCauley (mikem@open.com.au)
// Copyright (C) 2011 Mike McCauley
// $Id: test.c,v 1.3 2012/11/29 01:39:33 mikem Exp $

#include <bcm2835.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    // Check name compatibility
    delay(100);
    delayMicroseconds(100);
    bcm2835_delay(100);
    bcm2835_delayMicroseconds(100);

    if (geteuid() == 0)
    {
	if (!bcm2835_init())
	    return 1;
	if (!bcm2835_close())
	    return 1;
    }
    else
    {
	fprintf(stderr, "****You need to be root to properly run this test program\n");
    }
    return 0;
}
