#include "rpi_lib/rpi.h"

int main(void){
	rpi_init();

	// GPIO47をOutputにセット
	pinMode(47,OUTPUT);

	while(1){
		// GPIO47をLにセット
		digitalWrite(47, LOW);
		// wait 500ms
		delay(500);
		// GPIO47をHにセット
		digitalWrite(47, HIGH);
		// wait 500ms
		delay(500);
	}

	return 0;
}