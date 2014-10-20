#include "rpi_lib/rpi.h"

int main(void){
	rpi_init();

	// GPIO47をOutputにセット
	pinMode(47,OUTPUT);
	// GPIO18をINPUT_PULLUPにセット
	pinMode(18,INPUT_PULLUP);
	digitalWrite(47,HIGH);

	while(1){
		// GPIO18がLOW(=ボタンが押された)ならLED点灯
		if(digitalRead(18) == LOW){
			digitalWrite(47,LOW);
		}
		else{
			digitalWrite(47,HIGH);
		}
	}

	return 0;
}