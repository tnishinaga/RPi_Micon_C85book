#include "rpi_lib/rpi.h"
#include <stdio.h>
#include <stdint.h>

extern void *__bss_end;
uint32_t bssend = (uint32_t)&__bss_end;
register char * stack_ptr asm ("sp");

int main(void){
	rpi_init();

	uint32_t i=0;
	int32_t *mem;
	int32_t *prev_mem;

	delay(1000);

	printf("BSS end: 0x%08x\n",bssend);
	printf("Stack ptr: 0x%08x\n",stack_ptr);

	printf("malloc test start\n");

	do{
		prev_mem = mem;
		i += 10;
		// get 10MB(= 10 * 1024KB = 10 * 1024 * 1024 Byte)
		// 32bit aligned
		mem = (int32_t *)malloc(10*1024*1024);
		printf("mem: 0x%08x\n",mem);
	}while(mem != NULL);

	free(prev_mem);
	printf("malloc faild in %d MB\n",i);

	while(1);
	
	return 0;
}
