#ifndef RPILIB_TIMER_H
#define RPILIB_TIMER_H
 
// init
void init_syst(void);

// systime
unsigned long long int get_systime(void);

#define mills()		(get_systime()/1000)	
#define micros()	get_systime()

// タイマー設定用マスク（ORマスクで利用してください）
#define	TMR_DBG_STOP	(0x00000100)
#define TMR_EN			(0x00000080)
#define TMR_INT_EN		(0x00000020)
#define TMR_BIT_32		(0x00000002)


void Timer_initialize(unsigned int period);
void Timer_setPeriod(unsigned int period);
void Timer_start(void);
void Timer_stop(void);
unsigned int Timer_read(void);
void Timer_attachInterrupt(void (*f)(void));
void Timer_dettachInterrupt(void);

// タイマー回り込み処理用関数ポインタ
extern void (*timerIRQ_func)(void);

#endif
