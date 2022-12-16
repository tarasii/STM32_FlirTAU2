#ifndef __core_H
#define __core_H

#include "flir.h"
#include "stm32f1xx_hal.h"
//#include "ui_menu.h"

#define PALETTE   6
#define EZOOM_DIV 8
#define BRI_STEP  50
#define MIN_EZOOM 80 //min value = max zoom

#define CIRCLE_ZOOM

void opt_init(void);
void opt_pallette_up(void);
void opt_pallette_down(void);
void opt_pallette_set(void);
void opt_pallette_get(uint8_t pal);

void opt_pallette_step_down(void);
void opt_pallette_step_up(void);

void opt_brightness_up(void);
void opt_brightness_down(void);
void opt_ezoom_up(void);
void opt_ezoom_down(void);
void opt_zoom_up(void);
void opt_zoom_down(void);
void opt_aim1(uint8_t color, uint8_t backcolor);
void opt_aim2(uint8_t color);
void opt_clear(void);	
void opt_save(void);	
void opt_test_colors(void);
void opt_agg(void);
void opt_tilt_up(void);
void opt_tilt_down(void);
void opt_pan_up(void);
void opt_pan_down(void);

void opt_sight0(void);
void opt_sight1(void);
void opt_sight2(void);
void opt_sight3(void);
void opt_sight4(void);

void opt_rot0(void);
void opt_rot1(void);
void opt_rot2(void);
void opt_rot3(void);

void opt_sight_up(void);
void opt_sight_down(void);

void opt_draw_pan_tilt(uint8_t cmd_id, uint16_t x, uint16_t y, 
							 uint16_t align, uint16_t font, uint8_t back_color, uint8_t color);

void opt_draw_palette(uint8_t cmd_id, uint16_t x, uint16_t y, 
							 uint16_t align, uint16_t font, uint8_t back_color, uint8_t color);


#endif /*__ core_H */
