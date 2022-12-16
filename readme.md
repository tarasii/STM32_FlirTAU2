STM32 Flir TAU2 library

Attention, wrong commands can harm your device.
This example shows an idea how it could be. 
Usage of this code is at your own risk.
Author is not responsible for the consequences.

flir_common - library

flir_time_new/MDK-ARM/flir_time.uvprojx - project for Keil-ARM

flir_time_new/flir_time_new.ioc - STM_Cube project


Flir TAU2 commands:

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



