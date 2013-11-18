#include "rpi_lib/rpi.h"

int main(void){
	rpi_init();

	while(1){
		// 1秒ごとに
		delay(1000);
		// HelloWorld!を送信
		uart0_puts("HelloWorld!\n");
	}
	return 0;
}