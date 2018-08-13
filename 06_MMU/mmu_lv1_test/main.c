#include "rpi_lib/rpi.h"
#include <stdio.h>
#include <stdint.h>

// pagatable start address(16KiB align)
#define PAGETABLE_START_ADDR 0x4000;
volatile uint32_t *ttbr0_lv1_pagetable = (uint32_t *)PAGETABLE_START_ADDR;


#define VMSAv6_SECTION_BASEADDR_SHIFT	(20)
#define VMSAv6_SECTION_NS_SHIFT			(19)
#define VMSAv6_SECTION_NG_SHIFT			(17)
#define VMSAv6_SECTION_S_SHIFT			(16)
#define VMSAv6_SECTION_AP2_SHIFT		(15)
#define VMSAv6_SECTION_TEX_SHIFT		(12)
#define VMSAv6_SECTION_AP10_SHIFT		(10)
#define VMSAv6_SECTION_IMP_SHIFT		(9)
#define VMSAv6_SECTION_DOMAIN_SHIFT		(5)
#define VMSAv6_SECTION_XN_SHIFT			(4)
#define VMSAv6_SECTION_C_SHIFT			(3)
#define VMSAv6_SECTION_B_SHIFT			(2)

#define VMSAv6_SECTION_NONSECURE		(1 << VMSAv6_SECTION_NS_SHIFT)
#define VMSAv6_SECTION_SHAREABLE		(1 << VMSAv6_SECTION_S_SHIFT)
#define VMSAv6_SECTION_NONGROBAL		(1 << VMSAv6_SECTION_NG_SHIFT)
#define VMSAv6_SECTION_IMP				(1 << VMSAv6_SECTION_IMP_SHIFT)
#define VMSAv6_SECTION_NOEXECUTABLE	(1 << VMSAv6_SECTION_XN_SHIFT)
#define VMSAv6_SECTION_CACHEABLE	(1 << VMSAv6_SECTION_C_SHIFT)
#define VMSAv6_SECTION_BAFFURABLE	(1 << VMSAv6_SECTION_B_SHIFT)
#define VMSAv6_SECTION_FLAG			(0x02)
// ap
#define VMSAv6_SECTION_AP_FULLACCESS	( (0 << VMSAv6_SECTION_AP2_SHIFT) \
										| (3 << VMSAv6_SECTION_AP10_SHIFT))
// tex
// TODO: write
// domain
#define VMSAv6_SECTION_DOMAIN_NOACCESS	(0 << VMSAv6_SECTION_DOMAIN_SHIFT)
#define VMSAv6_SECTION_DOMAIN_CLIENT	(1 << VMSAv6_SECTION_DOMAIN_SHIFT)
#define VMSAv6_SECTION_DOMAIN_MANAGER	(3 << VMSAv6_SECTION_DOMAIN_SHIFT)


void vmsav6_set_mmu_section(
	uint32_t va,
	uint32_t pa,
	uint32_t options
)
{
	uint32_t idx = va >> 20;
	uint32_t entry = (pa & 0xfff00000) | (options & 0x7ffc) | VMSAv6_SECTION_FLAG ;
	ttbr0_lv1_pagetable[idx] = entry;
	return;
}

void create_straight_pagetable(void)
{
	uint32_t entry = 0;
	uint32_t peri_base = PHY_PERI_ADDR(0);
	uint32_t entry_peri_base = peri_base >> 20;

	// 0x00000000 ~ 0x20000000(512MiB): RAM
	for (entry = 0; entry < (0x20000000 >> 20);  entry++) {
		uint32_t addr = entry << 20;
		vmsav6_set_mmu_section(
			addr, 
			addr, 
			VMSAv6_SECTION_CACHEABLE 
			| VMSAv6_SECTION_BAFFURABLE 
			| VMSAv6_SECTION_AP_FULLACCESS
		);
	}

	for (entry = (peri_base >> 20); entry < 4096; entry++) {
		uint32_t addr = entry << 20;
		vmsav6_set_mmu_section(
			addr, 
			addr, 
			VMSAv6_SECTION_AP_FULLACCESS
		);
	}

	
}

void print_vmsav6_pagetable(uint32_t *table_base)
{
	printf("Addr\tVA\tPA\traw\r\n");
	for (uint32_t i = 0; i < 4096; i++) {
		uint32_t pa = table_base[i] & 0xfff00000;
		uint32_t va = i << 20;
		printf("%08x\t%08x\t%08x\t%08x\r\n", &(table_base[i]), va, pa, table_base[i]);
	}
}

static inline uint32_t get_ttbr0(void)
{
	uint32_t ttbr0 = 0;
	__asm__ volatile("mrc p15, 0, %[t], c8, c7" : : [t] "rw" (ttbr0) : );
	return ttbr0;
}


static inline void  __armv6_dsb(void)
{
	volatile uint32_t zero = 0;
	// data synchronization barrier
	__asm__ volatile("mcr p15, 0, %[zero], c7, c10" : : [zero] "r" (zero) : );
}


static inline void set_alldomain_to_manager(void)
{
	volatile uint32_t domain_all_manager = 0xffffffff;
	__asm__ volatile("mcr p15, 0, %[d], c3, c0, 0" : : [d] "r" (domain_all_manager) : );
}

static inline void invalidate_cache_all(void)
{
	volatile uint32_t zero = 0;
	__asm__ volatile("mcr p15, 0, %[zero], c7, c7" : : [zero] "r" (zero) : );
}

static inline void invalidate_tlb_inst(void)
{
	volatile uint32_t zero = 0;
	__asm__ volatile("mcr p15, 0, %[zero], c8, c5" : : [zero] "r" (zero) : );
	__armv6_dsb();
}

static inline void invalidate_tlb_data(void)
{
	volatile uint32_t zero = 0;
	__asm__ volatile("mcr p15, 0, %[zero], c8, c6" : : [zero] "r" (zero) : );
	__armv6_dsb();
}

static inline void invalidate_tlb_joint(void)
{
	volatile uint32_t zero = 0;
	__asm__ volatile("mcr p15, 0, %[zero], c8, c7" : : [zero] "r" (zero) : );
	__armv6_dsb();
}



static inline void enable_mmu(void)
{
	volatile uint32_t mmu_en_mask = 0;
	mmu_en_mask |= 1; 		// M = 1
	mmu_en_mask |= 1 << 2;	// C = 1
	mmu_en_mask |= 1 << 16;	// I = 1

	__asm__ volatile(
		"mrc p15, 0, r0, c1, c0, 0;" 
		"orr r0, r0, %[x];"
		"mcr p15, 0, r0, c1, c0, 0;"
		: 
		: [x] "r" (mmu_en_mask)
		: "r0"
	);
}

static inline void set_ttbr0(uint32_t *pagetable)
{
	__asm__ volatile("mcr p15, 0, %[p], c2, c0, 0" : : [p] "r" (pagetable) : );
}

static inline void nop_x10(void)
{
	__asm__ volatile("nop;nop;nop;nop;nop;" : : : );
	__asm__ volatile("nop;nop;nop;nop;nop;" : : : );
}


void init_mmu(void)
{
	create_straight_pagetable();

	// invalidate icache and dcache
	invalidate_cache_all();

	// invalidate tlb
	invalidate_tlb_inst();
	invalidate_tlb_data();
	invalidate_tlb_joint();
	
	// set all domain to manager
	set_alldomain_to_manager();

	// set page-table base address to TTBR0
	set_ttbr0(ttbr0_lv1_pagetable);

	// DEBUG: show pagetable
	// uint32_t ttbr0 = get_ttbr0();

	// enable mmu and cache
	enable_mmu();

	nop_x10();
}


void jtag_init(void)
{
	// JTAG用設定
	// 3.3V			: ARM_VREF 
	// GPIO22 (ALT4): ARM_TRST
	// GPIO4  (ALT5): ARM_TDI
	// GPIO27 (ALT4): ARM_TMS
	// GPIO25 (ALT4): ARM_TCK
	// GPIO24 (ALT4): ARM_TDO
	// GND			: ARM_GND
	pinMode(22, ALT4);
	pinMode(4, ALT5);
	pinMode(27, ALT4);
	pinMode(25, ALT4);
	pinMode(24, ALT4);
}


int main(void){
	rpi_init();

	jtag_init();

	init_mmu();

	while(1) {
		printf("Nyan\r\n");
		delay(1000);
	}

	return 0;
}
