// init
void init_syst(void);

// systime
unsigned long long int get_systime(void);

#define mills()		(get_systime()/1000)	
#define micros()	get_systime()