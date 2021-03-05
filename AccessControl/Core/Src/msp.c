/*
 * msp.c
 *
 *  Created on: Jul 14, 2020
 *      Author: Borna
 */

#include "main.h"

/**
  * @brief  Initialize the MSP.
  * @retval None
  */
void HAL_MspInit(void)
{
	/* Low Level Processor specific initializations */

	// 1. Set up the priority grouping of the ARM Cortex Mx Processor
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	// 2. Enable the required system exceptions of the ARM Cortex Mx Processor
	SCB->SHCSR |= (0x7 << 16);	// Usage Fault, Memory Fault and Bus Fault system exceptions

	// 3. Configure the priority for the system exceptions
	HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
	HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
	HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
}

/**
  * @brief  UART MSP Init.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval None
  */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	GPIO_InitTypeDef GPIO_UART = {0};

	/* Low Level Initializations of the USART1 peripheral */
	if (huart->Instance == USART1)
	{
		// 1. Enable the clock for the USART1 peripheral as well as for the GPIOA peripheral
		__HAL_RCC_USART1_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();

		// 2. Pin Muxing configurations
		GPIO_UART.Pin = GPIO_PIN_9;					// USART1_TX --> PA9
		GPIO_UART.Mode = GPIO_MODE_AF_PP;
		GPIO_UART.Pull = GPIO_PULLUP;
		GPIO_UART.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_UART.Alternate =  GPIO_AF7_USART1;
		HAL_GPIO_Init(GPIOA, &GPIO_UART);
		GPIO_UART.Pin = GPIO_PIN_10;				// USART1_RX --> PA10
		HAL_GPIO_Init(GPIOA, &GPIO_UART);

		// 3. Enable the IRQ and set up the priority (NVIC settings)
		HAL_NVIC_EnableIRQ(USART1_IRQn);
		HAL_NVIC_SetPriority(USART1_IRQn, 15, 0);
	}
	else if (huart->Instance == USART2)
	{
		// 1. Enable the clock for the USART2 peripheral as well as for the GPIOA peripheral
		__HAL_RCC_USART2_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();

		// 2. Pin Muxing configurations
		GPIO_UART.Pin = GPIO_PIN_2;					// USART2_TX --> PA2
		GPIO_UART.Mode = GPIO_MODE_AF_PP;
		GPIO_UART.Pull = GPIO_PULLUP;
		GPIO_UART.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_UART.Alternate =  GPIO_AF7_USART2;
		HAL_GPIO_Init(GPIOA, &GPIO_UART);
		GPIO_UART.Pin = GPIO_PIN_3;					// USART2_RX --> PA3
		HAL_GPIO_Init(GPIOA, &GPIO_UART);

		// 3. Enable the IRQ and set up the priority (NVIC settings)
		HAL_NVIC_EnableIRQ(USART2_IRQn);
		HAL_NVIC_SetPriority(USART2_IRQn, 15, 0);
	}
	else if (huart->Instance == USART3)
	{
		// 1. Enable the clock for the USART3 peripheral as well as for the GPIOB and GPIOC peripheral
		__HAL_RCC_USART3_CLK_ENABLE();
		__HAL_RCC_GPIOC_CLK_ENABLE();
		__HAL_RCC_GPIOB_CLK_ENABLE();

		// 2. Pin Muxing configurations
		GPIO_UART.Pin = GPIO_PIN_10;				// USART3_TX --> PB10
		GPIO_UART.Mode = GPIO_MODE_AF_PP;
		GPIO_UART.Pull = GPIO_PULLUP;
		GPIO_UART.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_UART.Alternate =  GPIO_AF7_USART3;
		HAL_GPIO_Init(GPIOB, &GPIO_UART);

		GPIO_UART.Pin = GPIO_PIN_5;					// USART3_RX --> PC5
		GPIO_UART.Mode = GPIO_MODE_AF_PP;
		GPIO_UART.Pull = GPIO_PULLUP;
		GPIO_UART.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_UART.Alternate =  GPIO_AF7_USART3;
		HAL_GPIO_Init(GPIOC, &GPIO_UART);

		// 3. Enable the IRQ and set up the priority (NVIC settings)
		HAL_NVIC_EnableIRQ(USART3_IRQn);
		HAL_NVIC_SetPriority(USART3_IRQn, 15, 0);
	}
}

/**
  * @brief  UART MSP DeInit.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval None
  */
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART1)
	{
		/* Peripheral clock disable */
		__HAL_RCC_USART1_CLK_DISABLE();

		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9);
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_10);

		/* USART1 interrupt DeInit */
		HAL_NVIC_DisableIRQ(USART1_IRQn);
	}
	else if (huart->Instance == USART2)
	{
		/* Peripheral clock disable */
		__HAL_RCC_USART2_CLK_DISABLE();

		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2);
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_3);

		/* USART2 interrupt DeInit */
		HAL_NVIC_DisableIRQ(USART2_IRQn);
	}
	else if (huart->Instance == USART3)
	{
		/* Peripheral clock disable */
		__HAL_RCC_USART3_CLK_DISABLE();

		HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10);
		HAL_GPIO_DeInit(GPIOC, GPIO_PIN_5);

		/* USART2 interrupt DeInit */
		HAL_NVIC_DisableIRQ(USART3_IRQn);
	}
}

/**
  * @brief  Initializes the TIM Base MSP.
  * @param  htim TIM Base handle
  * @retval None
  */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM2)
	{
		// 1. Enable the clock for the TIM2 peripheral
		__HAL_RCC_TIM2_CLK_ENABLE();

		// 2. Enable the IRQ of TIM2
		HAL_NVIC_EnableIRQ(TIM2_IRQn);

		// 3. Set up the priority for TIM2_IRQn
		HAL_NVIC_SetPriority(TIM2_IRQn, 12, 0);
	}
	else if (htim->Instance == TIM7)
	{
		// 1. Enable the clock for the TIM7 peripheral
		__HAL_RCC_TIM7_CLK_ENABLE();

		// 2. Enable the IRQ of TIM7
		HAL_NVIC_EnableIRQ(TIM7_IRQn);

		// 3. Set up the priority for TIM7_IRQn
		HAL_NVIC_SetPriority(TIM7_IRQn, 15, 0);
	}
}

/**
  * @brief  DeInitializes TIM Base MSP.
  * @param  htim TIM Base handle
  * @retval None
  */
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM2)
	{
		/* Peripheral clock disable */
		__HAL_RCC_TIM2_CLK_DISABLE();

		/* TIM2 interrupt DeInit */
		HAL_NVIC_DisableIRQ(TIM2_IRQn);
	}
	else if (htim->Instance == TIM7)
	{
		/* Peripheral clock disable */
		__HAL_RCC_TIM7_CLK_DISABLE();

		/* TIM7 interrupt DeInit */
		HAL_NVIC_DisableIRQ(TIM7_IRQn);
	}
}

void PWM_TIM_GPIO_Init(TIM_HandleTypeDef* htim)
{
	GPIO_InitTypeDef PWM_GPIO_InitStruct = {0};

	if (htim->Instance == TIM2)
	{
		/* Enable clock for GPIOA peripheral */
		__HAL_RCC_GPIOA_CLK_ENABLE();

		/* Pin configuration for TIMER2 CHANNEL1*/
		PWM_GPIO_InitStruct.Pin = BUZZER_GPIO_PIN;
		PWM_GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		PWM_GPIO_InitStruct.Pull = GPIO_NOPULL;
		PWM_GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		PWM_GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
	    HAL_GPIO_Init(BUZZER_GPIO_PORT, &PWM_GPIO_InitStruct);
	}
}

/**
  * @brief  Initializes the RTC MSP.
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @retval None
  */
void HAL_RTC_MspInit(RTC_HandleTypeDef* hrtc)
{
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

	/* Configure LSE/LSI as RTC clock source */
	#ifdef RTC_CLOCK_SOURCE_LSE
		RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE;
		RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
		RCC_OscInitStruct.LSEState = RCC_LSE_ON;
		RCC_OscInitStruct.LSIState = RCC_LSI_OFF;

		if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
		{
			Error_Handler();
		}

		PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
		PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;

		if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
		{
			Error_Handler();
		}
	#elif defined (RTC_CLOCK_SOURCE_LSI)
		RCC_OscInitStruct.OscillatorType =  RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE;
		RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
		RCC_OscInitStruct.LSIState = RCC_LSI_ON;
		RCC_OscInitStruct.LSEState = RCC_LSE_OFF;

		if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
		{
			Error_Handler();
		}

		PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
		PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;

		if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
		{
			Error_Handler();
		}
	#else
	#error Please select the RTC Clock source inside the main.h file
	#endif /* RTC_CLOCK_SOURCE_LSE */

	/* Enable RTC Peripheral Clock */
	__HAL_RCC_RTC_ENABLE();
}

/**
  * @brief  DeInitializes the RTC MSP.
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @retval None
  */
void HAL_RTC_MspDeInit(RTC_HandleTypeDef* hrtc)
{
	/* Peripheral clock disable */
	__HAL_RCC_RTC_DISABLE();
}
