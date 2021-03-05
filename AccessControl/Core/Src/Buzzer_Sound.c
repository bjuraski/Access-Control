/*
 * Buzzer_Sound.c
 *
 *  Created on: Aug 10, 2020
 *      Author: Borna
 */

#include "main.h"
#include "Buzzer_Sound.h"

/**
  * @brief  Stop Sound Generation
  * @param  None
  * @retval None
  */
void Sound_Off(void)
{
	htim2.Instance->CCR1 = 0;					//Duty Cycle = 0
	HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);	//Stopping PWM Timer
}
/**
  * @brief  Press Sound Generation
  * @param  None
  * @retval None
  */
void Sound_Press(void)
{
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	htim2.Instance->CCR1 = 500;
	HAL_Delay(100);
	Sound_Off();
}
/**
  * @brief  Access Granted Sound Generation
  * @param  None
  * @retval None
  */
void Sound_Password_Correct(void)
{
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	htim2.Instance->CCR1 = 800;
	HAL_Delay(500);
	Sound_Off();
}

/**
  * @brief  Access Denied Sound Generation
  * @param  None
  * @retval None
  */
void Sound_Password_Invalid(void)
{
	uint16_t value = 0;
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);

	while (value < 1000)
	{
		htim2.Instance->CCR1 = value;
		value += 100;
		HAL_Delay(100);
	}

	Sound_Off();
}

/**
  * @brief  Emergency Sound Generation
  * @param  None
  * @retval None
  */
void Sound_Alarm_Alert(void)
{
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	uint16_t dutyCycle = HAL_TIM_ReadCapturedValue(&htim2, TIM_CHANNEL_1);

	while (dutyCycle < __HAL_TIM_GET_AUTORELOAD(&htim2))
	{
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, dutyCycle += 50);
		HAL_Delay(200);
	}

	HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
	dutyCycle = 0;
	HAL_Delay(100);
}






