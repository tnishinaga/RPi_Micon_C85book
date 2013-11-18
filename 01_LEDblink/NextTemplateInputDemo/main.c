#include "rpi_lib/rpi.h"

int main(void){
	rpi_init();

	// GPIO16をOutputにセット
	pinMode(16,OUTPUT);
	// GPIO18をINPUT_PULLUPにセット
	pinMode(18,INPUT_PULLUP);
	digitalWrite(16,HIGH);

	while(1){
		// GPIO18がLOW(=ボタンが押された)ならLED点灯
		if(digitalRead(18) == LOW){
			digitalWrite(16,LOW);
		}
		else{
			digitalWrite(16,HIGH);
		}
	}

	return 0;
}