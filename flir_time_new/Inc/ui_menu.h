#ifndef __ui_menu_H
#define __ui_menu_H

#include "flir.h"
#include "core.h"
#include "stm32f1xx_hal.h"

enum {
	MENU_LAST=0,
	MENU_LABEL,
	MENU_SUB,
	MENU_EXEC,
};

#pragma anon_unions
struct menuitem_t {
	const uint8_t *text;
	uint8_t type;
	uint8_t len;
  const uint8_t *header;
	uint8_t header_len;
	void (*callback)(void);
	union {
		const struct menuitem_t* sub_menu;
		uint32_t *val;
		const uint32_t num; /* first label contains number of items :) */
	};
};

void menu_draw(void);
void menu_hide(void);
void menu_up(void);
void menu_down(void);
void menu_select(void);
void menu_main(void);

void menu_item_info(void);
void menu_item_sight(void);
void menu_item_pos(void);

void menu_item_pal_1(void);
void menu_item_pal_2(void);
void menu_item_pal_3(void);
void menu_item_pal_4(void);
void menu_item_pal_5(void);
void menu_item_pal_6(void);
	
#endif /*__ ui_menu_H */
