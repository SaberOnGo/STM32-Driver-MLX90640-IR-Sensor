/****************************************
JinMing electronics
https://www.ebay.com/itm/223274591029
****************************************/

#include "main.h"
#include "stm32f4xx_hal.h"
#include "delay.h"
#include "USART.h"
#include "MLX90640_API.h"

#define  Rate2HZ   0x02
#define  Rate4HZ   0x03
#define  Rate8HZ   0x04
#define  Rate16HZ  0x05
#define  Rate32HZ  0x06

#define  MLX_I2C_ADDR 0x33
#define	 RefreshRate Rate4HZ 
#define  TA_SHIFT 8 //Default shift for MLX90640 in open air

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void All_Init(void);


int main(void)
{
	static uint16_t eeMLX90640[832];  
	uint16_t frame[834];
	float Ta,tr;
	float emissivity=0.95;
	static float mlx90640To[768];
	u16 i=0;
	u32 CheckCode;
	
  All_Init();
	
	UsartBuff[0]=0x5A;
	UsartBuff[1]=0x5A;
	UsartBuff[2]=0x02;
	UsartBuff[3]=0x06;
	
	MLX90640_SetRefreshRate(MLX_I2C_ADDR, RefreshRate);
	MLX90640_SetChessMode(MLX_I2C_ADDR);
	paramsMLX90640 mlx90640;
	MLX90640_DumpEE(MLX_I2C_ADDR, eeMLX90640);
	MLX90640_ExtractParameters(eeMLX90640, &mlx90640);
	
	for(i=0;i<3;i++)//Lose the start frame
	{
			MLX90640_GetFrameData(MLX_I2C_ADDR, frame);
			delay_ms(500);
	}

	HAL_UART_Transmit_DMA(&UART1_Handler,UsartBuff,SEND_BUF_SIZE);//start DMA transfer
	
  while (1)
  {
			MLX90640_GetFrameData(MLX_I2C_ADDR, frame);
			Ta = MLX90640_GetTa(frame, &mlx90640);
			tr = Ta - TA_SHIFT;
			MLX90640_CalculateTo(frame, &mlx90640, emissivity, tr, mlx90640To);

			CheckCode=0x5A5A;
			CheckCode+=0x0602;		
			for(i=0;i<768;i++)//hundredfold,first send low 8bit and then high 8bit,DMA circular transfer
			{							
					UsartBuff[i*2+4]= (u16)(mlx90640To[i]*100)&0xFF;
					UsartBuff[i*2+5]= ((u16)(mlx90640To[i]*100)>>8)&0xFF;
					CheckCode+=(u16)(mlx90640To[i]*100);
			}
			UsartBuff[1540]= (u16)(Ta*100)&0xFF;
			UsartBuff[1541]= ((u16)(Ta*100)>>8)&0xFF;
			CheckCode+=(u16)(Ta*100);
			UsartBuff[1542]= (u16)CheckCode&0xFF;
			UsartBuff[1543]= ((u16)CheckCode>>8)&0xFF;
  }
}

void All_Init(void)
{
	HAL_Init();
  SystemClock_Config();
	Delay_Init(84);
  MX_GPIO_Init();
	uart_init(115200);
	
}

void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 84;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
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


/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_SET);

  /*Configure GPIO pins : PB6 PB7 */
  GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/* USER CODE BEGIN 4 */

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
