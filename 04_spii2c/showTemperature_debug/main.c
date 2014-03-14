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
#define SPI0_CS_MODE_0x		((vu32_t)0x00 << 3) // CPOL = 0
#define SPI0_CS_MODE_1x		((vu32_t)0x01 << 3) // CPOL = 1
#define SPI0_CS_MODE_x0		((vu32_t)0x00 << 2) // CPHA = 0
#define SPI0_CS_MODE_x1		((vu32_t)0x01 << 2) // CPHA = 1
#define SPI0_CS_MODE_00		(SPI0_CS_MODE_0x | SPI0_CS_MODE_x0)	
#define SPI0_CS_MODE_01		(SPI0_CS_MODE_0x | SPI0_CS_MODE_x1) 
#define SPI0_CS_MODE_10		(SPI0_CS_MODE_1x | SPI0_CS_MODE_x0)	
#define SPI0_CS_MODE_11		(SPI0_CS_MODE_1x | SPI0_CS_MODE_x1)	
// FLAGs
#define SPI0_CS_RXF_MASK	((vu32_t)0x01 << 20)
#define SPI0_CS_RXR_MASK	((vu32_t)0x01 << 19)
#define SPI0_CS_TXD_MASK	((vu32_t)0x01 << 18)
#define SPI0_CS_RXD_MASK	((vu32_t)0x01 << 17)
#define SPI0_CS_DONE_MASK	((vu32_t)0x01 << 16)


// temperature device
#define ADT7310_CMD_ADDR_STATUS  	(0x00 << 3)
#define ADT7310_CMD_ADDR_CONFIG  	(0x01 << 3)
#define ADT7310_CMD_ADDR_TEMP		(0x02 << 3)

#define ADT7310_CMD_READ			(0x01 << 6)
#define ADT7310_CMD_WRITE			(0x00 << 6)
#define ADT7310_CMD_CONRINUOUS 		(0x01 << 2)


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


	// SPI0 configure
	*SPI0_CS = 0;
	// CSPOL0=HIGH, TA=0, CS=CS0
	*SPI0_CS |= SPI0_CS_CSPOL0_LOW | SPI0_CS_CS_CS0;
	// 送受信バッファ初期化
	*SPI0_CS |= SPI0_CS_CLEAR_ALL;
	// Clock setting
	// 3.81[kHz]
	*SPI0_CLK = 0;
	// MODE10
	*SPI0_CS |= SPI0_CS_MODE_11;


	// software reset
	// 通信開始（TA = 1）
	*SPI0_CS |= SPI0_CS_TA_ACTIVE;
	// 送信可能待ち（wait TXD）
	while(!(*SPI0_CS & SPI0_CS_TXD_MASK));
	// ソフトウェアリセット（32bit連続1書き込み）
	*SPI0_FIFO = 0xFF;
	*SPI0_FIFO = 0xFF;
	*SPI0_FIFO = 0xFF;
	*SPI0_FIFO = 0xFF;
	// 送信完了待ち
	while(!(*SPI0_CS & SPI0_CS_DONE_MASK));
	// 通信終了（TA = 0）
	*SPI0_CS &= ~SPI0_CS_TA_ACTIVE;

	delay(50);

	// Configuretionレジスタ設定
	// 通信開始（TA = 1）
	*SPI0_CS |= SPI0_CS_TA_ACTIVE;
	// 送信可能待ち（wait TXD）
	while(!(*SPI0_CS & SPI0_CS_TXD_MASK));
	// Command Byte送信
	// Resister address=0x01, Write mode
	*SPI0_FIFO = ADT7310_CMD_ADDR_CONFIG | ADT7310_CMD_WRITE;
	// Configuretion resisterをセット
	// Resolution=16bit, Oparation mode=standard
	*SPI0_FIFO = 0x80;
	// 送信完了待ち
	while(!(*SPI0_CS & SPI0_CS_DONE_MASK));
	// 通信終了（TA = 0）
	*SPI0_CS &= ~SPI0_CS_TA_ACTIVE;

	delay(500);

	// 通信開始（TA = 1）
	*SPI0_CS |= SPI0_CS_TA_ACTIVE;
	// Command Byte送信
	// Resister address=0x02, Read mode, Conrinuousモード
	*SPI0_FIFO = ADT7310_CMD_ADDR_TEMP | ADT7310_CMD_READ | ADT7310_CMD_CONRINUOUS;
	// 転送完了待ち
	while(!(*SPI0_CS & SPI0_CS_DONE_MASK));
	// 通信終了（TA = 0）
	*SPI0_CS &= ~SPI0_CS_TA_ACTIVE;

	delay(500);

	while(1){
		// init 
		rawtemp = 0;
		temp = 0.0;
		// 送受信バッファ初期化
		*SPI0_CS |= SPI0_CS_CLEAR_ALL;

		// start get temperature
		// 通信開始（TA = 1）
		*SPI0_CS |= SPI0_CS_TA_ACTIVE;
		// 温度読み込み（Conrinuousモード）
		*SPI0_FIFO = 0;
		*SPI0_FIFO = 0;

		// 受信待ち
		while(!(*SPI0_CS & SPI0_CS_RXD_MASK));
		// 上位8bit受信
		rawtemp = *SPI0_FIFO;
		rawtemp = rawtemp << 8;
		// 受信待ち
		while(!(*SPI0_CS & SPI0_CS_RXD_MASK));
		// 下位8bit受信
		rawtemp |= *SPI0_FIFO;

		// 転送完了待ち
		while(!(*SPI0_CS & SPI0_CS_DONE_MASK));

		// 通信終了（TA = 0）
		*SPI0_CS &= ~SPI0_CS_TA_ACTIVE;
		// end get temperature

		// 温度計算
		if(rawtemp & 0x8000){
			// MSBが1ならネガティブ
			temp = (rawtemp - 32768) / 128.0;
		}
		else{
			// MSBが0ならポジティブ
			temp = rawtemp / 128.0;
		}

		// temperetureレジスタ値表示
		printf("temperature register : 0x%04x\n",rawtemp);
		// 温度表示
		printf("temperature : %f (dec)\n",temp);

		// wait 3sec
		delay(3000);
	}


	return 0;
}
