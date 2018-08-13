#include "rpi_gpio.h"
#include "../peripherals/rpi_peripherals.h"
#include "../rpi_type.h"
#include "../delay/rpi_delay.h"

void init_gpio(void){
	int i;
	// pullup all
	*GPPUD 	= 0x02;
	// wait 150 cycle
	for(i=0;i<150;i++){
		// nop
		asm("mov r0,r0");
	}
	*GPPUDCLK0 = 0xffffffff;
	*GPPUDCLK1 = 0xffffffff;
	// wait 150 cycle
	for(i=0;i<150;i++){
		// nop
		asm("mov r0,r0");
	}
	*GPPUDCLK0 = 0;
	*GPPUDCLK1 = 0;

	*GPFSEL0 = 0;
	*GPFSEL1 = 0;
	*GPFSEL2 = 0;
	*GPFSEL3 = 0;
	*GPFSEL4 = 0;
	*GPFSEL5 = 0;
}

void setPullUpDown(int pin,int mode){
	int i;
	vu32_t *res;

	if(pin <= 32){
		res = GPPUDCLK0;
	}
	else{
		res = GPPUDCLK1;
	}
	// set PULLUP
	if(mode == INPUT_PULLUP){
		*GPPUD 	= 0x02;
	}else if (mode == INPUT_PULLDOWN){
		*GPPUD 	= 0x01;
	}else{
		return;
	}
	// wait 150 cycle
	for(i=0;i<150;i++){
		// nop
		asm("mov r0,r0");
	}
	*res = (0x01 << (pin % 32));
	// wait 150 cycle
	for(i=0;i<150;i++){
		// nop
		asm("mov r0,r0");
	}
	*res = 0;
	return;
}


void pinMode(int pin,int mode){
	vu32_t *res;

	// GPFSEL select
	if(0 <= pin && pin <= 9){
		res = GPFSEL0;
	}else if (pin <= 19)
	{
		res = GPFSEL1;
	}else if (pin <= 29)
	{
		res = GPFSEL2;
	}else if (pin <= 39)
	{
		res = GPFSEL3;
	}else if (pin <= 49)
	{
		res = GPFSEL4;
	}else if (pin <= 53)
	{
		res = GPFSEL5;
	}else{
		// pin missmuch
		return;
	}

	// mode set
	switch(mode){
		case INPUT:
			*res &= GPFSEL_MASK_IN(pin);
			break;
		case INPUT_PULLUP:
			setPullUpDown(pin,INPUT_PULLUP);
			*res &= GPFSEL_MASK_IN(pin);
			break;
		case INPUT_PULLDOWN:
			setPullUpDown(pin,INPUT_PULLDOWN);
			*res &= GPFSEL_MASK_IN(pin);
			break;
		case OUTPUT:
			*res |= GPFSEL_MASK_OUT(pin);
			break;
		case ALT0:
			*res |= GPFSEL_MASK_ALT0(pin);
			break;
		case ALT1:
			*res |= GPFSEL_MASK_ALT1(pin);
			break;
		case ALT2:
			*res |= GPFSEL_MASK_ALT2(pin);
			break;
		case ALT3:
			*res |= GPFSEL_MASK_ALT3(pin);
			break;
		case ALT4:
			*res |= GPFSEL_MASK_ALT4(pin);
			break;
		case ALT5:
			*res |= GPFSEL_MASK_ALT5(pin);
			break;
		default:
			// error!
			;
	}
	return;
}


void digitalWrite(int pin, int value){
	vu32_t *res;

	if(value == HIGH){
		// GPSET select
		if(0 <= pin && pin <= 32){
			res = GPSET0;
		}else if (pin <= 53)
		{
			res = GPSET1;
		}else{
			// pin missmuch
			return;
		}
	}
	else if(value == LOW){
		// GPCLR select
		if(0 <= pin && pin <= 32){
			res = GPCLR0;
		}else if (pin <= 53)
		{
			res = GPCLR1;
		}else{
			// pin missmuch
			return;
		}
	}
	else{
		return;
	}
	// output
	*res = (0x01 << (pin % 32));
	return;
}


int digitalRead(int pin){
	unsigned int val;
	unsigned int mask;

	// GPCLR select
	if(0 <= pin && pin <= 32){
		val = *GPLEV0;
	}else if (pin <= 53)
	{
		val = *GPLEV1;
	}else{
		// pin missmuch
		return -1;
	}

	// input
	mask = 0x01 << (pin % 32);
	if((val & mask) != 0){
		return HIGH;
	}
	return LOW;
}