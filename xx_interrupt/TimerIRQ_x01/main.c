#include "rpi_lib/rpi.h"
#include <stdio.h>
#include <stdint.h>

// タイマー割り込み処理
void LEDblink(void){
	// ここにタイマ割り込みでやりたい処理を書く

	// LEDの状態保持用変数
	static unsigned int led = 0;

	if(led){
		// printf("LED: ON\n");
		digitalWrite(16, LOW);
	}
	else{
		// printf("LED: OFF\n");
		digitalWrite(16, HIGH);
	}
	// LEDの状態を反転
	led = !led;
}

int main(void){
	rpi_init();

	Timer_initialize(4000000);
	Timer_attachInterrupt(LEDblink);

	while(1);

	return 0;
}
