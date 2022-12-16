#include "ui_menu.h"

static const struct menuitem_t menuPos[] = {
	{
		.text = "x2",
		.type = MENU_LABEL,
		.len = 2,
		.callback = 0,
		{
			.num = 3,
		},
	},
	{
		.text = "x4",
		.type = MENU_LABEL,
		.len = 2,
		.callback = 0,
	},
	{
		.text = "x8",
		.type = MENU_LABEL,
		.len = 2,
		.callback = 0,
	},
};

static const struct menuitem_t menuPal[] = {
	{
		.text = "1",
		.type = MENU_EXEC,
		.len = 1,
		.header = "PALETTE:",
		.header_len = 8,		
		.callback = menu_item_pal_1,
		{
			.num = 7,

		},
	},
	{
		.text = "2",
		.type = MENU_EXEC,
		.len = 1,
		.callback = menu_item_pal_2,
	},
	{
		.text = "3",
		.type = MENU_EXEC,
		.len = 1,
		.callback = menu_item_pal_3,
	},
	{
		.text = "4",
		.type = MENU_EXEC,
		.len = 1,
		.callback = menu_item_pal_4,
	},
	{
		.text = "5",
		.type = MENU_EXEC,
		.len = 1,
		.callback = menu_item_pal_5,
	},
	{
		.text = "6",
		.type = MENU_EXEC,
		.len = 1,
		.callback = menu_item_pal_6,
	},
	{
		.text = "EXIT",
		.type = MENU_LABEL,
		.len = 4,
		.callback = 0,
	}
};

static const struct menuitem_t menuRot[] = {
	{
		.text = "NORMAL",
		.type = MENU_EXEC,
		.len = 6,
		.header = "ORIENTATION:",
		.header_len = 12,
		.callback = opt_rot0,
		{
			.num = 5,
		},
	},
	{
		.text = "INVERT",
		.type = MENU_EXEC,
		.len = 6,
		.callback = opt_rot1,
	},
	{
		.text = "REVERT",
		.type = MENU_EXEC,
		.len = 6,
		.callback = opt_rot2,
	},
	{
		.text = "BOTH",
		.type = MENU_EXEC,
		.len = 4,
		.callback = opt_rot3,
	},
	{
		.text = "EXIT",
		.type = MENU_LABEL,
		.len = 4,
		.callback = 0,
	},
};

static const struct menuitem_t mainMenu[] = {
	{
		.text = "INFO",
		.type = MENU_EXEC,
		.len = 4,
		.header = "SETTINGS:",
		.header_len = 9,
		.callback = menu_item_info,
		{
			.num = 7,

		},
	},
	{
		.text = "SIGHT",
		.type = MENU_EXEC,
		.len = 5,
		.callback = menu_item_sight,
	},
	{
		.text = "POSITION",
		.type = MENU_EXEC,
		.len = 8,
		.callback = menu_item_pos,
		{
			.sub_menu = menuPos,
		},
	},
	{
		.text = "ORIENTATION",
		.type = MENU_SUB,
		.len = 11,
		.callback = 0,
		{
			.sub_menu = menuRot,
		},
	},
	{
		.text = "PALETTE",
		.type = MENU_SUB,
		.len = 7,
		.callback = 0,
		{
			.sub_menu = menuPal,
		},
	},
	{
		.text = "BRIGHTNESS",
		.type = MENU_EXEC,
		.len = 11,
		.callback = opt_agg,
	},
	{
		.text = "EXIT",
		.type = MENU_LABEL,
		.len = 5,
		.callback = 0,
	},
};


//extern uint8_t rx_cnt;
const struct menuitem_t *cur_menu = mainMenu;
uint8_t cur_menu_pos = 0;
extern uint8_t keys_mode;

void menu_draw(void)
{
	uint8_t i, l, bc;
	l = cur_menu[0].num;
	flir_text(99, 20, 25, 1, 0, 0, 255, cur_menu[0].header, cur_menu[0].header_len);
	for (i=0; i<l; i++){
		if (i==cur_menu_pos) bc = 1; else bc = 0;
		flir_text(98 - i, 80, 80 + 40 * i,  1, 0, bc, 255, cur_menu[i].text,  cur_menu[i].len);
	}

	//flir_text(huart, rx_buff, 98, 80, 80,  1, 0, 0, 255, "Info",         4);
	//flir_text(huart, rx_buff, 97, 80, 120, 1, 0, 1, 255, "Grid",         4);
	//flir_text(huart, rx_buff, 96, 80, 160, 1, 0, 0, 255, "POSITION",     8);
	//flir_text(huart, rx_buff, 95, 80, 200, 1, 0, 0, 255, "Rotation",     8);
	//flir_text(huart, rx_buff, 94, 80, 240, 1, 0, 0, 255, "Palette",      7);
	//flir_text(huart, rx_buff, 93, 80, 280, 1, 0, 0, 255, "Brightness ", 11);
	//flir_text(huart, rx_buff, 92, 80, 320, 1, 0, 0, 255, "Reset",        5);
	
}

void menu_down(void)
{
	uint8_t i, l, bc;
	l = cur_menu[0].num;
	i = cur_menu_pos;
	bc = 0;
	flir_text(98 - i, 80, 80 + 40 * i,  1, 0, bc, 255, cur_menu[i].text,  cur_menu[i].len);
	if (++cur_menu_pos > l - 1) cur_menu_pos = 0;
	i = cur_menu_pos;
	bc = 1;
	flir_text(98 - i, 80, 80 + 40 * i,  1, 0, bc, 255, cur_menu[i].text,  cur_menu[i].len);
}

void menu_up(void)
{
	uint8_t i, l, bc;
	l = cur_menu[0].num;
	i = cur_menu_pos;
	bc = 0;
	flir_text(98 - i, 80, 80 + 40 * i,  1, 0, bc, 255, cur_menu[i].text,  cur_menu[i].len);
	if (cur_menu_pos-- == 0) cur_menu_pos = l - 1; 
	i = cur_menu_pos;
	bc = 1;
	flir_text(98 - i, 80, 80 + 40 * i,  1, 0, bc, 255, cur_menu[i].text,  cur_menu[i].len);
}


void menu_hide(void){
	uint8_t i, l;
	l = cur_menu[0].num;
	for (i=0; i<l; i++){
		flir_clear(98-i);
	}
	flir_clear(99);
	
	//flir_clear(huart, rx_buff, 99);
	//flir_clear(huart, rx_buff, 98);
	//flir_clear(huart, rx_buff, 97);
	//flir_clear(huart, rx_buff, 96);
	//flir_clear(huart, rx_buff, 95);
	//flir_clear(huart, rx_buff, 94);
	//flir_clear(huart, rx_buff, 93);
	//flir_clear(huart, rx_buff, 92);
	//flir_clear(huart, rx_buff, 91);	
}

void menu_select(void){
	uint8_t i;
	i = cur_menu_pos;
	
	if (cur_menu[i].type == MENU_EXEC)
	{
		cur_menu[i].callback();
		//menu_hide();
		//cur_menu = mainMenu;
		//cur_menu_pos = 0;		
		//keys_mode = 0;
	} else if (cur_menu[i].type == MENU_SUB)
	{
		menu_hide();
		cur_menu = cur_menu[i].sub_menu;
		cur_menu_pos = 0;
		menu_draw();
	} else 
	{
		menu_hide();
		keys_mode = 0;
		cur_menu = mainMenu;
		cur_menu_pos = 0;		
	}
	
}

void menu_main(void){
	cur_menu = mainMenu;
	cur_menu_pos = 0;
}

void menu_item_info(void){
	//
	uint16_t bri, maxzoom, ezoom;
	uint8_t plt;
	//int16_t tilt, pan;
	uint8_t str[31];
	uint16_t tmp; 
	
	keys_mode = 2;
	bri = flir_get_brightness();
	
	menu_hide();
	tmp = flir_get_temp();
	flir_text(99, 20, 25, 1, 0, 0, 255, "INFO:", 6);	
	sprintf((char *)str,"brightness: %05d, temp: %03d", bri, tmp);
	flir_text(98, 80, 80, 1, 0, 0, 255, str, 29);
	
	maxzoom = flir_get_max_zoom();
	ezoom = flir_get_cur_zoom();	
	sprintf((char *)str,"ezoom: %05d, max: %05d", ezoom, maxzoom);
	flir_text(97, 80, 120, 1, 0, 0, 255, str, 24);
	
	plt = flir_get_palette();
	sprintf((char *)str,"palette: %02d", plt);
	flir_text(96, 80, 160, 1, 0, 0, 255, str, 11);
	
	//flir_get_tilt_pan(&tilt, &pan);
	//sprintf((char *)str,"tilt: %03d, pan: %03d", tilt, pan);
	opt_draw_pan_tilt(95, 80, 200, 1, 0, 0, 255);
	
	flir_get_camera_part(str);
	flir_text(94, 80, 240, 1, 0, 0, 255, str, 31);
	
}

void menu_item_sight(void){
	
	keys_mode = 3;
	menu_hide();
	flir_text(99, 20, 25, 1, 0, 0, 255, "SIGHT:", 6);

}

void menu_item_pos(void){
	
	keys_mode = 4;
	menu_hide();

	opt_draw_pan_tilt(99, 20, 25, 1, 0, 0, 255);

}

void menu_item_pal_1(void){
	
	keys_mode = 5;
	menu_hide();
	opt_pallette_get(0);
	opt_draw_palette(99, 20, 25, 1, 0, 0, 255);

}



void menu_item_pal_2(void){
	
	keys_mode = 5;
	menu_hide();
	opt_pallette_get(1);
	opt_draw_palette(99, 20, 25, 1, 0, 0, 255);

}

void menu_item_pal_3(void){
	
	keys_mode = 5;
	menu_hide();
	opt_pallette_get(2);
	opt_draw_palette(99, 20, 25, 1, 0, 0, 255);

}

void menu_item_pal_4(void){
	
	keys_mode = 5;
	menu_hide();
	opt_pallette_get(3);
	opt_draw_palette(99, 20, 25, 1, 0, 0, 255);

}

void menu_item_pal_5(void){
	
	keys_mode = 5;
	menu_hide();
	opt_pallette_get(4);
	opt_draw_palette(99, 20, 25, 1, 0, 0, 255);

}

void menu_item_pal_6(void){
	
	keys_mode = 5;
	menu_hide();
	opt_pallette_get(5);
	opt_draw_palette(99, 20, 25, 1, 0, 0, 255);

}



