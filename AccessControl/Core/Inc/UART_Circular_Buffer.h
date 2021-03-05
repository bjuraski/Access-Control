/*
 * UART_Circular_Buffer.h
 *
 *  Created on: Jul 14, 2020
 *      Author: Borna
 */

#ifndef INC_UART_CIRCULAR_BUFFER_H_
#define INC_UART_CIRCULAR_BUFFER_H_

#include "stm32f4xx_hal.h"

#define UART_BUFFER_SIZE	256

typedef struct
{
	unsigned char buffer[UART_BUFFER_SIZE];
	volatile unsigned int head;
	volatile unsigned int tail;
} Ring_Buffer_t;

/* Ring Buffer initialization */
void RingBuffer_Init(void);

/* Storing data in buffer */
void Store_character(unsigned char character, Ring_Buffer_t *buffer);

/* Reading data in the Rx_Buffer and incrementing the tail count in Rx_Buffer */
int UART_Read(UART_HandleTypeDef *huart);

/* Writing data to the Tx_Buffer and incrementing the head count in Tx_Buffer */
void UART_Write(int character, UART_HandleTypeDef *huart);

/* Transmitting String to UART */
void UART_String_Transmit(const char *stringBuffer, UART_HandleTypeDef *huart);

/* Checking if the data is available to read in the Rx_Buffer */
int Is_Data_Received(UART_HandleTypeDef *huart);

/* ISR for UART */
void UART_ISR(UART_HandleTypeDef *huart);

#endif /* INC_UART_CIRCULAR_BUFFER_H_ */
