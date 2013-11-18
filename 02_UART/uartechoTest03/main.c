#include "rpi_lib/rpi.h"

int main(void){
	rpi_init();

	char buf[256];
	while(1){
		uart0_gets(buf,256);
		uart0_puts(buf);
	}

	return 0;
}