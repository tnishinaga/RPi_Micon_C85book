#include "rpi.h"

void rpi_init(void){
	clearBss();
	// システムタイマーを初期化
	init_syst();
	// GPIOをすべてINPUT_PULLUPに設定
	init_gpio();

	// 起動確認用
	pinMode(16,OUTPUT);
	digitalWrite(16, LOW);
}