#ifndef RPILIB_I2C_H
#define RPILIB_I2C_H

// include
#include <stdint.h>
#include "../rpi_type.h"
#include "../peripherals/rpi_peripherals.h"
#include "../gpio/rpi_gpio.h"

// I2C(BSC) Concrol MASK(use OR MASK)
#define BSC_C_I2CEN_EN		((vu32_t)0x01 << 15)
#define BSC_C_ST_EN			((vu32_t)0x01 << 7)
#define BSC_C_CLEAR_ALL		((vu32_t)0x03 << 4)
#define BSC_C_READ_READ		((vu32_t)0x01)

// I2C(BSC) Status MASK
#define BSC_S_CLKT			((vu32_t)0x01 << 9)
#define BSC_S_ERR			((vu32_t)0x01 << 8)
#define BSC_S_RXF			((vu32_t)0x01 << 7)
#define BSC_S_TXE			((vu32_t)0x01 << 6)
#define BSC_S_RXD			((vu32_t)0x01 << 5)
#define BSC_S_TXD			((vu32_t)0x01 << 4)
#define BSC_S_RXR			((vu32_t)0x01 << 3)
#define BSC_S_TXW			((vu32_t)0x01 << 2)
#define BSC_S_DONE			((vu32_t)0x01 << 1)
#define BSC_S_TA			((vu32_t)0x01 << 0)

// clock divider
#define BSC_DIV_100KHZ		(0x5dc)
#define BSC_DIV_400KHZ		(0x177)
#define BSC_DIV_MINIMUM		(0)

// buffer length
#define BUFFER_LENGTH 		64

// FIFO length
#define FIFO_LENGTH			16

// functions
void Wire1_setSpeed(uint16_t div);
void Wire1_begin(void);
uint32_t Wire1_requestFrom(uint8_t address, uint32_t count);
void Wire1_beginTransmission(uint8_t address);
uint8_t Wire1_endTransmission(int mode);
uint32_t Wire1_writeByte(uint8_t value);
uint32_t Wire1_writeString(uint8_t *string);
uint32_t Wire1_writeData(uint8_t *data, uint32_t quantity);
uint32_t Wire1_available(void);
uint8_t Wire1_read(void);

#define Wire1_write 		Wire1_writeByte

#endif