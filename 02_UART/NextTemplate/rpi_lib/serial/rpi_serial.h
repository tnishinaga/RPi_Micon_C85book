#ifndef RPILIB_SERIAL_H
#define RPILIB_SERIAL_H
 
void setSerialTimeout(int read_ms,int write_ms);

int uart0_getc(void);
int uart0_putc(int c);
int uart0_puts(char *s);
char *uart0_gets(char *s,int len);

int Serial_begin(int baudrate);
void Serial_end(void);
int Serial_available(void);

int Serial_write(char *buf,int len);
#define Serial_read		uart0_getc

#endif
