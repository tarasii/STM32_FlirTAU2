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


void print_buf_in_hex(uint8_t* buf, uint8_t len);
void clr_buf(uint8_t* buf, uint8_t len);
//void make_cmd(uint8_t* cmd_p, uint8_t cmd, uint8_t data_len, uint16_t data);
void make_cmd_uni(uint8_t cmd, uint8_t data_len, uint8_t* data);
void make_cmd_word(uint8_t cmd, uint16_t data);

void flir_set_standart(UART_HandleTypeDef *huart, uint8_t* ans_p, uint16_t data);
void flir_set_palette(UART_HandleTypeDef *huart, uint8_t* ans_p, uint16_t data);
void flir_save(UART_HandleTypeDef *huart, uint8_t* ans_p);
void flir_set_zoom(UART_HandleTypeDef *huart, uint8_t* ans_p, uint16_t data);
void flir_set_brightness(UART_HandleTypeDef *huart, uint8_t* ans_p, uint16_t data);
void flir_set_agg(UART_HandleTypeDef *huart, uint8_t* ans_p, uint16_t data);
void flir_ezoom_down(UART_HandleTypeDef *huart, uint8_t* ans_p, uint8_t step);
void flir_ezoom_up(UART_HandleTypeDef *huart, uint8_t* ans_p, uint8_t step);
uint8_t flir_test(UART_HandleTypeDef *huart, uint8_t* ans_p);
void flir_wait(uint8_t* ans_p);
void flir_set_ezoom(UART_HandleTypeDef *huart, uint8_t* ans_p, uint16_t zoom);
void flir_calibrate(UART_HandleTypeDef *huart, uint8_t* ans_p);
void flir_rectangle(UART_HandleTypeDef *huart, uint8_t* ans_p, uint8_t cmd_id, uint16_t x, 
	                  uint16_t y, uint16_t width, uint16_t height, uint8_t back_color, uint8_t color, uint16_t outline);
void flir_bar(UART_HandleTypeDef *huart, uint8_t* ans_p, uint8_t cmd_id, uint16_t x, 
	            uint16_t y, uint16_t width, uint16_t height, uint8_t back_color, uint8_t color);
void flir_clear(UART_HandleTypeDef *huart, uint8_t* ans_p, uint8_t cmd_id);


uint16_t flir_get_max_zoom(UART_HandleTypeDef *huart, uint8_t* ans_p);
uint16_t flir_get_brightness(UART_HandleTypeDef *huart, uint8_t* ans_p);
uint8_t flir_get_zoom(UART_HandleTypeDef *huart, uint8_t* ans_p);
uint8_t flir_get_palette(UART_HandleTypeDef *huart, uint8_t* ans_p);
uint16_t flir_get_cur_zoom(UART_HandleTypeDef *huart, uint8_t* ans_p);

uint16_t flir_get_splash_screen_delay(UART_HandleTypeDef *huart, uint8_t* ans_p);



