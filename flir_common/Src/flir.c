#include "flir.h"

uint8_t cmd_p[FLIR_CMDLEN] = {0x6E, 0x00, 0x00, 0x05, 0x00, 0x00, 0x34, 0x4b, 0x00, 0x00, 
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
											 
extern uint8_t answ_ready;
extern uint8_t rx_cnt;
//extern uint16_t cur_timeout;
extern UART_HandleTypeDef huart1;
extern uint8_t *ui_rx_buff;
														
															
void print_buf_in_hex(uint8_t* buf, uint8_t len){
	uint8_t i;
	if (len > 0){
		for (i=0; i<len; i++){
			printf(" %02x", *(buf + i));
		}
		printf("\n\r");
	} else {
		printf("00 .. .. 00\n\r");
	}
}


void clr_buf(uint8_t* buf, uint8_t len){
	uint8_t i;
	for (i=0; i<len; i++){
		*(buf + i) = 0;
	}
}


void make_cmd_uni(uint8_t cmd, uint8_t data_len, uint8_t* data){
	uint32_t crc;
	uint8_t i;
	
	cmd_p[3] = cmd;
	cmd_p[5] = data_len;
	crc = crc16(cmd_p, 6);
	cmd_p[6] = (crc >> 8) & 0xff;
	cmd_p[7] = crc & 0xff;
	if (data_len > 0){
		for (i=0; i<data_len; i++){
			cmd_p[8 + i] = *(data + i);
		}
		crc = crc16(cmd_p + 8, data_len);
		cmd_p[8 + i] = (crc >> 8) & 0xff;
		cmd_p[8 + i + 1] = crc & 0xff;
  } else {
		cmd_p[8] = 0;
		cmd_p[9] = 0;
	}
#ifdef FLIR_DEBUG
	print_buf_in_hex(cmd_p, 10 + data_len);
#endif
}

void make_cmd_word(uint8_t cmd, uint16_t data){
	uint8_t tmp[2];
	tmp[1] = data & 0xff;
	tmp[0] = (data >> 8)& 0xff;
	make_cmd_uni(cmd, 2, tmp);
}

void flir_set_standart(uint16_t data){
	answ_ready = 0;
	rx_cnt = 0;
	make_cmd_word(0x72, data);
	HAL_UART_Transmit(FLIR_HUART, cmd_p, 12, 0xFFFF);
	flir_wait(FLIR_RX_BUFF_POINTER);
}

void flir_set_palette(uint16_t data){
	answ_ready = 0;
	rx_cnt = 0;
	make_cmd_word(0x10, data);
	HAL_UART_Transmit(FLIR_HUART, cmd_p, 12, 0xFFFF);
	flir_wait(FLIR_RX_BUFF_POINTER);
}

void flir_set_orientation(uint16_t data){
	answ_ready = 0;
	rx_cnt = 0;
	make_cmd_word(0x11, data);
	HAL_UART_Transmit(FLIR_HUART, cmd_p, 12, 0xFFFF);
	flir_wait(FLIR_RX_BUFF_POINTER);
}

void flir_save(void){
	answ_ready = 0;
	rx_cnt = 0;
	make_cmd_uni(1, 0, 0);
	HAL_UART_Transmit(FLIR_HUART, cmd_p, 10, 0xFFFF);
	flir_wait(FLIR_RX_BUFF_POINTER);
}

void flir_calibrate(void){
	answ_ready = 0;
	rx_cnt = 0;
	make_cmd_uni(0x0C, 0, 0);
	HAL_UART_Transmit(FLIR_HUART, cmd_p, 10, 0xFFFF);
	flir_wait(FLIR_RX_BUFF_POINTER);
}

void flir_restore_defaults(uint8_t* cmd_p){
	answ_ready = 0;
	rx_cnt = 0;
	make_cmd_uni(3, 0, 0);
	HAL_UART_Transmit(FLIR_HUART, cmd_p, 10, 0xFFFF);
	flir_wait(FLIR_RX_BUFF_POINTER);
}

void flir_set_zoom(uint16_t data){
	answ_ready = 0;
	rx_cnt = 0;
	if (data) data = 2 << data;
	make_cmd_word(0x0f, data);
	HAL_UART_Transmit(FLIR_HUART, cmd_p, 12, 0xFFFF);
	flir_wait(FLIR_RX_BUFF_POINTER);
	if (answ_ready > 0) {
		if (*(FLIR_RX_BUFF_POINTER + 1) != 0) {
			answ_ready = 0;
		}
		if (*(FLIR_RX_BUFF_POINTER + 3) != 0x0f) {
			answ_ready = 0;
		}
	}
}

void flir_ezoom_up(uint8_t step){
	uint8_t data[4] = {0, 2, 0, step};
	
	answ_ready = 0;
	rx_cnt = 0;
  
	make_cmd_uni(0x32, 4, data);
	HAL_UART_Transmit(FLIR_HUART, cmd_p, 14, 0xFFFF);
	flir_wait(FLIR_RX_BUFF_POINTER);
}

void flir_ezoom_down(uint8_t step){
	uint8_t data[4] = {0, 3, 0, step};
	
	answ_ready = 0;
	rx_cnt = 0;
  
	make_cmd_uni(0x32, 4, data);
	HAL_UART_Transmit(FLIR_HUART, cmd_p, 14, 0xFFFF);
	flir_wait(FLIR_RX_BUFF_POINTER);
}

void flir_set_ezoom(uint16_t zoom){
	uint8_t data[4] = {0, 1, 0, 0};
	
	data[3] = zoom & 0xff;
	data[2] = (zoom >> 8) & 0xff;
	
	answ_ready = 0;
	rx_cnt = 0;
  
	make_cmd_uni(0x32, 4, data);
	HAL_UART_Transmit(FLIR_HUART, cmd_p, 14, 0xFFFF);
	flir_wait(FLIR_RX_BUFF_POINTER);
}

void flir_set_tilt_pan(int16_t tilt, int16_t pan){
	uint8_t data[4] = {0, 0, 0, 0};
	
	data[1] = tilt & 0xff;
	data[0] = (tilt >> 8) & 0xff;
	data[3] = pan & 0xff;
	data[2] = (pan >> 8) & 0xff;
	
	answ_ready = 0;
	rx_cnt = 0;
  
	make_cmd_uni(0x70, 4, data);
	HAL_UART_Transmit(FLIR_HUART, cmd_p, 14, 0xFFFF);
	flir_wait(FLIR_RX_BUFF_POINTER);
}


void flir_set_brightness(uint16_t data){
	answ_ready = 0;
	rx_cnt = 0;
	make_cmd_word(0x15, data);
	HAL_UART_Transmit(FLIR_HUART, cmd_p, 12, 0xFFFF);
	flir_wait(FLIR_RX_BUFF_POINTER);
}

void flir_set_agg(uint16_t data){
	answ_ready = 0;
	rx_cnt = 0;
	make_cmd_word(0x13, data);
	HAL_UART_Transmit(FLIR_HUART, cmd_p, 12, 0xFFFF);
	flir_wait(FLIR_RX_BUFF_POINTER);
}

void flir_rectangle(uint8_t cmd_id, uint16_t x, uint16_t y, 
									  uint16_t width, uint16_t height, uint8_t back_color, uint8_t color, uint16_t outline){
	//                    id,            x,     y,     w,     h,    cl 
	uint8_t data[16] = {0, 0,  0, 4,  0, 0,  0, 0,  0, 0,  0, 0,  0, 0,  0, 0};
	
	data[1] = cmd_id;
	data[4] = (x >> 8) & 0xFF;
	data[5] = x & 0xFF;
	data[6] = (y >> 8) & 0xFF;
	data[7] = y & 0xFF;
	data[8] = (width >> 8) & 0xFF;
	data[9] = width & 0xFF;
	data[10] = (height >> 8) & 0xFF;
	data[11] = height & 0xFF;
	data[12] = back_color;
	data[13] = color;
	data[14] = (outline >> 8) & 0xFF;
	data[15] = outline & 0xFF;
	
	answ_ready = 0;
	rx_cnt = 0;
	make_cmd_uni(0x2f, 16, data);
	HAL_UART_Transmit(FLIR_HUART, cmd_p, 26, 0xFFFF);
	flir_wait(FLIR_RX_BUFF_POINTER);
}


void flir_bar(uint8_t cmd_id, uint16_t x, uint16_t y, 
					    uint16_t width, uint16_t height, uint8_t back_color, uint8_t color){
	//                    id,            x,     y,     w,     h,    cl 
	uint8_t data[14] = {0, 0,  0, 1,  0, 0,  0, 0,  0, 0,  0, 0,  0, 0};
	
	data[1] = cmd_id;
	data[4] = (x >> 8) & 0xFF;
	data[5] = x & 0xFF;
	data[6] = (y >> 8) & 0xFF;
	data[7] = y & 0xFF;
	data[8] = (width >> 8) & 0xFF;
	data[9] = width & 0xFF;
	data[10] = (height >> 8) & 0xFF;
	data[11] = height & 0xFF;
	data[12] = back_color;
	data[13] = color;
	
	answ_ready = 0;
	rx_cnt = 0;
	make_cmd_uni(0x2f, 14, data);
	HAL_UART_Transmit(FLIR_HUART, cmd_p, 24, 0xFFFF);
	flir_wait(FLIR_RX_BUFF_POINTER);
}
										
void flir_text(uint8_t cmd_id, uint16_t x, uint16_t y, 
							 uint16_t align, uint16_t font, uint8_t back_color, uint8_t color, const uint8_t* text, uint8_t l){
	//                    id,            x,     y,     a,     f,    cl 
	uint8_t i;
  uint8_t data[46] = {0, 0,  0, 2,  0, 0,  0, 0,  0, 0,  0, 0,  0, 0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	
	
	data[1] = cmd_id;
	data[4] = (x >> 8) & 0xFF;
	data[5] = x & 0xFF;
	data[6] = (y >> 8) & 0xFF;
	data[7] = y & 0xFF;
	data[8] = (align >> 8) & 0xFF;
	data[9] = align & 0xFF;
	data[10] = (font >> 8) & 0xFF;
	data[11] = font & 0xFF;
	data[12] = back_color;
	data[13] = color;
	
	if (l > 31) l = 31;
	for (i = 0; i < l; i++){
		data[i+14] = text[i];
	}
	if (l > 0 && l%2) l++;
	
	answ_ready = 0;
	rx_cnt = 0;
	make_cmd_uni(0x2f, 14+l, data);
	HAL_UART_Transmit(FLIR_HUART, cmd_p, 24+l, 0xFFFF);
	flir_wait(FLIR_RX_BUFF_POINTER);
}
										

void flir_clear(uint8_t cmd_id){
	//                    id,            x,     y,     w,     h,    cl 
	uint8_t data[14] = {0, 0,  0, 0,  0, 0,  0, 0,  0, 0,  0, 0,  0, 0};
	
	data[1] = cmd_id;
	
	answ_ready = 0;
	rx_cnt = 0;
	make_cmd_uni(0x2f, 14, data);
	HAL_UART_Transmit(FLIR_HUART, cmd_p, 24, 0xFFFF);
	flir_wait(FLIR_RX_BUFF_POINTER);
}


uint16_t flir_get_brightness(){
	uint16_t tmp;
	uint16_t ret = 0;
	
	answ_ready = 0;
	rx_cnt = 0;
	make_cmd_uni(0x15, 0, 0);
	HAL_UART_Transmit(FLIR_HUART, cmd_p, 10, 0xFFFF);
	flir_wait(FLIR_RX_BUFF_POINTER);
	if (answ_ready > 0) {
		if (*(FLIR_RX_BUFF_POINTER + 1) != 0) {
			answ_ready = 0;
		}
		if (*(FLIR_RX_BUFF_POINTER + 3) != 0x15) {
			answ_ready = 0;
		}
	}
	if (answ_ready > 0){
		tmp = *(FLIR_RX_BUFF_POINTER + 8) << 8;
		tmp += *(FLIR_RX_BUFF_POINTER + 9);
		ret = tmp;
	}
	return ret;
}

uint16_t flir_get_brightness_bias(void){
	uint16_t tmp;
	uint16_t ret = 0;
	
	answ_ready = 0;
	rx_cnt = 0;
	make_cmd_uni(0x18, 0, 0);
	HAL_UART_Transmit(FLIR_HUART, cmd_p, 10, 0xFFFF);
	flir_wait(FLIR_RX_BUFF_POINTER);
	if (answ_ready > 0) {
		if (*(FLIR_RX_BUFF_POINTER + 1) != 0) {
			answ_ready = 0;
		}
		if (*(FLIR_RX_BUFF_POINTER + 3) != 0x15) {
			answ_ready = 0;
		}
	}
	if (answ_ready > 0){
		tmp = *(FLIR_RX_BUFF_POINTER + 8) << 8;
		tmp += *(FLIR_RX_BUFF_POINTER + 9);
		ret = tmp;
	}
	return ret;
}

uint8_t flir_get_zoom(void){
	uint16_t tmp;
	uint8_t ret = 0;
	
	answ_ready = 0;
	rx_cnt = 0;
	make_cmd_uni(0x0f, 0, 0);
	HAL_UART_Transmit(FLIR_HUART, cmd_p, 10, 0xFFFF);
	flir_wait(FLIR_RX_BUFF_POINTER);
	if (answ_ready > 0) {
		if (*(FLIR_RX_BUFF_POINTER + 1) != 0) {
			answ_ready = 0;
		}
		if (*(FLIR_RX_BUFF_POINTER + 3) != 0x0f) {
			answ_ready = 0;
		}
	}
	if (answ_ready > 0){
		tmp = *(FLIR_RX_BUFF_POINTER + 8) << 8;
		tmp += *(FLIR_RX_BUFF_POINTER + 9);
		tmp = tmp >> 2;  
		ret = tmp & 3;
	}
	return ret;
}

uint8_t flir_get_palette(void){
	uint16_t tmp;
	uint8_t ret = 0;
	
	answ_ready = 0;
	rx_cnt = 0;
	make_cmd_uni(0x10, 0, 0);
	HAL_UART_Transmit(FLIR_HUART, cmd_p, 10, 0xFFFF);
	flir_wait(FLIR_RX_BUFF_POINTER);
	if (answ_ready > 0){
		tmp = *(FLIR_RX_BUFF_POINTER + 8) << 8;
		tmp += *(FLIR_RX_BUFF_POINTER + 9);
		ret = tmp & 0xff;
	}
	if (answ_ready > 0) {
		if (*(FLIR_RX_BUFF_POINTER + 1) != 0) {
			answ_ready = 0;
		}
		if (*(FLIR_RX_BUFF_POINTER + 3) != 0x10) {
			answ_ready = 0;
		}
	}
	return ret;
}

uint16_t flir_get_max_zoom(void){
	uint16_t tmp = 0;
	uint16_t ret = 0;
	uint8_t data[4] = {0, 4, 0, 0};
	
	answ_ready = 0;
	rx_cnt = 0;
	make_cmd_uni(0x32, 4, data);
	HAL_UART_Transmit(FLIR_HUART, cmd_p, 14, 0xFFFF);
	flir_wait(FLIR_RX_BUFF_POINTER);
	if (answ_ready > 0){
		tmp = *(FLIR_RX_BUFF_POINTER + 8) << 8;
		tmp += *(FLIR_RX_BUFF_POINTER + 9);
		ret = tmp;
	}
	if (answ_ready > 0) {
		if (*(FLIR_RX_BUFF_POINTER + 1) != 0) {
			answ_ready = 0;
		}
		if (*(FLIR_RX_BUFF_POINTER + 3) != 0x32) {
			answ_ready = 0;
		}
	}
	return ret;
}

void flir_get_tilt_pan(int16_t* tilt, int16_t* pan){
	//uint16_t tmp = 0;
	//uint8_t data[4] = {0, 4, 0, 0};
	
	answ_ready = 0;
	rx_cnt = 0;
	make_cmd_uni(0x70, 0, 0);
	HAL_UART_Transmit(FLIR_HUART, cmd_p, 10, 0xFFFF);
	flir_wait(FLIR_RX_BUFF_POINTER);
	if (answ_ready > 0){
		*tilt = ((*(FLIR_RX_BUFF_POINTER + 8))<<8) + (*(FLIR_RX_BUFF_POINTER + 9));
		*pan = ((*(FLIR_RX_BUFF_POINTER + 10))<<8) + (*(FLIR_RX_BUFF_POINTER + 11));
	}
	if (answ_ready > 0) {
		if (*(FLIR_RX_BUFF_POINTER + 1) != 0) {
			answ_ready = 0;
		}
		if (*(FLIR_RX_BUFF_POINTER + 3) != 0x70) {
			answ_ready = 0;
		}
	}
	return;
}


uint16_t flir_get_cur_zoom(void){
	uint16_t tmp = 0;
	uint16_t ret = 0;
	//uint8_t data[4] = {0, 0, 0, 0};
	
	answ_ready = 0;
	rx_cnt = 0;
	//make_cmd_uni(0x32, 4, data);
	make_cmd_uni(0x32, 0, 0);
	HAL_UART_Transmit(FLIR_HUART, cmd_p, 10, 0xFFFF);
	flir_wait(FLIR_RX_BUFF_POINTER);
	if (answ_ready > 0) {
		if (*(FLIR_RX_BUFF_POINTER + 1) != 0) {
			answ_ready = 0;
		}
		if (*(FLIR_RX_BUFF_POINTER + 3) != 0x32) {
			answ_ready = 0;
		}
	}
	if (answ_ready > 0){
		tmp = *(FLIR_RX_BUFF_POINTER + 8) << 8;
		tmp += *(FLIR_RX_BUFF_POINTER + 9);
		ret = tmp;
	}
	return ret;
}

uint16_t flir_get_temp(void){
	uint16_t tmp = 0;
	uint16_t ret = 0;
	uint8_t data[2] = {0, 0};
	
	answ_ready = 0;
	rx_cnt = 0;

	make_cmd_uni(0x20, 2, data);
	HAL_UART_Transmit(FLIR_HUART, cmd_p, 12, 0xFFFF);
	flir_wait(FLIR_RX_BUFF_POINTER);
	if (answ_ready > 0) {
		if (*(FLIR_RX_BUFF_POINTER + 1) != 0) {
			answ_ready = 0;
		}
		if (*(FLIR_RX_BUFF_POINTER + 3) != 0x20) {
			answ_ready = 0;
		}
	}
	if (answ_ready > 0){
		tmp = *(FLIR_RX_BUFF_POINTER + 8) << 8;
		tmp += *(FLIR_RX_BUFF_POINTER + 9);
		ret = tmp;
	}
	return ret;
}

void flir_get_camera_part(uint8_t* partnumber){
	int i;
	
	answ_ready = 0;
	rx_cnt = 0;

	make_cmd_uni(0x66, 0, 0);
	HAL_UART_Transmit(FLIR_HUART, cmd_p, 10, 0xFFFF);
	flir_wait(FLIR_RX_BUFF_POINTER);

	if (answ_ready > 0) {
		if (*(FLIR_RX_BUFF_POINTER + 1) != 0) {
			answ_ready = 0;
			partnumber[0] = 0;
			return;
		}
		if (*(FLIR_RX_BUFF_POINTER + 3) != 0x66) {
			answ_ready = 0;
		}
	}
	
	for (i=0;i<31;i++) {
		partnumber[i] = FLIR_RX_BUFF_POINTER[i+8];
	}
	
	return;
}

//uint64_t flir_get_serial_new(void){
//	int i;
//	uint64_t res = 0;
//	
//	answ_ready = 0;
//	rx_cnt = 0;

//	make_cmd_uni(0x65, 0, 0);
//	HAL_UART_Transmit(FLIR_HUART, cmd_p, 10, 0xFFFF);
//	flir_wait(FLIR_RX_BUFF_POINTER);

//	if (answ_ready > 0) {
//		if (*(FLIR_RX_BUFF_POINTER + 1) != 0) {
//			answ_ready = 0;
//			return 0;
//		}
//		if (*(FLIR_RX_BUFF_POINTER + 3) != 0x65) {
//			answ_ready = 0;
//		}
//	}
//	
//	for (i=0;i<8;i++) {
//		res = (res << 8) + FLIR_RX_BUFF_POINTER[i+8];
//	}
//	
//	return res;
//}

void flir_get_serial(SerialsStructTypeDef* ser){
	int i;
	uint32_t res = 0;
	
	answ_ready = 0;
	rx_cnt = 0;

	make_cmd_uni(0x04, 0, 0);
	HAL_UART_Transmit(FLIR_HUART, cmd_p, 10, 0xFFFF);
	flir_wait(FLIR_RX_BUFF_POINTER);

	if (answ_ready > 0) {
		if (*(FLIR_RX_BUFF_POINTER + 1) != 0) {
			answ_ready = 0;
			return;
		}
		if (*(FLIR_RX_BUFF_POINTER + 3) != 0x04) {
			answ_ready = 0;
		}
	}
	
	for (i=0;i<4;i++) {
		res = (res << 8) + FLIR_RX_BUFF_POINTER[i+8];
	}
	ser->camera = res;
	
	res = 0;
	for (i=0;i<4;i++) {
		res = (res << 8) + FLIR_RX_BUFF_POINTER[i+8+4];
	}
	ser->sensor = res;
	
	return;
}


void flir_get_revision(WareStructTypeDef* ware){
	uint16_t res = 0;
	
	answ_ready = 0;
	rx_cnt = 0;

	make_cmd_uni(0x05, 0, 0);
	HAL_UART_Transmit(FLIR_HUART, cmd_p, 10, 0xFFFF);
	flir_wait(FLIR_RX_BUFF_POINTER);

	if (answ_ready > 0) {
		if (*(FLIR_RX_BUFF_POINTER + 1) != 0) {
			answ_ready = 0;
			return;
		}
		if (*(FLIR_RX_BUFF_POINTER + 3) != 0x05) {
			answ_ready = 0;
		}
	}
	
	res = FLIR_RX_BUFF_POINTER[8+0];
	res = (res << 8) + FLIR_RX_BUFF_POINTER[8+1];
	ware->major_software = res;
	
	res = FLIR_RX_BUFF_POINTER[8+2];
	res = (res << 8) + FLIR_RX_BUFF_POINTER[8+3];
	ware->minor_software = res;

	res = FLIR_RX_BUFF_POINTER[8+4];
	res = (res << 8) + FLIR_RX_BUFF_POINTER[8+5];
	ware->major_firmware = res;

	res = FLIR_RX_BUFF_POINTER[8+6];
	res = (res << 8) + FLIR_RX_BUFF_POINTER[8+7];
	ware->minor_firmware = res;

	return;
}


uint16_t flir_get_splash_screen_delay(void){
	uint16_t tmp = 0;
	uint16_t ret = 0;
	
	answ_ready = 0;
	rx_cnt = 0;
	make_cmd_uni(0x31, 0, 0);
	HAL_UART_Transmit(FLIR_HUART, cmd_p, 12, 0xFFFF);
	flir_wait(FLIR_RX_BUFF_POINTER);
	if (answ_ready > 0){
		tmp = *(FLIR_RX_BUFF_POINTER + 8) << 8;
		tmp += *(FLIR_RX_BUFF_POINTER + 9);
		ret = tmp;
	}
	if (answ_ready > 0) {
		if (*(FLIR_RX_BUFF_POINTER + 1) != 0) {
			answ_ready = 0;
		}
		if (*(FLIR_RX_BUFF_POINTER + 3) != 0x31) {
			answ_ready = 0;
		}
	}
	return ret;
}


uint8_t flir_test(void){

	answ_ready = 0;

	rx_cnt = 0;
	make_cmd_uni(5, 0, 0);
	HAL_UART_Transmit(FLIR_HUART, cmd_p, 10, 0xFFFF);
	
	flir_wait(FLIR_RX_BUFF_POINTER);
	if (answ_ready > 0) {
		if (*(FLIR_RX_BUFF_POINTER + 1) != 0) {
			answ_ready = 0;
		}
		if (*(FLIR_RX_BUFF_POINTER + 3) != 5) {
			answ_ready = 0;
		}
	}
	return answ_ready;
}

uint8_t flir_getid(void){

	answ_ready = 0;

	rx_cnt = 0;
	make_cmd_uni(5, 0, 0);
	HAL_UART_Transmit(FLIR_HUART, cmd_p, 10, 0xFFFF);
	flir_wait(FLIR_RX_BUFF_POINTER);
	
	if (answ_ready > 0) {
		if (*(FLIR_RX_BUFF_POINTER + 1) != 0) {
			answ_ready = 0;
		}
		if (*(FLIR_RX_BUFF_POINTER + 3) != 5) {
			answ_ready = 0;
		}
	}
	return answ_ready;
}

void flir_wait(uint8_t* ans_p){
	//HAL_Delay(FLIR_SHORT_DELAY);
	uint16_t i = 0;
	while (!answ_ready && i++ < FLIR_WAIT){
		HAL_Delay(1);
	}
	printf(" delay:%d, rx_cnt=%d\n\r", i, rx_cnt);
#ifdef FLIR_DEBUG
	print_buf_in_hex(ans_p, rx_cnt);
#endif
}
