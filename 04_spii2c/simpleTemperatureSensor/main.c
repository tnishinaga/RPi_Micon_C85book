#include "rpi_lib/rpi.h"
#include <stdio.h>
#include <stdint.h>

// device
#include "rpi_lib/device/ADT7310/ADT7310.h"
#include "rpi_lib/device/AQM0802/AQM0802.h"

int main(void){
	// オ(0xB5),ン(0xDD),ド(0xC4 + 0xDE), \0(0x00)
	uint8_t tempStr[] = {0xB5,0xDD,0xC4,0xDE,0x00};
	uint8_t tempVal[10];
	float temp;

	rpi_init();

	// SPI initialize
	SPI0_begin(SPI_SS_CS0);
	SPI0_setDataMode(SPI_MODE3);

	// I2C initialize
	Wire1_begin();

	// device initialize
	aqm0802_init();
	adt7310_initialize();

	while(1){
		// 画面クリア
		aqm0802_cleardisplay();

		// 温度取得
		temp = adt7310_getTemperature(ADT7310_MODE_CONRINUOUS);

		// 温度表示
		sprintf(tempVal,"%4.1f dec",temp);
		aqm0802_print_line(tempStr, 1);
		aqm0802_print_line(tempVal, 2);

		// wait 3sec
		delay(3000);
	}

	SPI0_end();

	return 0;
}
