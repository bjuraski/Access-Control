/*
 * UART_Circular_Buffer.c
 *
 *  Created on: Jul 14, 2020
 *      Author: Borna
 */

#include "UART_Circular_Buffer.h"
#include <stdint.h>
#include <string.h>

/* External Peripheral Handlers */
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;

#define UART_HANDLER_NEXTION	&huart1
#define UART_HANDLER_PC			&huart2
#define UART_HANDLER_GSM		&huart3

Ring_Buffer_t Rx_buffer1 = {{0}, 0, 0};
Ring_Buffer_t Tx_buffer1 = {{0}, 0, 0};
Ring_Buffer_t *_rx_buffer1;
Ring_Buffer_t *_tx_buffer1;

Ring_Buffer_t Rx_buffer2 = {{0}, 0, 0};
Ring_Buffer_t Tx_buffer2 = {{0}, 0, 0};
Ring_Buffer_t *_rx_buffer2;
Ring_Buffer_t *_tx_buffer2;

Ring_Buffer_t Rx_buffer3 = {{0}, 0, 0};
Ring_Buffer_t Tx_buffer3 = {{0}, 0, 0};
Ring_Buffer_t *_rx_buffer3;
Ring_Buffer_t *_tx_buffer3;



/**
  * @brief UART Ring Buffer initialization function
  * @param None
  * @retval None
  */
void RingBuffer_Init(void)
{
	_rx_buffer1 = &Rx_buffer1;
	_tx_buffer1 = &Tx_buffer1;
	_rx_buffer2 = &Rx_buffer2;
	_tx_buffer2 = &Tx_buffer2;
	_rx_buffer3 = &Rx_buffer3;
	_tx_buffer3 = &Tx_buffer3;

	/* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
	__HAL_UART_ENABLE_IT(UART_HANDLER_NEXTION, UART_IT_ERR);
	__HAL_UART_ENABLE_IT(UART_HANDLER_PC, UART_IT_ERR);
	__HAL_UART_ENABLE_IT(UART_HANDLER_GSM, UART_IT_ERR);

	/* Enable the UART Data Register Not Empty Interrupt */
	__HAL_UART_ENABLE_IT(UART_HANDLER_NEXTION, UART_IT_RXNE);
	__HAL_UART_ENABLE_IT(UART_HANDLER_PC, UART_IT_RXNE);
	__HAL_UART_ENABLE_IT(UART_HANDLER_GSM, UART_IT_RXNE);
}

/**
  * @brief Buffer characters store function
  * @param Character to store
  * @param Pointer to buffer
  * @retval None
  */
void Store_Character(unsigned char character, Ring_Buffer_t *buffer)
{
	int i = (unsigned int)(buffer->head + 1) % UART_BUFFER_SIZE;

	/* If written data reaches tail, stop writing */
	if (i != buffer->tail)
	{
		buffer->buffer[buffer->head] = character;
		buffer->head = i;
	}
}

/**
  * @brief UART read data function
  * @param Pointer to UART handler
  * @retval Integer typed data
  */
int UART_Read(UART_HandleTypeDef *huart)
{
	if (huart == UART_HANDLER_NEXTION)
	{
		/* If the head is not ahead of the tail, there is no characters */
		if (_rx_buffer1->head == _rx_buffer1->tail)
		{
			return -1;
		}
		else
		{
			unsigned char c = _rx_buffer1->buffer[_rx_buffer1->tail];
			_rx_buffer1->tail = (unsigned int)(_rx_buffer1->tail + 1) % UART_BUFFER_SIZE;
			return c;
		}
	}
	else if (huart == UART_HANDLER_PC)
	{
		/* If the head is not ahead of the tail, there is no characters */
		if (_rx_buffer2->head == _rx_buffer2->tail)
		{
			return -1;
		}
		else
		{
			unsigned char c = _rx_buffer2->buffer[_rx_buffer2->tail];
			_rx_buffer2->tail = (unsigned int)(_rx_buffer2->tail + 1) % UART_BUFFER_SIZE;
			return c;
		}
	}
	else if (huart == UART_HANDLER_GSM)
	{
		/* If the head is not ahead of the tail, there is no characters */
		if (_rx_buffer3->head == _rx_buffer3->tail)
		{
			return -1;
		}
		else
		{
			unsigned char c = _rx_buffer3->buffer[_rx_buffer3->tail];
			_rx_buffer3->tail = (unsigned int)(_rx_buffer3->tail + 1) % UART_BUFFER_SIZE;
			return c;
		}
	}
	else
	{
		return -1;
	}
}

/**
  * @brief UART write data function
  * @param Integer typed data
  * @param Pointer to UART handler
  * @retval None
  */
void UART_Write(int character, UART_HandleTypeDef *huart)
{
	if (character >= 0)
	{
		if (huart == UART_HANDLER_NEXTION)
		{
			int i = (_tx_buffer1->head + 1) % UART_BUFFER_SIZE;

			/* If the output buffer is full, wait for the interrupt handler to empty it */
			while (i == _tx_buffer1->tail);

			_tx_buffer1->buffer[_tx_buffer1->head] = (uint8_t)character;
			_tx_buffer1->head = i;

			/* Enable UART transmission interrupt */
			__HAL_UART_ENABLE_IT(UART_HANDLER_NEXTION, UART_IT_TXE);
		}
		else if (huart == UART_HANDLER_PC)
		{
			int i = (_tx_buffer2->head + 1) % UART_BUFFER_SIZE;

			/* If the output buffer is full, wait for the interrupt handler to empty it */
			while (i == _tx_buffer2->tail);

			_tx_buffer2->buffer[_tx_buffer2->head] = (uint8_t)character;
			_tx_buffer2->head = i;

			/* Enable UART transmission interrupt */
			__HAL_UART_ENABLE_IT(UART_HANDLER_PC, UART_IT_TXE);
		}
		else if (huart == UART_HANDLER_GSM)
		{
			int i = (_tx_buffer3->head + 1) % UART_BUFFER_SIZE;

			/* If the output buffer is full, wait for the interrupt handler to empty it */
			while (i == _tx_buffer3->tail);

			_tx_buffer3->buffer[_tx_buffer3->head] = (uint8_t)character;
			_tx_buffer3->head = i;

			/* Enable UART transmission interrupt */
			__HAL_UART_ENABLE_IT(UART_HANDLER_GSM, UART_IT_TXE);
		}
	}
}

/**
  * @brief String to UART Transmit function
  * @param Character pointer to send
  * @retval Data
  */
void UART_String_Transmit (const char *stringBuffer, UART_HandleTypeDef *huart)
{
	while (*stringBuffer != '\0')
	{
		UART_Write(*stringBuffer++, huart);
	}
}

/**
  * @brief UART Check if the data is available to read function
  * @param Pointer to UART handler
  * @retval Data
  */
int Is_Data_Received(UART_HandleTypeDef *huart)
{
	if (huart == UART_HANDLER_NEXTION)
	{
		return (uint16_t)(UART_BUFFER_SIZE + _rx_buffer1->head - _rx_buffer1->tail) % UART_BUFFER_SIZE;
	}
	else if (huart == UART_HANDLER_PC)
	{
		return (uint16_t)(UART_BUFFER_SIZE + _rx_buffer2->head - _rx_buffer2->tail) % UART_BUFFER_SIZE;
	}
	else if (huart == UART_HANDLER_GSM)
	{
		return (uint16_t)(UART_BUFFER_SIZE + _rx_buffer3->head - _rx_buffer3->tail) % UART_BUFFER_SIZE;
	}
	else
	{
		return -1;
	}

}

/**
  * @brief UART Interrupt Service Routine
  * @param Pointer to UART handler
  * @retval None
  */
void UART_ISR (UART_HandleTypeDef *huart)
{
	uint32_t ISR_Flags = READ_REG(huart->Instance->SR);
	uint32_t CR1_Flags = READ_REG(huart->Instance->CR1);

	/* If DR is not empty and the Rx Int is enabled */
	if (((ISR_Flags & USART_SR_RXNE) != RESET) && ((CR1_Flags & USART_CR1_RXNEIE) != RESET))
	{
		huart->Instance->SR;                    // Read status register
		unsigned char c = huart->Instance->DR;  // Read data register
		if (huart == UART_HANDLER_NEXTION)
		{
			Store_Character(c, _rx_buffer1);			// Store data in buffer
		}
		else if (huart == UART_HANDLER_PC)
		{
			Store_Character(c, _rx_buffer2);			// Store data in buffer
		}
		else if (huart == UART_HANDLER_GSM)
		{
			Store_Character(c, _rx_buffer3);			// Store data in buffer
		}
		return;
	}

	/* If interrupt is caused due to Transmit Data Register Empty */
	if (((ISR_Flags & USART_SR_TXE) != RESET) && ((CR1_Flags & USART_CR1_TXEIE) != RESET))
	{
		if (huart == UART_HANDLER_NEXTION)
		{
			/* If Buffer is empty, disable interrupts */
			if (Tx_buffer1.head == Tx_buffer1.tail)
			{
				__HAL_UART_DISABLE_IT(UART_HANDLER_NEXTION, UART_IT_TXE);
			}
			/* There is more data in the output buffer. Send the next byte */
			else
			{
				 unsigned char c = Tx_buffer1.buffer[Tx_buffer1.tail];
				 Tx_buffer1.tail = (Tx_buffer1.tail + 1) % UART_BUFFER_SIZE;
				 huart->Instance->SR;
				 huart->Instance->DR = c;
			}
		}
		else if (huart == UART_HANDLER_PC)
		{
			/* If Buffer is empty, disable interrupts */
			if (Tx_buffer2.head == Tx_buffer2.tail)
			{
				__HAL_UART_DISABLE_IT(UART_HANDLER_PC, UART_IT_TXE);
			}
			/* There is more data in the output buffer. Send the next byte */
			else
			{
				 unsigned char c = Tx_buffer2.buffer[Tx_buffer2.tail];
				 Tx_buffer2.tail = (Tx_buffer2.tail + 1) % UART_BUFFER_SIZE;
				 huart->Instance->SR;
				 huart->Instance->DR = c;
			}
		}
		else if (huart == UART_HANDLER_GSM)
		{
			/* If Buffer is empty, disable interrupts */
			if (Tx_buffer3.head == Tx_buffer3.tail)
			{
				__HAL_UART_DISABLE_IT(UART_HANDLER_GSM, UART_IT_TXE);
			}
			/* There is more data in the output buffer. Send the next byte */
			else
			{
				 unsigned char c = Tx_buffer3.buffer[Tx_buffer3.tail];
				 Tx_buffer3.tail = (Tx_buffer3.tail + 1) % UART_BUFFER_SIZE;
				 huart->Instance->SR;
				 huart->Instance->DR = c;
			}
		}
		return;
	}
}
