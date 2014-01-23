#include "rpi_lib/rpi.h"

int main(void){
	rpi_init();

	// GPIO16をOutputにセット
	pinMode(16,OUTPUT);

	// JTAG用設定
	// 3.3V			: ARM_VREF 
	// GPIO22 (ALT4): ARM_TRST
	// GPIO4  (ALT5): ARM_TDI
	// GPIO27 (ALT4): ARM_TMS
	// GPIO25 (ALT4): ARM_TCK
	// GPIO24 (ALT4): ARM_TDO
	// GND			: ARM_GND
	pinMode(22, ALT4);
	pinMode(4, ALT5);
	pinMode(27, ALT4);
	pinMode(25, ALT4);
	pinMode(24, ALT4);

	// GPIO16をHにセット
	digitalWrite(16, HIGH);

	// GPIO16をLにセット
	digitalWrite(16, LOW);

	while(1);
	return 0;
}