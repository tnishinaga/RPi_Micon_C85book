#ifndef RPILIB_GPIO_H
#define RPILIB_GPIO_H
 
#define HIGH	1
#define LOW		0

// GPIO mode
#define INPUT	0x01
#define OUTPUT	0x00
#define ALT0	0x04
#define ALT1	0x05
#define ALT2	0x06
#define ALT3	0x07
#define ALT4	0x03
#define ALT5	0x02
#define INPUT_PULLUP	0x08
#define INPUT_PULLDOWN	0x09

// init 
void init_gpio(void);

// functions
void setPullUpDown(int pin,int mode);
void pinMode(int pin,int mode);
void digitalWrite(int pin, int value);
int digitalRead(int pin);
#endif
