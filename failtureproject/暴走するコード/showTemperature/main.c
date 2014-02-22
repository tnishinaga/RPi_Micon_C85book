#include "rpi_lib/rpi.h"
#include <stdio.h>
#include <stdint.h>

// SPI0 peripheral
#define SPI0_BASE		(0x00204000)
#define SPI0_CS			((vu32_t *)PHY_PERI_ADDR(SPI0_BASE + 0x00))
#define SPI0_FIFO		((vu32_t *)PHY_PERI_ADDR(SPI0_BASE + 0x04))
#define SPI0_CLK		((vu32_t *)PHY_PERI_ADDR(SPI0_BASE + 0x08))
#define SPI0_DLEN		((vu32_t *)PHY_PERI_ADDR(SPI0_BASE + 0x0C))
#define SPI0_LTOH		((vu32_t *)PHY_PERI_ADDR(SPI0_BASE + 0x10))
#define SPI0_DC			((vu32_t *)PHY_PERI_ADDR(SPI0_BASE + 0x14))

// SPI0 CS MASK(use OR MASK)
#define SPI0_CS_CSPOL0_HIGH	((vu32_t)0x01 << 21)
#define SPI0_CS_CSPOL0_LOW	((vu32_t)0x00 << 21)
#define SPI0_CS_TA_ACTIVE	((vu32_t)0x01 << 7)
#define SPI0_CS_CSPOL_HIGH	((vu32_t)0x01 << 6)
#define SPI0_CS_CSPOL_LOW	((vu32_t)0x00 << 6)
#define SPI0_CS_CLEAR_TX	((vu32_t)0x01 << 4)
#define SPI0_CS_CLEAR_RX	((vu32_t)0x02 << 4)
#define SPI0_CS_CLEAR_ALL	(SPI0_CS_CLEAR_TX | SPI0_CS_CLEAR_RX)
#define SPI0_CS_CS_CS0		((vu32_t)0x00)
#define SPI0_CS_CS_CS1		((vu32_t)0x01)
#define SPI0_CS_CS_CS2		((vu32_t)0x10)
// SPI0 MODE
#define SPI0_CS_MODE_0x		((vu32_t)0x00 << 3)
#define SPI0_CS_MODE_1x		((vu32_t)0x01 << 3)
#define SPI0_CS_MODE_x0		((vu32_t)0x00 << 2)
#define SPI0_CS_MODE_x1		((vu32_t)0x01 << 2)
#define SPI0_CS_MODE_00		(SPI0_CS_MODE_0x | SPI0_CS_MODE_x0)	///< CPOL = 0, CPHA = 0
#define SPI0_CS_MODE_01		(SPI0_CS_MODE_0x | SPI0_CS_MODE_x1) ///< CPOL = 0, CPHA = 1
#define SPI0_CS_MODE_10		(SPI0_CS_MODE_1x | SPI0_CS_MODE_x0)	///< CPOL = 1, CPHA = 0
#define SPI0_CS_MODE_11		(SPI0_CS_MODE_1x | SPI0_CS_MODE_x1)	///< CPOL = 1, CPHA = 1
// FLAGs
#define SPI0_CS_RXF_MASK	((vu32_t)0x01 << 20)
#define SPI0_CS_RXR_MASK	((vu32_t)0x01 << 19)
#define SPI0_CS_TXD_MASK	((vu32_t)0x01 << 18)
#define SPI0_CS_RXD_MASK	((vu32_t)0x01 << 17)
#define SPI0_CS_DONE_MASK	((vu32_t)0x01 << 16)


// temperature device
#define ADT7310_CMD_REG_CONFIG  (0x01 << 3)
#define ADT7310_CMD_REG_TEMP	(0x02 << 3)
#define ADT7310_CMD_READ		(0x01 << 6)
#define ADT7310_CMD_WRITE		(0x00 << 6)


int main(void){
	uint32_t	rawtemp;
	double		temp;

	rpi_init();

	// debug
	pinMode(22, ALT4);
	pinMode(4, ALT5);
	pinMode(27, ALT4);
	pinMode(25, ALT4);
	pinMode(24, ALT4);
	
	// pin setting
	// SS
	pinMode( 8, ALT0);
	// MISO
	pinMode( 9, ALT0);
	// MOSI
	pinMode(10, ALT0);
	// SCLK
	pinMode(11, ALT0);

	// CS dis
	// digitalWrite(8, HIGH);

	printf("start\n");


	// SPI0 configure
	*SPI0_CS = 0;
	// CSPOL0=HIGH, TA=0, CS=CS0
	*SPI0_CS |= SPI0_CS_CSPOL0_LOW | SPI0_CS_CS_CS0;
	// Buffer Clear
	*SPI0_CS |= SPI0_CS_CLEAR_ALL;

	// Clock setting
	// 3.81[kHz]
	*SPI0_CLK = 0;
	// MODE setting
	// MODE00
	*SPI0_CS |= SPI0_CS_MODE_00;

	// Configuretionレジスタ設定
	// TA = 1
	*SPI0_CS |= SPI0_CS_TA_ACTIVE;

	// CS en
	// digitalWrite(8, LOW);

	// wait TXD
	while(!(*SPI0_CS & SPI0_CS_TXD_MASK));

	printf("pass TXD check\n");

	// write Command Byte
	// Resister address=0x01, Write mode
	*SPI0_FIFO = ADT7310_CMD_REG_CONFIG | ADT7310_CMD_WRITE;
	// set Configuretion resister
	// Resolution=16bit, Oparation mode=standard
	*SPI0_FIFO = 0x80;
	// 送信完了待ち
	while(!(*SPI0_CS & SPI0_CS_DONE_MASK));

	printf("pass DONE check\n");

	// TA = 0
	*SPI0_CS &= ~SPI0_CS_TA_ACTIVE;
	// CS dis
	// digitalWrite(8, HIGH);


	while(1){
		// init 
		rawtemp = 0;
		temp = 0.0;

		// start get temperature
		// TA = 1
		*SPI0_CS |= SPI0_CS_TA_ACTIVE;
		// CS en
		// digitalWrite(8, LOW);
		// write Command Byte
		// Resister address=0x02, Read mode
		*SPI0_FIFO = ADT7310_CMD_REG_TEMP | ADT7310_CMD_READ;
		*SPI0_FIFO = 0;
		*SPI0_FIFO = 0;

		// 受信待ち
		while(!(*SPI0_CS & SPI0_CS_RXD_MASK));
		// 上位8bit受信
		rawtemp = *SPI0_FIFO;
		rawtemp <<= 8;
		// 受信待ち
		while(!(*SPI0_CS & SPI0_CS_RXD_MASK));
		rawtemp |= *SPI0_FIFO;

		// 転送完了待ち
		while(!(*SPI0_CS & SPI0_CS_DONE_MASK));
		// TA = 0
		*SPI0_CS &= 0xffffffff - SPI0_CS_TA_ACTIVE;
		// CS dis
		// digitalWrite(8, HIGH);
		// end get temparature

		// calc temperature
		if(rawtemp & 0x8000){
			// MSBが1ならネガティブ
			temp = (rawtemp - 32768) / 128.0;
		}
		else{
			// MSBが0ならポジティブ
			temp = rawtemp / 128.0;
		}

		printf("Temparature register : 0x%04x\n",rawtemp);
		printf("Temparature : %.3lf (dec)\n",temp);

		// wait 3sec
		delay(3000);
	}


	return 0;
}
