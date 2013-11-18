#include "rpi_lib/rpi.h"
#include <stdio.h>
#include <stdint.h>

int main(void){
	rpi_init();

	char str[256];
	uint64_t t;

	while(1){
		// 1秒ごとにsysclock送信
		t = micros();
		sprintf(str,"sysclock: %lld (us)\n",t);
		printf("%s",str);
		delay(1000);
	}
	
	return 0;
}
