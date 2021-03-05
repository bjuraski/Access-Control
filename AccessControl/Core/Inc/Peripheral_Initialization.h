/*
 * Peripheral_Initialization.h
 *
 *  Created on: Aug 10, 2020
 *      Author: Borna
 */

#ifndef INC_PERIPHERAL_INITIALIZATION_H_
#define INC_PERIPHERAL_INITIALIZATION_H_

/* External Peripheral Handlers */
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim7;
extern RTC_HandleTypeDef hrtc;

/* Function Prototypes */
void GPIO_Init(void);
void UART1_Init(void);
void UART2_Init(void);
void UART3_Init(void);
void TIMER2_Init(void);
void TIMER7_Init(void);
void RTC_Init(void);

#endif /* INC_PERIPHERAL_INITIALIZATION_H_ */
