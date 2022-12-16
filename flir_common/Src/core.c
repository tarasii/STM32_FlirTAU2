#include "core.h"


extern uint8_t rx_cnt;
extern uint8_t *ui_rx_buff;

//const uint8_t plts[PALETTE] = {0, 1, 2, 7, 3, 0x10};
uint8_t plts[PALETTE] = {0, 1, 2, 7, 3, 0x10};

uint8_t plt_mode = 0;
uint8_t plt_cur = 0;

uint16_t bright_cur = 0;
uint8_t agg_mode = 0;
uint8_t bright_mode = 5;

uint16_t ezoom_div = 1;
uint16_t zoom_max = 3;
uint16_t ezoom_max = 4;
uint16_t ezoom_cur = 0;
uint16_t zoom_cur = 0;

uint8_t cmd_pal = 0;
uint8_t cmd_ezoom = 0;
uint8_t cmd_bri = 0;
uint8_t cmd_tilt = 0;

uint8_t ed_pal = 0;
uint8_t ed_plt_mode = 0;

int16_t tilt;
int16_t pan;

extern uint16_t to_save;
extern uint8_t answ_ready;

void (*aims[5])(void)={opt_sight0, opt_sight1, opt_sight2, opt_sight3, opt_sight4};
uint8_t aim_cur = 0;

void opt_init(void){
	uint8_t i;
	//uint32_t tmp1, tmp2;
	SerialsStructTypeDef ser;
	WareStructTypeDef ware;

	uint8_t str[32];
	
	//spl_scr = flir_get_splash_screen_delay(huart, rx_buff);
	//printf("splash screen %d\n\r", spl_scr);
	
	flir_set_agg(2);
	printf("agg on\n\r");
  HAL_Delay(100);
	
	plt_cur = flir_get_palette();
	if (answ_ready == 0) {
		cmd_pal = 0;
	  printf("palette read error\n\r");
	}		
	else {
		cmd_pal = 1;	
		for (i=0; i<PALETTE; i++){
			if (plts[i] == plt_cur) {
				plt_mode = i;
				break;
			}
		}
	  printf("palette:%d, mod:%d\n\r", plt_cur, plt_mode);
	}
	
	ezoom_max = flir_get_max_zoom();
	if (answ_ready == 0) {
		cmd_ezoom = 0;
	  printf("ezoom read error\n\r");
	}		
	else {
		cmd_ezoom = 1;	
		printf("max ezoom:%d\n\r", zoom_max);
	}
	//if (zoom_max == 0) zoom_max = 80;

	ezoom_cur = flir_get_cur_zoom();
	printf("cur ezoom:%d\n\r", ezoom_cur);
  if (ezoom_cur == 0) ezoom_cur = ezoom_max;	
	ezoom_div = ezoom_max / EZOOM_DIV;
	ezoom_cur = (ezoom_cur / ezoom_div) * (ezoom_div);
	printf("div ezoom:%d\n\r", ezoom_cur);	
	
	//flir_set_zoom(1);	
	zoom_cur = flir_get_zoom();
	printf("cur zoom:%d %d\n\r", zoom_cur, zoom_max);
	
	flir_get_tilt_pan(&tilt, &pan);
	if (answ_ready == 0) {
	  printf("tilt pen read error\n\r");
		tilt=0; pan=0;
	}		
	else {
		cmd_tilt = 1;
		printf("tilt:%d; pen:%d;\n\r", tilt, pan);
	}
	
	flir_get_serial(&ser);
	printf("Serial camera:%d %x sensor:%d %x\n\r", ser.camera , ser.camera, ser.sensor, ser.sensor);

	
	flir_get_revision(&ware);	
	printf("Software:%d-%d firmware:%d-%d\n\r", ware.minor_software, ware.major_software, ware.minor_firmware, ware.major_firmware);

	flir_get_camera_part(str);
	printf("Part:%s\n\r",str);


}

void opt_pallette_up(void){
	if (cmd_pal == 0) return;
	plt_mode++;
	if (plt_mode >= 6) plt_mode = 0;
	flir_set_palette(plts[plt_mode]);
	//to_save = 1;
	printf("palette:%d %d\n\r", plt_mode, plts[plt_mode]);
	
}

void opt_pallette_down(void){
	if (cmd_pal == 0) return;
	if (plt_mode == 0) plt_mode = 6;
	plt_mode--;
	flir_set_palette(plts[plt_mode]);
	printf("palette:%d %d\n\r", plt_mode, plts[plt_mode]);

	//to_save = 1;
}

void opt_pallette_get(uint8_t pal){
	flir_set_palette(plts[pal]);
	ed_pal = plts[pal];
	ed_plt_mode = pal;
}

void opt_pallette_set(void){
	plts[ed_plt_mode] = ed_pal;
}

void opt_pallette_step_down(void){
	if (cmd_pal == 0) return;
	if (ed_pal > 0) ed_pal--;
	flir_set_palette(ed_pal);
	//to_save = 1;
}

void opt_pallette_step_up(void){
	if (cmd_pal == 0) return;
	if (ed_pal < 64) ed_pal++;
	flir_set_palette(ed_pal);
	//to_save = 1;
}

void opt_draw_palette(uint8_t cmd_id, uint16_t x, uint16_t y, 
							 uint16_t align, uint16_t font, uint8_t back_color, uint8_t color){
	uint8_t str[31];
	sprintf((char *)str,"palette: %03d %03d", ed_plt_mode, ed_pal);
	flir_text(cmd_id, x, y, align, font, back_color, color, str, 19);
}

void opt_draw_pan_tilt(uint8_t cmd_id, uint16_t x, uint16_t y, 
							 uint16_t align, uint16_t font, uint8_t back_color, uint8_t color){
	uint8_t str[31];
	sprintf((char *)str,"tilt: %03d, pan: %03d", tilt, pan);
	flir_text(cmd_id, x, y, align, font, back_color, color, str, 19);
}


void opt_tilt_up(void){
	if (cmd_tilt == 0) return;
	tilt++;
	if (tilt>= 40) tilt = 40;
	flir_set_tilt_pan(tilt, pan);
	to_save = 1;
}

void opt_tilt_down(void){
	if (cmd_tilt == 0) return;
	tilt--;
	if (tilt <= -40) tilt = -40;
	flir_set_tilt_pan(tilt, pan);
	to_save = 1;
}

void opt_pan_up(void){
	if (cmd_tilt == 0) return;
	pan++;
	if (pan >= 40) pan = 40;
	flir_set_tilt_pan(tilt, pan);
	to_save = 1;
}

void opt_pan_down(void){
	if (cmd_tilt == 0) return;
	pan--;
	if (pan <= -40) pan = -40;
	flir_set_tilt_pan(tilt, pan);
	to_save = 1;
}


void opt_brightness_up(void){
	if (cmd_bri == 0) { 
		bright_cur = flir_get_brightness();
		if (answ_ready == 0) {
	//	cmd_bri = 0;
			printf("brightness read error\n\r");
			return;
		}		
		else {
			if (bright_cur == 0) return;
			cmd_bri = 1;	
			printf("brightness:%d\n\r", bright_cur);
		}
	}
	if (bright_mode == 0) bright_mode = 1;
	bright_mode--;
	if (agg_mode == 0){
		// set manual AGG if brightness - pressed
		agg_mode = 1;
		flir_set_agg(3);
	}
	printf("br:%d %d %d\n\r", bright_cur, bright_cur + (bright_mode-5) * BRI_STEP, bright_mode);
	if (bright_cur + bright_mode * BRI_STEP > 250) {
		flir_set_brightness(bright_cur + (bright_mode - 5) * BRI_STEP);
	}
}

void opt_brightness_down(void){
	if (cmd_bri == 0) { 
		bright_cur = flir_get_brightness();
		if (answ_ready == 0) {
	//	cmd_bri = 0;
			printf("brightness read error\n\r");
			return;
		}		
		else {
			if (bright_cur == 0) return;
			cmd_bri = 1;	
			printf("brightness:%d\n\r", bright_cur);
		}
	}
	bright_mode++;
	if (bright_mode > 10) bright_mode = 10;
	if (agg_mode == 0){
		// set manual AGG if brightness - pressed
		agg_mode = 1;
		flir_set_agg(3);
	} 
	printf("br:%d %d %d\n\r", bright_cur, bright_cur + (bright_mode-5) * BRI_STEP, bright_mode);
	if (bright_cur + bright_mode * BRI_STEP > 250) {
		flir_set_brightness(bright_cur + (bright_mode-5) * BRI_STEP);
	}
}

void opt_ezoom_up(void){
	if (cmd_ezoom == 0) return;
	ezoom_cur += ezoom_div;
#ifdef CIRCLE_ZOOM
	if (ezoom_cur > ezoom_max) ezoom_cur = MIN_EZOOM;
#else
	if (ezoom_cur > ezoom_max) ezoom_cur = ezoom_max;
#endif
	flir_set_ezoom(ezoom_cur);
	to_save = 1;
}

void opt_ezoom_down(void){	
	if (cmd_ezoom == 0) return;
	ezoom_cur -= ezoom_div;
#ifdef CIRCLE_ZOOM
	if (ezoom_cur <= MIN_EZOOM) ezoom_cur = ezoom_max;
#else
	if (ezoom_cur <= MIN_EZOOM) ezoom_cur = MIN_EZOOM;
#endif
	flir_set_ezoom(ezoom_cur);
	to_save = 1;
}


void opt_zoom_up(void){
	zoom_cur += 1;
#ifdef CIRCLE_ZOOM
	if (zoom_cur > zoom_max) zoom_cur = 0;
#else
	if (zoom_cur > zoom_max) zoom_cur = zoom_max;
#endif
	flir_set_zoom(zoom_cur);
	printf("cur zoom:%d %d\n\r", zoom_cur, zoom_max);
	if (answ_ready == 0) 
	{
		zoom_max = --zoom_cur;
#ifdef CIRCLE_ZOOM
		zoom_cur = 0;
		flir_set_zoom(zoom_cur);
		printf("cur zoom:%d %d\n\r", zoom_cur, zoom_max);
#endif
	}
	to_save = 1;
}

void opt_zoom_down(void){	
#ifdef CIRCLE_ZOOM
	if (zoom_cur == 0) zoom_cur = zoom_max; else zoom_cur -= 1;
#else
	if (zoom_cur == 0) zoom_cur = 0; else zoom_cur -= 1;
#endif
	answ_ready = 0;
	while (answ_ready == 0){
		flir_set_zoom(zoom_cur);
		printf("cur zoom:%d %d\n\r", zoom_cur, zoom_max);
		if (answ_ready == 0) 
		{
			zoom_max = --zoom_cur;
		}
	}
	to_save = 1;
}

void opt_aim1(uint8_t color, uint8_t backcolor){	
	//flir_bar(huart, rx_buff, 98, 319, 140, 1, 200, 0, backcolor);			
	//flir_bar(huart, rx_buff, 97, 220, 239, 200, 1, 0, backcolor);
	flir_bar(90, 320, 140, 2, 200, 0, color);			
	flir_bar(89, 220, 240, 200, 2, 0, color);
	flir_clear(88);
	flir_clear(87);
	printf("cross\n\r");
}

void opt_aim2(uint8_t color){	
	flir_bar(90, 320, 250, 2, 90, 0, color);		
	flir_bar(89, 220, 240, 90, 2, 0, color);
	flir_bar(88, 330, 240, 90, 2, 0, color);
	flir_bar(87, 319, 239, 2, 2, 0, color);
	printf("aim\n\r");
}

void opt_sight0(void){
	opt_clear();
}
void opt_sight1(void){
	opt_aim1(255, 1);
}
void opt_sight2(void){
	opt_aim1(1, 255);
}
void opt_sight3(void){
	opt_aim2(255);
}
void opt_sight4(void){
	opt_aim2(1);
}

void opt_rot0(void){
	flir_set_orientation(0);
}

void opt_rot1(void){
	flir_set_orientation(1);
}

void opt_rot2(void){
	flir_set_orientation(2);
}

void opt_rot3(void){
	flir_set_orientation(3);
}

void opt_sight_up(void){
	aim_cur++;
	if (aim_cur > 4) aim_cur = 0;
	aims[aim_cur]();
}

void opt_sight_down(void){
	if (aim_cur == 0) aim_cur = 4; else	aim_cur--;
	aims[aim_cur]();
}


void opt_clear(void){	
	flir_clear(91);
	flir_clear(90);
	flir_clear(89);
	flir_clear(88);
	flir_clear(87);
	printf("clear\n\r");
}

void opt_test_colors(void){	
	//uint8_t i;
	//for (i=50; i<100; i++){
	//	flir_bar(huart, rx_buff, 20+10*i, 40, 10, 10, 10, 0, i);		
	//}
	//flir_rectangle(huart, rx_buff, 99, 10, 30, 610, 445, 100, 0xff, 1);			
	flir_rectangle(99, 7, 28, 616, 451, 100, 0xff, 1);			
	//for (i=0; i<30; i++){
	//	flir_bar(huart, rx_buff, 98-i, 8+i*20, 458, 20, 20, 0, (i+1)*4);		
	//}
	//for (i=0; i<30; i++){
	//	flir_bar(huart, rx_buff, 68-i, 8+i*20, 438, 20, 20, 0, (i+31)*4);		
	//}			

//	flir_bar(huart, rx_buff, 98, 50, 50, 20, 20, 100, 0);	//0 - transperent		
	flir_bar(37, 8, 29, 20, 20, 0, 1);		//1 - black
	flir_bar(36, 8, 49, 20, 20, 0, 255); //255 - white			
	flir_bar(35, 8, 69, 20, 20, 0, 64); //64 - green			
	flir_bar(34, 8, 89, 20, 20, 0, 128); //128 - red			
	flir_bar(33, 8, 109, 20, 20, 0, 8); //8 - blue		
	flir_bar(32, 8, 129, 20, 20, 0, 144); //144 - purple				
	flir_bar(31, 8, 149, 20, 20, 0, 180); //180 - darck grey				
	flir_bar(30, 8, 169, 20, 20, 0, 184); //184 - grey
	flir_bar(29, 8, 189, 20, 20, 0, 192); //192 - light grey
	
	printf("test colors\n\r");
}


void opt_save(void){	
	if (ui_rx_buff[1] != 0) return;
	flir_calibrate();				
	printf("calibrate\n\r");	
	HAL_Delay(100);
	flir_save();	
	printf("save\n\r");
}

void opt_agg(void){
	
	flir_set_agg(2);
	printf("agg on\n\r");
  HAL_Delay(100);
	
	bright_cur = flir_get_brightness();
	if (answ_ready == 0) {
		cmd_bri = 0;
	  printf("brightness read error\n\r");
	}		
	else {
		cmd_bri = 1;	
		printf("brightness:%d\n\r", bright_cur);
	}
	agg_mode = 0;
	bright_mode = 5;
}


