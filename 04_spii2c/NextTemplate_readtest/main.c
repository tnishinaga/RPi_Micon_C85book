#include "rpi_lib/rpi.h"
#include <stdio.h>
#include <stdint.h>

// LIS3DH
// 3-axis sensor
#define LIS3DH_ADDR	 (0x30)

//Register Addresses:
#define  WHO_AM_I  0x0F //Dummy register

int main(void){
	rpi_init();

	// debug
	pinMode(22, ALT4);
	pinMode(4, ALT5);
	pinMode(27, ALT4);
	pinMode(25, ALT4);
	pinMode(24, ALT4);

	Wire1_begin();

	while(1){
		Wire1_beginTransmission(LIS3DH_ADDR);
		Wire1_writeByte(WHO_AM_I);
		Wire1_endTransmission(FALSE);

		Wire1_requestFrom(LIS3DH_ADDR, 1);

		if(Wire1_available()){
			printf("WHO_AM_I = 0x%02x\n", Wire1_read());
		}
		else{
			printf("WHO_AM_I = error!\n");
		}
		delay(3000);
	}

	return 0;
}
