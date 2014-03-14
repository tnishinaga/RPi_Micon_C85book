#include "rpi_lib/rpi.h"
#include <stdio.h>
#include <stdint.h>
#include "rpi_lib/device/AQM0802/AQM0802.h"


int main(void){
	uint8_t hello[] = "Hello";
	uint8_t world[] = "World";

	rpi_init();
	

	Wire1_begin();
	aqm0802_init();

	delay(200);

	// print "Hello"
	aqm0802_print_line(hello, 1);
	aqm0802_print_line(world, 2);

	return 0;
}
