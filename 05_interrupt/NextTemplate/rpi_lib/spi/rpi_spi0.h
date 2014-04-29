#ifndef RPILIB_SPI0_H
#define RPILIB_SPI0_H

// include
#include <stdint.h>
#include "../rpi_type.h"
#include "../peripherals/rpi_peripherals.h"
#include "../gpio/rpi_gpio.h"


// SPI0 CS MASK(use OR MASK)
#define SPI0_CS_CSPOL2_HIGH		((vu32_t)0x01 << 23)
#define SPI0_CS_CSPOL1_HIGH		((vu32_t)0x01 << 22)
#define SPI0_CS_CSPOL0_HIGH		((vu32_t)0x01 << 21)
#define SPI0_CS_TA_ACTIVE		((vu32_t)0x01 << 7)
#define SPI0_CS_CSPOL_HIGH		((vu32_t)0x01 << 6)
#define SPI0_CS_CSPOL_LOW		((vu32_t)0x00 << 6)
#define SPI0_CS_CLEAR_TX		((vu32_t)0x01 << 4)
#define SPI0_CS_CLEAR_RX		((vu32_t)0x02 << 4)
#define SPI0_CS_CLEAR_ALL		(SPI0_CS_CLEAR_TX | SPI0_CS_CLEAR_RX)
#define SPI0_CS_CS_CS0			((vu32_t)0x00)
#define SPI0_CS_CS_CS1			((vu32_t)0x01)
#define SPI0_CS_CS_CS2			((vu32_t)0x10)
// SPI0 MODE
#define SPI0_CS_MODE_0x			((vu32_t)0x00 << 3) // CPOL = 0
#define SPI0_CS_MODE_1x			((vu32_t)0x01 << 3) // CPOL = 1
#define SPI0_CS_MODE_x0			((vu32_t)0x00 << 2) // CPHA = 0
#define SPI0_CS_MODE_x1			((vu32_t)0x01 << 2) // CPHA = 1
#define SPI0_CS_MODE_00			(SPI0_CS_MODE_0x | SPI0_CS_MODE_x0)	
#define SPI0_CS_MODE_01			(SPI0_CS_MODE_0x | SPI0_CS_MODE_x1) 
#define SPI0_CS_MODE_10			(SPI0_CS_MODE_1x | SPI0_CS_MODE_x0)	
#define SPI0_CS_MODE_11			(SPI0_CS_MODE_1x | SPI0_CS_MODE_x1)	
// FLAGs
#define SPI0_CS_RXF_MASK		((vu32_t)0x01 << 20)
#define SPI0_CS_RXR_MASK		((vu32_t)0x01 << 19)
#define SPI0_CS_TXD_MASK		((vu32_t)0x01 << 18)
#define SPI0_CS_RXD_MASK		((vu32_t)0x01 << 17)
#define SPI0_CS_DONE_MASK		((vu32_t)0x01 << 16)
// DMA or LoSSI
#define SPI0_CS_LEN_LONG_32BIT	((vu32_t)0x01 << 25)
#define SPI0_CS_DMA_LEN_EN		((vu32_t)0x01 << 24)
#define SPI0_CS_LEN_EN			((vu32_t)0x01 << 12)
#define SPI0_CS_ADCS_EN			((vu32_t)0x01 << 11)
#define SPI0_CS_DMAEN_EN		((vu32_t)0x01 << 8)
// interrupt
#define SPI0_CS_INTR			((vu32_t)0x01 << 10)
#define SPI0_CS_INTD			((vu32_t)0x01 << 9)

// SPI0 DC
#define SPI0_DC_RPANIC(x)	((x) << 24)
#define SPI0_DC_RDREQ(x)	((x) << 16)
#define SPI0_DC_TPANIC(x)	((x) << 8)
#define SPI0_DC_TDREQ(x)	(x)


// Arduino SPI MODE
#define SPI_MODE0			0
#define SPI_MODE1			1
#define SPI_MODE2			2
#define SPI_MODE3			3

// SS setting
#define SPI_SS_CS0		(0)
#define SPI_SS_CS1		(1)
#define SPI_SS_CS2		(2)
#define SPI_SS_OTHER	(-1)

#define SPI_SS_PORT_CS0	(8)
#define SPI_SS_PORT_CS1	(7)
#define SPI_SS_MODE_CS0	(ALT0)
#define SPI_SS_MODE_CS1	(ALT0)

// divider setting
#define SPI_CLOCK_DIV2			(2)
#define SPI_CLOCK_DIV4			(4)
#define SPI_CLOCK_DIV8			(8)
#define SPI_CLOCK_DIV16			(16)
#define SPI_CLOCK_DIV32			(32)
#define SPI_CLOCK_DIV64			(64)
#define SPI_CLOCK_DIV128		(128)
#define SPI_CLOCK_DIV256		(256)
#define SPI_CLOCK_DIV512		(512)
#define SPI_CLOCK_DIV1024		(1024)
#define SPI_CLOCK_DIV2048		(2048)
#define SPI_CLOCK_DIV4096		(4096)
#define SPI_CLOCK_DIV8192		(8192)
#define SPI_CLOCK_DIV16384		(16384)
#define SPI_CLOCK_DIV32768		(32768)
#define SPI_CLOCK_DIV65536		(0)

// bit order
#define SPI_BIT_ORDER_MSBFIRST		(0)
#define SPI_BIT_ORDER_LSBFIRST		(1)

// functions
void SPI0_begin(int ss_port);
void SPI0_end(void);
void SPI0_setBitOrder(int order);
void SPI0_setClockDivider(uint16_t divider);
void SPI0_setDataMode(uint8_t mode);
uint8_t SPI0_transfer(uint8_t value);

#endif