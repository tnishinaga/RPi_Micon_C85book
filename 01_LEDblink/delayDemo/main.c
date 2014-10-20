#include "rpi_lib/rpi.h"

#define GPFSEL4  0x20200010
#define GPSET1   0x20200020
#define GPCLR1   0x2020002C

#define SYST_CLO	0x20003004
#define SYST_CHI	0x20003008


volatile unsigned long long get_systime(void){
	unsigned long long t;
	unsigned int chi;
	unsigned int clo;

	// カウンタの値を取得
	chi = *(volatile unsigned int *)SYST_CHI;
	clo = *(volatile unsigned int *)SYST_CLO;

	// 桁上りチェック
	if(chi !=  *(volatile unsigned int *)SYST_CHI){
		// 桁上りが起こっているならCHIとCLOを更新する
		chi = *(volatile unsigned int *)SYST_CHI;
		clo = *(volatile unsigned int *)SYST_CLO;
	}

	// 64bitに組み直す
	t = chi;
	t = t << 32;
	t += clo;

	return t;
}


void delay_ms(unsigned int delay){
	unsigned long long alermTime;
	alermTime = get_systime() + delay * 1000;
	
	while(get_systime() < alermTime);

	return;
}

int main(void){
	rpi_init();

	// システムタイマーを初期化
	*(volatile unsigned int *)SYST_CHI = 0;
	*(volatile unsigned int *)SYST_CLO = 0;

	// GPIO47をOutputにセット
	*(volatile unsigned int *)GPFSEL4 |= 0x01 << (3*7);

	while(1){
		// wait 3000ms
		delay_ms(3000);
		// GPIO47をLにセット
		*(volatile unsigned int *)GPSET1 |= 0x01 << 15;
	}

	return 0;
}