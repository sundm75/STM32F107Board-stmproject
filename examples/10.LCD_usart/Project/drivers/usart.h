#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"	
#include <stdio.h>
#include <stdarg.h>

/* USART1_REMAP = 0 */
#define UART1_GPIO_TX		GPIO_Pin_9
#define UART1_GPIO_RX		GPIO_Pin_10
#define UART1_GPIO		GPIOA

void UartConfig(void);
void PutChar(uint8_t ch);
uint8_t GetChar(void);
void PutString(uint8_t *str);
void UartPrintf(const char *format, ...);

#endif
