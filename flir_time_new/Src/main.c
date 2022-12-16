
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_hal.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "flir.h"
#include "core.h"
#include "ui_menu.h"

#define  RXBUFLEN  64

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
uint32_t cur_tick = 0;
uint32_t tmp_tick = 0;
uint32_t sec_tick = 0;

uint8_t rx_cnt = 0;
uint8_t rx_data = 0;
uint8_t rx_buff[RXBUFLEN];
uint8_t answ_ready = 0;
uint8_t *ui_rx_buff;

uint16_t to_save = 0;

uint8_t keys_mode = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

	//uint8_t i;
	uint8_t btn_grp[3] = {0, 0 ,0};
  uint8_t btn = 0, tmp_btn = 0;
	uint8_t mode = 0;
	uint8_t cmd_init = 0;
	uint8_t res_test = 0;
	uint8_t cnt_init = 0;
	uint16_t btn_time = 0;
	
	//btn_grp[0] = 0; btn_grp[1] = 0; btn_grp[2] = 0;

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11|GPIO_PIN_12, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10, GPIO_PIN_SET);
	
	HAL_UART_Receive_IT(&huart1, &rx_data, 1);
  
	ui_rx_buff = rx_buff;
	
	printf("Hello!\n\r");
	
	btn = 0;
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
	if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11) == GPIO_PIN_RESET) btn = 4;
	else if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12) == GPIO_PIN_RESET) btn = 1;
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
	
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
	if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11) == GPIO_PIN_RESET) btn = 5;
	else if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12) == GPIO_PIN_RESET) btn = 2;
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
	if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11) == GPIO_PIN_RESET) btn = 6;
	else if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12) == GPIO_PIN_RESET) btn = 3;
	//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
	//btn = 3;
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_RESET);
	while ((HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11) == GPIO_PIN_RESET)||(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12) == GPIO_PIN_RESET)){
		//HAL_Delay(1);
	}
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10, GPIO_PIN_SET);

	printf("Button init %d\n\r", btn);
	
	HAL_Delay(1000);

	cur_tick = HAL_GetTick();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
		//test ping on start
		if (cmd_init == 0){
			res_test = flir_test();
			printf("test %d\n\r", res_test);
			if (res_test != 0) cmd_init = 1;
			else cmd_init = 2;
		}
		//init after ping reply
		if (cmd_init == 1){
			opt_init();
			
			//if (btn == 1) opt_clear();
			//if (btn == 2) opt_aim1(0xff, 1);
			//if (btn == 3) opt_aim2(0xff);
			//if (btn == 5) opt_aim1(1, 0xff);
			//if (btn == 6) opt_aim2(1,);
			//if (btn != 0) to_save = 1;
			btn = 0;
			cmd_init = 3;
			//opt_test_colors(&huart1, rx_buff);
		}

		cur_tick = HAL_GetTick();
		if (cur_tick - tmp_tick >= 20){
			//printf(" tick %d %d %d\n\r", cur_tick, tmp_tick, cur_tick - tmp_tick);
			tmp_tick = cur_tick;
			
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10, GPIO_PIN_SET);
			if      (mode == 0) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
			else if (mode == 1) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
			else if (mode == 2) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
			
			btn_grp[mode] = 0;
			if      (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11) == GPIO_PIN_RESET) btn_grp[mode] = 1;
			else if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12) == GPIO_PIN_RESET) btn_grp[mode] = 2;

			if      (mode == 0 && btn_grp[mode] == 1) btn = 1;
			else if (mode == 1 && btn_grp[mode] == 1) btn = 2;
			else if (mode == 2 && btn_grp[mode] == 1) btn = 3;
			else if (mode == 0 && btn_grp[mode] == 2) btn = 4;
			else if (mode == 1 && btn_grp[mode] == 2) btn = 5;
			else if (mode == 2 && btn_grp[mode] == 2) btn = 6;

			if (mode == 2) {
				if (btn_grp[0] == 0 && btn_grp[1] == 0 && btn_grp[2] == 0) 
				{
					btn = 0;
					if (tmp_btn) printf("Button up %d %d\n\r", tmp_btn, btn_time);
					
					if (keys_mode==0){
						if (btn_time > 60){
							//if (tmp_btn == 1) opt_agg  ();
							//if (tmp_btn == 2) opt_aim1 (0xff, 1);
							//if (tmp_btn == 3) opt_aim2 (0xff);
							//if (tmp_btn == 4) opt_clear();
							//if (tmp_btn == 5)	opt_aim1 (1, 0xff);
							//if (tmp_btn == 6)	opt_aim2 (1);
						}else if (btn_time > 5){
							if (tmp_btn == 1) opt_zoom_up        ();
							if (tmp_btn == 2) opt_brightness_up  ();
							//if (tmp_btn == 1) opt_tilt_up       ();
							if (tmp_btn == 3) opt_pallette_up    ();
							//if (tmp_btn == 3) opt_pan_up        ();
							//if (tmp_btn == 4) opt_zoom_down     ();
							if (tmp_btn == 4) {
								menu_draw();
								keys_mode = 1;
							}
							if (tmp_btn == 5)	opt_brightness_down();

							//if (tmp_btn == 4)	opt_tilt_down  ();
							if (tmp_btn == 6)	opt_pallette_down  ();
							//if (tmp_btn == 6)	opt_pan_down  ();
						}
					} else if (keys_mode==1) {
						//
							if (tmp_btn == 2) menu_up();
							if (tmp_btn == 5)	menu_down();					
							if (tmp_btn == 4) {
								menu_select();
							}
					} else if (keys_mode==2) {
						//
							if (tmp_btn == 4) {
								menu_hide();
								keys_mode = 0;
							}
					} else if (keys_mode==3) {
						//
							if (tmp_btn == 2) opt_sight_up();
							if (tmp_btn == 5)	opt_sight_down();					
							if (tmp_btn == 4) {
								menu_hide();
								keys_mode = 0;
							}
					} else if (keys_mode==4) {
						//
							if (tmp_btn == 2) 
							{
								opt_tilt_up();
								opt_draw_pan_tilt(99, 20, 25, 1, 0, 0, 255);
							}
							if (tmp_btn == 5)	{
								opt_tilt_down();
								opt_draw_pan_tilt(99, 20, 25, 1, 0, 0, 255);
							}					
							if (tmp_btn == 1) { 
								opt_pan_up();
								opt_draw_pan_tilt(99, 20, 25, 1, 0, 0, 255);
							}
							if (tmp_btn == 3)	{
								opt_pan_down();					
								opt_draw_pan_tilt(99, 20, 25, 1, 0, 0, 255);
							}
							if (tmp_btn == 4) {
								menu_main();
								menu_hide();
								keys_mode = 0;
							}
					} else if (keys_mode==5) {
							if (tmp_btn == 2) {
								opt_pallette_step_up();
								opt_draw_palette(99, 20, 25, 1, 0, 0, 255);
							}
							if (tmp_btn == 5)	{
								opt_pallette_step_down();	
								opt_draw_palette(99, 20, 25, 1, 0, 0, 255);
						  }								
							if (tmp_btn == 4) {
								menu_hide();
								opt_pallette_set();
								keys_mode = 0;
							}
					}
					btn_time = 0;
				}
			}
			
			if (btn != 0) {
				btn_time++;
			}

			if (btn != tmp_btn) {
				if (btn != 0) {
					printf("Button down %d\n\r", btn);
					rx_cnt = 0;
					if (cmd_init == 3){
						//if (btn == 1) opt_zoom_up        ();
						//if (btn == 2) opt_brightness_up  ();
						//if (btn == 3) opt_pallette_up    ();
						//if (btn == 4) opt_zoom_down      ();
						//if (btn == 5)	opt_brightness_down();
						//if (btn == 6)	opt_pallette_down  ();
					}
				} else {
					//if (tmp_btn) printf("Button up %d\n\r", tmp_btn);
				}
				tmp_btn = btn;
			}
			
			mode++;
			if (mode > 2) {
				mode = 0;
				//if (btn_grp[0] == 0 && btn_grp[1] == 0 && btn_grp[2] == 0) btn = 0;
			}
		}
		
		cur_tick = HAL_GetTick();
		if (cur_tick - sec_tick >= 1000){
			//printf(" tick %d %d %d\n\r", cur_tick, tmp_tick, cur_tick - tmp_tick);
			sec_tick = cur_tick;
			
			//reping after 1sec
			if (cmd_init == 2) cnt_init++;
			if (cnt_init >= 1){
				cmd_init = 0;
				cnt_init = 0;
			}

#ifdef FLIR_AUTOSAVE
			if (to_save > 0) to_save++;
			if (to_save >= 10){
				opt_save();	
				to_save = 0;
			}
#endif
		}			
  }
  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	uint8_t i;
	if (huart->Instance == USART1) {
		if (rx_cnt == 0) {
			for (i=0; i < RXBUFLEN; i++) rx_buff[i]=0;
		}
		
		rx_buff[rx_cnt] = rx_data;
		rx_cnt++;
		
		HAL_UART_Receive_IT(huart, &rx_data, 1);

		if (rx_cnt >= 8){
			if (rx_buff[0] == FLIR_FIRST_BYTE){
				if (rx_cnt == rx_buff[5] + 10) {
					answ_ready = 1;
				}
			}
		}
	}
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
