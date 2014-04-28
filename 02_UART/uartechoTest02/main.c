#include "rpi_lib/rpi.h"

// UART peripheral
#define UART0_BASE		0x00201000
#define UART0_DR		((vu32_t *)PHY_PERI_ADDR(UART0_BASE + 0x00))
#define UART0_FR		((vu32_t *)PHY_PERI_ADDR(UART0_BASE + 0x18))
#define UART0_IBRD		((vu32_t *)PHY_PERI_ADDR(UART0_BASE + 0x24))
#define UART0_FBRD		((vu32_t *)PHY_PERI_ADDR(UART0_BASE + 0x28))
#define UART0_LCRH		((vu32_t *)PHY_PERI_ADDR(UART0_BASE + 0x2c))
#define UART0_CR		((vu32_t *)PHY_PERI_ADDR(UART0_BASE + 0x30))

int main(void){
	rpi_init();

	/****	初期設定開始	***/
	// UART無効化
	*UART0_CR 	= 0;
	
	//ポートの設定
	pinMode(14,INPUT_PULLDOWN);
	pinMode(15,INPUT_PULLDOWN);
	pinMode(14,ALT0);
	pinMode(15,ALT0);

	// ボーレートの設定
	//(3000000 / (16 * 115200) = 1.627
	//(0.627*64)+0.5 = 40
	// IBRD 1 FBRD 40
	*UART0_IBRD = 1;
	*UART0_FBRD = 40;
	
	// LCRH
	// stick parity dis, 8bit, FIFO en, two stop bit no, odd parity, parity dis, break no
	*UART0_LCRH = 0x70;

	// CR
	// CTS dis, RTS dis, OUT1-2=0, RTS dis, DTR dis, RXE en, TXE en, loop back dis, SIRLP=0, SIREN=0, UARTEN en
	*UART0_CR 	= 0x0301;
	/****	初期設定終了	***/

	char echo[256];
	int i,c,length;
	while(1){
		length = 0;

		
        // 255文字以下で改行（LF）が来るまで読み込む
        // （255文字以下の文字列を読み込む）
		do{
			// 受信FIFOにデータが来るのを待つ
			while (*UART0_FR & (1 << 4));
			// 受信データを読み込む
			c = *UART0_DR;
			echo[length] = c & 0xff;
			length++;
		}while(c != '\n' && length < 256);

		// 受け取った文字列を返信
		for(i=0;i<length;i++){
			// FIFOが空くまで待つ
			while(*UART0_FR & (0x01 << 5));
			// 1文字ずつ送る
			*UART0_DR = echo[i];
		}
	}
	
	return 0;
}
