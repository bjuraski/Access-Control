/*
 * Access_Control.h
 *
 *  Created on: Aug 10, 2020
 *      Author: Borna
 */

#ifndef INC_ACCESS_CONTROL_H_
#define INC_ACCESS_CONTROL_H_

#include "main.h"

/* External Peripheral Handlers */
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim7;
extern RTC_HandleTypeDef hrtc;

/* Function Prototypes */
void Swap(uint8_t *a, uint8_t *b);
void ShuffleNumbers(uint8_t arr[], uint8_t n);
void PrintArray(uint8_t arr[], uint8_t n);
void Print_TimeInfo(void);
void Print_Log_Terminal(uint8_t value);

#endif /* INC_ACCESS_CONTROL_H_ */
