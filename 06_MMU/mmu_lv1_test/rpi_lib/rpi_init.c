#include "rpi.h"

void rpi_init(void){
	// bssのクリア
	clearBss();

	// 割り込み不許可
	disable_all_IRQ();

	// ベクタテーブルセット
	set_vector_table();

	// システムタイマーを初期化
	init_syst();
	// GPIOをすべてINPUT_PULLUPに設定
	init_gpio();

	// 起動確認用
	pinMode(16,OUTPUT);
	digitalWrite(16, LOW);

	// UARTを有効
	Serial_begin(115200);
}