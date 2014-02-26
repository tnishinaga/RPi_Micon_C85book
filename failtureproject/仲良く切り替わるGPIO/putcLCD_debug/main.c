#include "rpi_lib/rpi.h"
#include <stdio.h>
#include <stdint.h>


int main(void){
	rpi_init();

	// debug
	pinMode(22, ALT4);
	pinMode(4, ALT5);
	pinMode(27, ALT4);
	pinMode(25, ALT4);
	pinMode(24, ALT4);

	pinMode(2, OUTPUT); // SDA
	pinMode(3, OUTPUT); // SCL
	
	digitalWrite(2,HIGH);
	digitalWrite(3,LOW);

	while(1);

	return 0;
}
