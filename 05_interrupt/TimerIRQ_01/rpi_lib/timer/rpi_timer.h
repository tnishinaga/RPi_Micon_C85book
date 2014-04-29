#ifndef RPILIB_TIMER_H
#define RPILIB_TIMER_H
 
// init
void init_syst(void);

// systime
unsigned long long int get_systime(void);

#define mills()		(get_systime()/1000)	
#define micros()	get_systime()
#endif
