#include "AQM0802.h"


void aqm0802_init(void){
	uint8_t aqm0802_initcmd_1st[18] = {
		AQM0802_CTRL_Co,	// Co=1,RS=0
		AQM0802_CMD_FUNCTION_BASE | AQM0802_CMD_FUNCTION_DL_EN | AQM0802_CMD_FUNCTION_N_EN,	// Function Set(0x38)
		AQM0802_CTRL_Co,	// Co=1,RS=0
		AQM0802_CMD_FUNCTION_BASE | AQM0802_CMD_FUNCTION_DL_EN | AQM0802_CMD_FUNCTION_N_EN | AQM0802_CMD_FUNCTION_IS_EN, // Function Set(0x39)
		AQM0802_CTRL_Co,	// Co=1,RS=0
		AQM0802_OSC_FREQ_BASE | AQM0802_OSC_FREQ_F2_EN,	// Internal OSC frequency(0x14)
		AQM0802_CTRL_Co,	// Co=1,RS=0
		AQM0802_CONTRAST_BASE,	// Contrast Set(0x70)
		AQM0802_CTRL_Co,	// Co=1,RS=0
		AQM0802_POW_ICON_CONTRAST_BASE | AQM0802_POW_ICON_CONTRAST_BON_EN | AQM0802_POW_ICON_CONTRAST_C5_EN, // Power/ICON/Contrast(0x56)
		AQM0802_CTRL_Co,	// Co=1,RS=0
		AQM0802_FOLLOWER_CTRL_BASE | AQM0802_FOLLOWER_CTRL_FON_EN | AQM0802_FOLLOWER_CTRL_RAB2_EN // Follower control(0x6C)
	};
	uint8_t aqm0802_initcmd_2nd[6] = {
		AQM0802_CTRL_Co,	// Co=1,RS=0
		AQM0802_CMD_FUNCTION_BASE | AQM0802_CMD_FUNCTION_DL_EN | AQM0802_CMD_FUNCTION_N_EN,	// Function Set(0x38)
		AQM0802_CTRL_Co,// Co=1,RS=0
		AQM0802_CMD_DISPLAY_CTRL_BASE | AQM0802_CMD_DISPLAY_CTRL_D,	// Display ON （0x0C）
		AQM0802_CTRL_Co,// Co=1,RS=0
		AQM0802_CMD_CLEAR_DISPLAY	// Display Clear(0x01)
	};

	// LCD init start
	Wire1_beginTransmission(AQM0802_ADDR);
	Wire1_writeData(aqm0802_initcmd_1st, 18);
	Wire1_endTransmission(TRUE);

	delay(250);

	Wire1_beginTransmission(AQM0802_ADDR);
	Wire1_writeData(aqm0802_initcmd_2nd, 6);
	Wire1_endTransmission(TRUE);
	// LCD init end
}

void aqm0802_cleardisplay(void){
	uint8_t aqm0802_cmd_clear[2] = {
		AQM0802_CTRL_Co,// Co=1,RS=0
		AQM0802_CMD_CLEAR_DISPLAY	// Display Clear(0x01)
	};

	Wire1_beginTransmission(AQM0802_ADDR);
	Wire1_writeData(aqm0802_cmd_clear, 2);
	Wire1_endTransmission(TRUE);

	return;
}

void aqm0802_print_line(uint8_t *str, uint8_t line){
	uint8_t aqm0802_cmd_sendchar[3] = {
		AQM0802_CTRL_Co,	// Co=1,RS=0
		AQM0802_SET_DDRAM_ADDR_BASE,	// set DDRAM address
		AQM0802_CTRL_RS	// Co=0,RS=1
	};
	uint32_t i = 0;

	// count length
	while(str[i] != '\0'){
		i++;
	}
	if(i > 8){
		// 1line is 8 character max
		i = 8;
	}

	if(line == 2){
		// 2nd line offset 0x40
		aqm0802_cmd_sendchar[1] |= 0x40;
	}

	Wire1_beginTransmission(AQM0802_ADDR);
	Wire1_writeData(aqm0802_cmd_sendchar, 3);
	Wire1_writeData(str, i);
	Wire1_endTransmission(TRUE);
}
