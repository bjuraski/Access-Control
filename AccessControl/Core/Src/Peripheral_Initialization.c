/*
 * Peripheral_Initialization.c
 *
 *  Created on: Aug 10, 2020
 *      Author: Borna
 */

#include "main.h"
#include "Peripheral_Initialization.h"

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
void GPIO_Init(void)
{
	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_PIR = {0};
	GPIO_InitTypeDef GPIO_LED = {0};
	GPIO_InitTypeDef GPIO_RELAY = {0};
	GPIO_InitTypeDef GPIO_BUTTON = {0};

	HAL_GPIO_WritePin(LED_GPIO_PORT, LED_GPIO_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RELAY_GPIO_PORT, RELAY_GPIO_PIN, GPIO_PIN_SET);

	/* Configure GPIO pin : PIR : PA1 */
	GPIO_PIR.Pin = PIR_GPIO_PIN;
	GPIO_PIR.Mode = GPIO_MODE_IT_RISING;
	GPIO_PIR.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(PIR_GPIO_PORT, &GPIO_PIR);

	/* Configure GPIO pin : LED : PA5 */
	GPIO_LED.Pin = LED_GPIO_PIN;
	GPIO_LED.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_LED.Pull = GPIO_NOPULL;
	GPIO_LED.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LED_GPIO_PORT, &GPIO_LED);

	/* Configure GPIO pin : RELAY : PA6 */
	GPIO_RELAY.Pin = RELAY_GPIO_PIN;
	GPIO_RELAY.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_RELAY.Pull = GPIO_NOPULL;
	GPIO_RELAY.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(RELAY_GPIO_PORT, &GPIO_RELAY);

	/* Configure GPIO pin : BUTTON : PC13 */
	GPIO_BUTTON.Pin = BUTTON_GPIO_PIN;
	GPIO_BUTTON.Mode = GPIO_MODE_IT_FALLING;
	GPIO_BUTTON.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(BUTTON_GPIO_PORT, &GPIO_BUTTON);

	/* EXTI interrupt init*/
	HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI1_IRQn);

	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
void UART1_Init(void)
{
	huart1.Instance = USART1;
	huart1.Init.BaudRate = 9600;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;

	if (HAL_UART_Init(&huart1) != HAL_OK )
	{
		Error_Handler();
	}
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
void UART2_Init(void)
{
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 9600;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;

	if (HAL_UART_Init(&huart2) != HAL_OK )
	{
		Error_Handler();
	}
}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
void UART3_Init(void)
{
	huart3.Instance = USART3;
	huart3.Init.BaudRate = 9600;
	huart3.Init.WordLength = UART_WORDLENGTH_8B;
	huart3.Init.StopBits = UART_STOPBITS_1;
	huart3.Init.Parity = UART_PARITY_NONE;
	huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart3.Init.Mode = UART_MODE_TX_RX;
	huart3.Init.OverSampling = UART_OVERSAMPLING_16;

	if (HAL_UART_Init(&huart3) != HAL_OK )
	{
		Error_Handler();
	}
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
void TIMER2_Init(void)
{
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_OC_InitTypeDef sConfigOC = {0};

	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 90 - 1;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 1000 - 1;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

	if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
	{
		Error_Handler();
	}

	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;

	if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
	{
		Error_Handler();
	}

	if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
	{
		Error_Handler();
	}

	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

	if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}

	PWM_TIM_GPIO_Init(&htim2);
}

/**
  * @brief TIM7 Initialization Function
  * @param None
  * @retval None
  */
void TIMER7_Init(void)
{
	htim7.Instance = TIM7;
	htim7.Init.Prescaler = 60000 - 1;
	htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim7.Init.Period = 1500 - 1;
	htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

	if (HAL_TIM_Base_Init(&htim7) != HAL_OK )
	{
		Error_Handler();
	}
}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
void RTC_Init(void)
{
	hrtc.Instance = RTC;
	hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
	hrtc.Init.AsynchPrediv = RTC_ASYNCH_PREDIV;
	hrtc.Init.SynchPrediv = RTC_SYNCH_PREDIV;
	hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
	hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
	hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
	__HAL_RTC_RESET_HANDLE_STATE(&hrtc);

	if (HAL_RTC_Init(&hrtc) != HAL_OK)
	{
		Error_Handler();
	}
}
