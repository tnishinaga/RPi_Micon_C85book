#include "rpi_lib/rpi.h"
#include <stdio.h>
#include <stdint.h>


#define BSC1_BASE	(0x00804000)
#define BSC1_C		((vu32_t *)PHY_PERI_ADDR(BSC1_BASE + 0x00))
#define BSC1_S		((vu32_t *)PHY_PERI_ADDR(BSC1_BASE + 0x04))
#define BSC1_DLEN	((vu32_t *)PHY_PERI_ADDR(BSC1_BASE + 0x08))
#define BSC1_A		((vu32_t *)PHY_PERI_ADDR(BSC1_BASE + 0x0C))
#define BSC1_FIFO	((vu32_t *)PHY_PERI_ADDR(BSC1_BASE + 0x10))
#define BSC1_DIV	((vu32_t *)PHY_PERI_ADDR(BSC1_BASE + 0x14))
#define BSC1_DEL	((vu32_t *)PHY_PERI_ADDR(BSC1_BASE + 0x18))
#define BSC1_CLKT	((vu32_t *)PHY_PERI_ADDR(BSC1_BASE + 0x1C))

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

// LIS3DH
// 3-axis sensor
#define LIS3DH_ADDR	 (0x30)

//Register Addresses:
#define  WHO_AM_I  0x0F //Dummy register

int main(void){
	rpi_init();

	// debug
	pinMode(22, ALT4);
	pinMode(4, ALT5);
	pinMode(27, ALT4);
	pinMode(25, ALT4);
	pinMode(24, ALT4);

	// I2C init
	// port setting
	pinMode(2, ALT0); // SDA
	pinMode(3, ALT0); // SCL
	// set I2C clock divider
	// SCL = 150[MHz] / 32768 = 4.58[kHz]
	*BSC1_DIV = 0;

	while(1){
		///// スレーブアドレス＆レジスタ選択 /////
		// 送信準備
		// Clear Status
		*BSC1_S = BSC_S_CLKT | BSC_S_ERR | BSC_S_DONE;
		// slave address
		*BSC1_A = LIS3DH_ADDR >> 1;
		// Write mode
		*BSC1_C &= ~BSC_C_READ_READ;
		// Buffer clear
		*BSC1_C |= BSC_C_CLEAR_ALL;
		// 送信データ長設定
		*BSC1_DLEN = 1; 
		// set data
		*BSC1_FIFO = WHO_AM_I;
		// transfer start
		*BSC1_C |= BSC_C_I2CEN_EN | BSC_C_ST_EN;
		// 転送開始待ち
		// 1 = Transfer active
		while(!(*BSC1_S & BSC_S_TA));

		// 受信データ長設定
		*BSC1_DLEN = 1;
		// Read mode
		*BSC1_C |= BSC_C_READ_READ;

		// transfer start
		*BSC1_C |= BSC_C_ST_EN;
		// 転送終了待ち
		// 1 = Transfer complete
		while(!(*BSC1_S & BSC_S_DONE));
		// データ受信
		if(*BSC1_S & BSC_S_RXD){
			printf("WHO_AM_I = 0x%02x\n", *BSC1_FIFO);
		}

		// 3秒待ち
		delay(3000);
	}

	// I2C通信終了
	*BSC1_C &= ~BSC_C_I2CEN_EN;

	return 0;
}
