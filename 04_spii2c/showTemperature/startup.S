@ startup
	.global _start
	.align
_start:
	// SVCモードのスタックポインタ設定
	// IRQ,FIQ disable, Thumb disable, SVC mode 
	ldr	r0, =0x000000d3
	msr	cpsr, r0
	ldr	sp, =0x06400000

	// main呼び出し
	bl main

	// 終了（無限ループ）
	b .