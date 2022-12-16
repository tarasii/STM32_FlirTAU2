#include "flir.h"
#include "stm32f1xx_hal.h"

#define PALLEN   6
#define ZOOM_DIV 8
#define BRI_STEP 50
#define MIN_ZOOM 80 //min value = max zoom

void opt_init(UART_HandleTypeDef *huart, uint8_t *rx_buff);
void opt_pallette_up(UART_HandleTypeDef *huart, uint8_t *rx_buff);
void opt_pallette_down(UART_HandleTypeDef *huart, uint8_t *rx_buff);
void opt_brightness_up(UART_HandleTypeDef *huart, uint8_t *rx_buff);
void opt_brightness_down(UART_HandleTypeDef *huart, uint8_t *rx_buff);
void opt_zoom_up(UART_HandleTypeDef *huart, uint8_t *rx_buff);
void opt_zoom_down(UART_HandleTypeDef *huart, uint8_t *rx_buff);
void opt_aim1(UART_HandleTypeDef *huart, uint8_t *rx_buff, uint8_t color, uint8_t backcolor);
void opt_aim2(UART_HandleTypeDef *huart, uint8_t *rx_buff, uint8_t color);
void opt_clear(UART_HandleTypeDef *huart, uint8_t *rx_buff);	
void opt_save(UART_HandleTypeDef *huart, uint8_t *rx_buff);	
void opt_test_colors(UART_HandleTypeDef *huart, uint8_t *rx_buff);
void opt_agg(UART_HandleTypeDef *huart, uint8_t *rx_buff);


