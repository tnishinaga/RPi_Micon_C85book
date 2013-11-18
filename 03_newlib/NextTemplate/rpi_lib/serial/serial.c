#include "rpi_serial.h"
#include "../peripherals/rpi_peripherals.h"
#include "../rpi_type.h"
#include "../timer/rpi_timer.h"
#include "../gpio/rpi_gpio.h"

// serial clock(default: 3MHz)
#define SERIAL_CLOCK	3000000

// read,write用タイムアウト(us)
long long write_to = -1;
long long read_to = -1;

void setSerialTimeout(int read_ms,int write_ms){
	// 読み込みタイムアウト設定	
	if(read_ms <= 0){
		read_to = -1;
	}
	else{
		read_to = read_ms * 1000;
	}
	// 書き込みタイムアウト設定
	if(write_ms <= 0){
		write_to = -1;
	}
	else{
		write_to = write_ms * 1000;
	}
}

int Serial_begin(int baudrate){
	unsigned int baudrate_temp[11]={300, 1200, 2400, 4800, 9600, 14400, 19200, 28800, 38400, 57600, 115200 };
	unsigned int ib_list[] = {625, 156,78,39,19,13,9,6,4,3,1};
	unsigned int fb_list[] = {0,16,8,4,34,1,49,33,57,16,40};
	int flag=0;
	int i;
	// baudrate check
	for(i=0;i<11;i++){
		if(baudrate == baudrate_temp[i]){
			flag = 1;
			break;
		}
	}
	if(flag == 0)
		return -1;

	
	/****	初期設定開始	***/
	// UART無効化
	*UART0_CR 	= 0;
	
	//ポートの設定
	pinMode(14,INPUT_PULLDOWN);
	pinMode(15,INPUT_PULLDOWN);
	pinMode(14,ALT0);
	pinMode(15,ALT0);

	// ボーレート計算
	// 諸事情により断念
	// float bauddiv = SERIAL_CLOCK / ( 16 * baudrate );
	// int ibrd = bauddiv;
	// bauddiv -= bauddiv - ibrd;
	// int fbrd = bauddiv * 64 + 0.5;

	// ボーレート設定
	*UART0_IBRD = ib_list[i];
	*UART0_FBRD = fb_list[i];
	
	// LCRH
	// stick parity dis, 8bit, FIFO en, two stop bit no, odd parity, parity dis, break no
	*UART0_LCRH = 0x70;

	// CR
	// CTS dis, RTS dis, OUT1-2=0, RTS dis, DTR dis, RXE en, TXE en, loop back dis, SIRLP=0, SIREN=0, UARTEN en
	*UART0_CR 	= 0x0301;
	/****	初期設定終了	***/

	return 0;
}

void Serial_end(void){
	// UART無効化
	*UART0_CR = 0;
}

int Serial_available(void){
	if(!(*UART0_FR & (0x01 << 4))){
		return 1;
	}
	return 0;
}

int Serial_write(char *buf,int len){
	int i=0;
	while(i < len){
		if(uart0_putc(*buf) < 0)
			break;
		i++;
		buf++;
	}
	return i;
}

int uart0_getc(void){
	unsigned long long to;
	// タイムアウトが設定されている場合はセット
	if(read_to != -1){
		to = micros() + to;
	}
	// データが来るまで待つ
	while(Serial_available() == 0){
		if(read_to != -1){
			if(to >= micros())
				return -1;
		}
	}
	// 読み込み
	int c = *UART0_DR;
	return c & 0xff;
}

int uart0_putc(int c){
	unsigned long long to;
	// タイムアウトが設定されている場合はセット
	if(write_to != -1){
		to = micros() + to;
	}
	// 送信FIFOが空くのを待つ
	while(*UART0_FR & (0x01 << 5)){
		if(write_to != -1){
			if(to >= micros())
				return -1;
		}
	}
	
	// 書き込み
	*UART0_DR = c;
	return 1;
}

int uart0_puts(char *s){
	int i=0;
	while(*s != '\n' && *s != '\0'){
		if(uart0_putc(*s) < 0)
			break;
		i++;
		s++;
	}
	if(*s == '\n'){
		uart0_putc('\n');
		i++;
	}
	return i;
}

char *uart0_gets(char *s,int len){
	int i=1;
	int c;
	do{
		c = uart0_getc();
		if(c == -1)
			return (void *)0;
		*s = c;
		s++;
		i++;
	}while(i < len && c != '\n');
	*s = '\0';

	return s;
}

