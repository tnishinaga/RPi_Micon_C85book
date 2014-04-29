#include "rpi_timer.h"
#include "../peripherals/rpi_peripherals.h"
#include "../rpi_type.h"
#include "../rpi_sysconfig.h"

// Timer IRQ function
void (*timerIRQ_func)(void);


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


void enable_timer_interrupt(){
	// タイマ割り込み許可
	*INTERRUPT_ENABLE_BASIC_IRQS |= 0x01;
	*TIMER_CONTROL |= TMR_INT_EN;
}


void disable_timer_interrupt(){
	// タイマ割り込み不許可
	*INTERRUPT_ENABLE_BASIC_IRQS &= 0xfffffff1;
	*TIMER_CONTROL &= ~TMR_INT_EN;
}


int set_timer_clock(unsigned int clock){
	// 単位はHz
	if(clock > APB_CLOCK){
		// error
		return -1;
	}

	unsigned int prediv = APB_CLOCK / clock - 1;

	// printf("prediv = %d\n",prediv);
	// delay(10000);

	// pre-divider設定
	*TIMER_PREDIVIDER = prediv;

	return 0;
}


unsigned int get_timer_rawIRQ(void){
	return *TIMER_RAWIRQ;
}

unsigned int get_timer_maskedIRQ(void){
	return *TIMER_MASKEDIRQ;
}

void clear_timer_flag(void){
	*TIMER_IRQ_CLR = 0;
}

// Arduino風関数

void Timer_initialize(unsigned int period){
	// timer clock を1MHzに設定
	set_timer_clock(1000000);

	// Timer 32bit
	*TIMER_CONTROL |= 0x00000002;

	// period(us)
	Timer_stop();
	// Timer_start(period);
	Timer_setPeriod(period);
}

void Timer_setPeriod(unsigned int period){
	// period(us)
	*TIMER_RELOAD = period;
	*TIMER_LOAD = period;
}

void Timer_start(void){
	// Timer Start
	*TIMER_CONTROL |= TMR_EN;
}

void Timer_stop(void){
	// Timer stop
	*TIMER_CONTROL &= ~TMR_EN;
}

unsigned int Timer_read(void){
	return *TIMER_VALUE;
}

void Timer_attachInterrupt(void (*f)(void)){
	// 割り込みフラグクリア
	*TIMER_IRQ_CLR = 0;
	// 関数ポインタセット
	timerIRQ_func = f;

	// タイマー開始
	Timer_start();

	// 割り込み有効
	enable_timer_interrupt();
	enable_IRQ();
}

void Timer_dettachInterrupt(void){
	// disable interrupt
	disable_timer_interrupt();
}
