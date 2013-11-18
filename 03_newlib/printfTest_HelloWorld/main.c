#include "rpi_lib/rpi.h"
#include <stdio.h>

int main(void){
	rpi_init();

	while(1){
		// 1秒ごとにHelloWorld送信
		printf("HelloWorld\n");
		delay(1000);
	}
	
	return 0;
}
