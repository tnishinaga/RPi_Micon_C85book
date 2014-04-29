#ifndef RPILIB_DEVICE_ADT7310_H
#define RPILIB_DEVICE_ADT7310_H

// spi
#include "../../spi/rpi_spi0.h"

// temperature device
#define ADT7310_CMD_ADDR_STATUS  	(0x00 << 3)
#define ADT7310_CMD_ADDR_CONFIG  	(0x01 << 3)
#define ADT7310_CMD_ADDR_TEMP		(0x02 << 3)

#define ADT7310_CMD_READ			(0x01 << 6)
#define ADT7310_CMD_WRITE			(0x00 << 6)
#define ADT7310_CMD_CONRINUOUS 		(0x01 << 2)

#define ADT7310_MODE_ONESHOT		(0)
#define ADT7310_MODE_1SPS			(1)
#define ADT7310_MODE_CONRINUOUS		(2)

// functions
void adt7310_initialize(void);
float adt7310_getTemperature(int mode);

#endif
