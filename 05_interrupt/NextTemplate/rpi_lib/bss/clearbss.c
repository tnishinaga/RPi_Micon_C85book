void clearBss(void){
	extern void *__bss_start;
	extern void *__bss_end;

	unsigned int *p;
	unsigned int *start = (unsigned int *)&__bss_start;
	unsigned int *end = (unsigned int *)&__bss_end;

	for(p = start;p < end; p++){
		*p = 0x00;
	}
}
