#ifndef __flir_H
#define __flir_H

#include "stdint.h"
#include "crc.h"
#include "stdio.h"
#include "stm32f1xx_hal.h"

#define FLIR_SHORT_DELAY 200
#define FLIR_AUTOSAVE 
#define FLIR_FIRST_BYTE 0x6E
#define FLIR_WAIT 1000
#define FLIR_CMDLEN 28
//#define FLIR_DEBUG
#define FLIR_HUART &huart1
#define FLIR_RX_BUFF_POINTER ui_rx_buff

typedef struct{
	uint32_t camera;
	uint32_t sensor;
} SerialsStructTypeDef;

typedef struct{
	uint16_t minor_firmware;
	uint16_t major_firmware;
	uint16_t minor_software;
	uint16_t major_software;
} WareStructTypeDef;

void print_buf_in_hex(uint8_t* buf, uint8_t len);
void clr_buf(uint8_t* buf, uint8_t len);
//void make_cmd(uint8_t* cmd_p, uint8_t cmd, uint8_t data_len, uint16_t data);
void make_cmd_uni(uint8_t cmd, uint8_t data_len, uint8_t* data);
void make_cmd_word(uint8_t cmd, uint16_t data);

void flir_set_standart(uint16_t data);
void flir_set_palette(uint16_t data);
void flir_set_orientation(uint16_t data);
void flir_save(void);
void flir_set_zoom(uint16_t data);
void flir_set_brightness(uint16_t data);
void flir_set_agg(uint16_t data);
void flir_ezoom_down(uint8_t step);
void flir_ezoom_up(uint8_t step);
uint8_t flir_test(void);
void flir_wait(uint8_t* ans_p);
void flir_set_ezoom(uint16_t zoom);
void flir_set_tilt_pan(int16_t tilt, int16_t pan);
void flir_calibrate(void);
void flir_rectangle(uint8_t cmd_id, uint16_t x, 
	                  uint16_t y, uint16_t width, uint16_t height, uint8_t back_color, uint8_t color, uint16_t outline);
void flir_bar(uint8_t cmd_id, uint16_t x, 
	            uint16_t y, uint16_t width, uint16_t height, uint8_t back_color, uint8_t color);
void flir_text(uint8_t cmd_id, uint16_t x, 
	            uint16_t y, uint16_t align, uint16_t font, uint8_t back_color, uint8_t color, const uint8_t* text, uint8_t l);

void flir_clear(uint8_t cmd_id);


void flir_get_tilt_pan(int16_t* tilt, int16_t* pan);

void flir_get_camera_part(uint8_t* str);
void flir_get_serial(SerialsStructTypeDef* ser);
void flir_get_revision(WareStructTypeDef* ware);

uint16_t flir_get_max_zoom(void);
uint16_t flir_get_brightness(void);
uint8_t flir_get_zoom(void);
uint8_t flir_get_palette(void);
uint16_t flir_get_cur_zoom(void);
uint16_t flir_get_temp(void);
uint16_t flir_get_brightness_bias(void);

uint16_t flir_get_splash_screen_delay(void);

#endif /*__ flir_H */

