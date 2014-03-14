#include "rpi_i2c.h"

// configure value
volatile uint16_t divider = BSC_DIV_100KHZ;
volatile int readmode = FALSE;

// buffer
volatile uint8_t txBuffer[BUFFER_LENGTH];
volatile uint32_t txBufferIndex;
volatile uint32_t txBufferLength;
volatile uint8_t rxBuffer[BUFFER_LENGTH];
volatile uint32_t rxBufferIndex;
volatile uint32_t rxBufferLength;

void Wire1_setSpeed(uint16_t div){
	divider = div;
}

void Wire1_begin(void){
	// I2C init
	// port setting
	pinMode(2, ALT0); // SDA
	pinMode(3, ALT0); // SCL

	// set I2C clock divider
	// SCL = 150[MHz] / divider
	*BSC1_DIV = divider;

	// reset index and length
	txBufferIndex = 0;
	txBufferLength = 0;
	rxBufferIndex = 0;
	rxBufferLength = 0;
}


uint32_t Wire1_requestFrom(uint8_t address, uint32_t count){
	int i=0;
	// reset index and length
	rxBufferIndex = 0;
	rxBufferLength = (count <= BUFFER_LENGTH)? count : BUFFER_LENGTH;

	if(readmode == TRUE){
		// repeated start
		readmode = FALSE;

		// transfer start
		*BSC1_C |= BSC_C_I2CEN_EN | BSC_C_ST_EN;
		// 転送開始待ち
		// 1 = Transfer active
		while(!(*BSC1_S & BSC_S_TA));
		// set receive data length
		*BSC1_DLEN = rxBufferLength;
		// Read mode
		*BSC1_C |= BSC_C_READ_READ;
		// transfer start
		*BSC1_C |= BSC_C_ST_EN;

		// 最遅（4.58kHz）でFIFOにあるデータを(1+n)byte送信し切るまで待つ
		// Tmax = 1 / 4.58*10^3
		// 8bit transfer time max = Tmax * 8 = 1747 us
		delayMicroseconds(1747 * (1 + txBufferLength));
	}
	else{
		// normal read

		// Clear Status
		*BSC1_S = BSC_S_CLKT | BSC_S_ERR | BSC_S_DONE;
		// Buffer clear
		*BSC1_C |= BSC_C_CLEAR_ALL;
		// slave address
		*BSC1_A = address >> 1;
		// set receive data length
		*BSC1_DLEN = rxBufferLength;
		// Read mode
		*BSC1_C |= BSC_C_READ_READ;
		// transfer start
		*BSC1_C |= BSC_C_I2CEN_EN | BSC_C_ST_EN;
	}

	// 転送終了待ち
	// 1 = Transfer complete
	while(!(*BSC1_S & BSC_S_DONE)){
		// error check
		if((*BSC1_S & BSC_S_ERR) | (*BSC1_S & BSC_S_CLKT)){
			// clock stretch error
			// slave address NoACK
			break;
		}
		// receive data
		if(rxBufferIndex < rxBufferLength  &&  *BSC1_S & BSC_S_RXD){
			rxBuffer[rxBufferIndex++] = 0xff & *BSC1_FIFO;
		}
	}

	// receive remainder data
	while(rxBufferIndex < rxBufferLength  &&  *BSC1_S & BSC_S_RXD){
		rxBuffer[rxBufferIndex++] = 0xff & *BSC1_FIFO;
	}

	rxBufferLength = rxBufferIndex;
	rxBufferIndex = 0;
	// return receive data length
	return rxBufferLength;
}


void Wire1_beginTransmission(uint8_t address){
	// reset index and length
	txBufferIndex = 0;
	txBufferLength = 0;

	// slave address
	*BSC1_A = address >> 1;
	// Write mode
	*BSC1_C &= ~BSC_C_READ_READ;
}


uint8_t Wire1_endTransmission(int mode){
	int i = 0;

	txBufferLength = txBufferIndex;

	// Clear Status
	*BSC1_S = BSC_S_CLKT | BSC_S_ERR | BSC_S_DONE;
	// Buffer clear
	*BSC1_C |= BSC_C_CLEAR_ALL;
	// 送信データ長設定
	*BSC1_DLEN = txBufferLength;

	// set data to FIFO
	while(i < txBufferLength && i <= (FIFO_LENGTH - 4)){
		*BSC1_FIFO = 0xff & txBuffer[i++];
	}

	// FALSEの時はrequestFormでI2C-readを行うものとして処理
	if(mode == FALSE){
		readmode = TRUE;
		return 0;
	}

	// transfer start
	*BSC1_C |= BSC_C_I2CEN_EN | BSC_C_ST_EN;

	// 転送終了待ち
	// 1 = Transfer complete
	while(!(*BSC1_S & BSC_S_DONE)){
		// error check
		if(*BSC1_S & BSC_S_ERR){
			// 2: slave address NoACK
			return 2;
		}
		if(*BSC1_S & BSC_S_CLKT){
			// clock stretch error
			// 4: unknown error
			return 4;
		}

		// txBufferの残りをFIFOに追加
		if(i < txBufferLength  &&  *BSC1_S & BSC_S_TXW){
			*BSC1_FIFO = 0xff & txBuffer[i++];
		}
	}

	// 0: transfer success
	return 0;
}


uint32_t Wire1_writeByte(uint8_t value){
	if(txBufferIndex >= BUFFER_LENGTH){
		// buffer full
		return 0;
	}

	// add buffer
	txBuffer[txBufferIndex++] = value;

	return 1;
}


uint32_t Wire1_writeString(uint8_t *string){
	uint32_t i = 0;

	while(string[i] != '\0'){
		i++;
	}

	return Wire1_writeData(string, i);
}


uint32_t Wire1_writeData(uint8_t *data, uint32_t quantity){
	uint32_t i = 0;

	if(quantity == 0){
		return 0;
	}

	while(i < quantity && txBufferIndex < BUFFER_LENGTH){
		txBuffer[txBufferIndex++] = data[i++];
	}

	return i;
}


uint32_t Wire1_available(void){
	return rxBufferLength - rxBufferIndex;
}


uint8_t Wire1_read(void){
	uint8_t read = 0;

	if(rxBufferIndex < rxBufferLength){
		read = rxBuffer[rxBufferIndex++];
	}
	
	return read;
}

