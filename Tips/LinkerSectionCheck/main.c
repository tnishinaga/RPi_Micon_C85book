#include "rpi_lib/rpi.h"

volatile int dataVal = 1;
volatile int bssVal;

int main(void){
	volatile char *rodataVal = "LoveLive!";

	rpi_init();

	return 0;
}