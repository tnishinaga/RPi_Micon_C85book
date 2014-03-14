#include "rpi_lib/rpi.h"
#include <stdio.h>
#include <stdint.h>
#include "rpi_lib/device/ADT7310/ADT7310.h"

int main(void){
	float temp;

	rpi_init();
	

	SPI0_begin(SPI_SS_CS0);
	SPI0_setDataMode(SPI_MODE3);

	adt7310_initialize();

	while(1){
		temp = adt7310_getTemperature(ADT7310_MODE_CONRINUOUS);

		// 温度表示
		printf("temperature : %f (dec)\n",temp);

		// wait 3sec
		delay(3000);
	}

	SPI0_end();

	return 0;
}
