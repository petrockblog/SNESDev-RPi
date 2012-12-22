#include <bcm2835.h>
#include "cpuinfo.h"

// Button on RPi pin GPIO 11
#define PIN    RPI_GPIO_P1_11
#define PIN_V2 RPI_V2_GPIO_P1_11

int main(int argc, char **argv)
{
    int usepin;

    if (!bcm2835_init())
		return 1;

    // check board revision and set the pin to be an output
    if (get_rpi_revision()==1)
    {
    	usepin = PIN;
    } else {
    	usepin = PIN_V2;
    }
    bcm2835_gpio_fsel(usepin, BCM2835_GPIO_FSEL_INP);

    while (1)
    {
    	if (bcm2835_gpio_lev(usepin)==LOW)
    	{
    		printf("Button state: NOT PRESSED\n");
    	} else {
    		printf("Button state: PRESSED\n");
    	}

		// wait a bit
		delay(500);
    }

    return 0;
}
