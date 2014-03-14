#ifndef RPILIB_PERIPHERALS_H
#define RPILIB_PERIPHERALS_H
 
#include "../rpi_type.h"

#define PHY_PERI_ADDR(x) (0x20000000 + (x))

// GPIO Peripheral
#define GPIO_BASE	(0x00200000)
#define GPFSEL0		((vu32_t *)PHY_PERI_ADDR(GPIO_BASE + 0x00))
#define GPFSEL1		((vu32_t *)PHY_PERI_ADDR(GPIO_BASE + 0x04))
#define GPFSEL2		((vu32_t *)PHY_PERI_ADDR(GPIO_BASE + 0x08))
#define GPFSEL3		((vu32_t *)PHY_PERI_ADDR(GPIO_BASE + 0x0c))
#define GPFSEL4		((vu32_t *)PHY_PERI_ADDR(GPIO_BASE + 0x10))
#define GPFSEL5		((vu32_t *)PHY_PERI_ADDR(GPIO_BASE + 0x14))
#define GPSET0		((vu32_t *)PHY_PERI_ADDR(GPIO_BASE + 0x1c))
#define GPSET1		((vu32_t *)PHY_PERI_ADDR(GPIO_BASE + 0x20))
#define GPCLR0		((vu32_t *)PHY_PERI_ADDR(GPIO_BASE + 0x28))
#define GPCLR1		((vu32_t *)PHY_PERI_ADDR(GPIO_BASE + 0x2c))
#define GPLEV0		((vu32_t *)PHY_PERI_ADDR(GPIO_BASE + 0x34))
#define GPLEV1		((vu32_t *)PHY_PERI_ADDR(GPIO_BASE + 0x38))
// for GPFSEL mask
// use AND mask
#define GPFSEL_MASK_IN(n)	(~(vu32_t)(0x07 << ((n % 10) * 3)))
// use OR mask
#define GPFSEL_MASK_OUT(n)	(0x01 << ((n % 10) * 3))
#define GPFSEL_MASK_ALT0(n)	(0x04 << ((n % 10) * 3))
#define GPFSEL_MASK_ALT1(n)	(0x05 << ((n % 10) * 3))
#define GPFSEL_MASK_ALT2(n)	(0x06 << ((n % 10) * 3))
#define GPFSEL_MASK_ALT3(n)	(0x07 << ((n % 10) * 3))
#define GPFSEL_MASK_ALT4(n)	(0x03 << ((n % 10) * 3))
#define GPFSEL_MASK_ALT5(n)	(0x02 << ((n % 10) * 3))
// GPIO PULLUP/DOWN
#define GPPUD 		((vu32_t *)PHY_PERI_ADDR(GPIO_BASE + 0x94))
#define GPPUDCLK0	((vu32_t *)PHY_PERI_ADDR(GPIO_BASE + 0x98))
#define GPPUDCLK1	((vu32_t *)PHY_PERI_ADDR(GPIO_BASE + 0x98))


// SystemTimer Peripheral
#define SYST_BASE	(0x00003000)
#define SYST_CS		((vu32_t *)PHY_PERI_ADDR(SYST_BASE + 0x00))
#define SYST_CLO	((vu32_t *)PHY_PERI_ADDR(SYST_BASE + 0x04))
#define SYST_CHI	((vu32_t *)PHY_PERI_ADDR(SYST_BASE + 0x08))
#define SYST_C0		((vu32_t *)PHY_PERI_ADDR(SYST_BASE + 0x0c))
#define SYST_C1		((vu32_t *)PHY_PERI_ADDR(SYST_BASE + 0x10))
#define SYST_C2		((vu32_t *)PHY_PERI_ADDR(SYST_BASE + 0x14))
#define SYST_C3		((vu32_t *)PHY_PERI_ADDR(SYST_BASE + 0x18))

#endif
