@ startup
	.global _start
	.align
_start:
	ldr	r0, =0x000000d3
	msr	cpsr, r0
	ldr	sp, =0x06400000
	bl main
