/*
 * it.h
 *
 *  Created on: Jul 14, 2020
 *      Author: Borna
 */

#ifndef INC_IT_H_
#define INC_IT_H_

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);

void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
void USART3_IRQHandler(void);
void TIM2_IRQHandler(void);
void TIM7_IRQHandler(void);
void EXTI1_IRQHandler(void);
void EXTI15_10_IRQHandler(void);

#endif /* INC_IT_H_ */
