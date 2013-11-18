#include "rpi_lib/rpi.h"

int main(void){
	rpi_init();

	// GPIO16をOutputにセット
	pinMode(16,OUTPUT);

	while(1){
		// GPIO16をLにセット
		digitalWrite(16, LOW);
		// wait 500ms
		delay(500);
		// GPIO16をHにセット
		digitalWrite(16, HIGH);
		// wait 500ms
		delay(500);
	}

	return 0;
}