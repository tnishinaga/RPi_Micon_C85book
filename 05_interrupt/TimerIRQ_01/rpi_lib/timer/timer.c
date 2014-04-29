#include "rpi_timer.h"
#include "../peripherals/rpi_peripherals.h"
#include "../rpi_type.h"

void init_syst(void){
	*SYST_CHI = 0;
	*SYST_CLO = 0;
}

unsigned long long int get_systime(void){
	unsigned long long int t;
	unsigned int chi;
	unsigned int clo;

	// カウンタの値を取得
	chi = *SYST_CHI;
	clo = *SYST_CLO;

	// 桁上りチェック
	if(chi !=  *SYST_CHI){
		// 桁上りが起こっているならCHIとCLOを更新する
		chi = *SYST_CHI;
		clo = *SYST_CLO;
	}

	// 64bitに組み直す
	t = chi;
	t = t << 32;
	t += clo;

	return t;
}
