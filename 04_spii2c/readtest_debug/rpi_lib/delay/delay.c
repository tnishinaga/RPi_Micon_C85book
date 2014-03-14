#include "../timer/rpi_timer.h"
#include "../peripherals/rpi_peripherals.h"
#include "../rpi_type.h"


void delayMicroseconds(unsigned int us){
	unsigned long long int alermTime;
	alermTime = get_systime() + us;
	
	while(get_systime() < alermTime);

	return;
}

void delay(unsigned int ms){
	unsigned long long int alermTime;
	alermTime = get_systime() + ms * 1000;
	
	while(get_systime() < alermTime);

	return;
}
