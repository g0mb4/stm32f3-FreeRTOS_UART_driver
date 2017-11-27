/**
  * @file	uart.h
  * @brief	UART function prototypes
  * @author	gmb
  * @date	2017
  */

#ifndef __UART_H__
#define __UART_H__

#include <FreeRTOS.h>
#include <queue.h>

#include <stm32f30x_usart.h>
#include <stm32f30x_gpio.h>
#include <stm32f30x_rcc.h>

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>

#define UART_USE_PORT 		1	///< UART port to use (1-4)
#define UART_USE_INTERRUPT	1	///< UART interrupt mode (1-3) or polling (1-4)

#if UART_USE_PORT == 1
	#define UART_PORT			USART1
	#define UART_TX_PIN			GPIO_Pin_4    ///< PC4 (USART1_TX)
	#define UART_TX_GPIO_SRC	GPIO_PinSource4
	#define UART_RX_PIN			GPIO_Pin_5    ///< PC5 (USART1_RX)
	#define UART_RX_GPIO_SRC	GPIO_PinSource5
	#define UART_GPIO_PORT		GPIOC
	#define UART_AF				GPIO_AF_7
	#define UART_GPIO_CLK		RCC_AHBPeriph_GPIOC
	#define UART_IRQN			USART1_IRQn
	#define UART_IRQN_FCN		USART1_IRQHandler
#elif UART_USE_PORT == 2
	#define UART_PORT			USART2
	#define UART_TX_PIN			GPIO_Pin_2    ///< PA2 (USART2_TX)
	#define UART_TX_GPIO_SRC	GPIO_PinSource2
	#define UART_RX_PIN			GPIO_Pin_3    ///< PA3 (USART2_RX)
	#define UART_RX_GPIO_SRC	GPIO_PinSource3
	#define UART_GPIO_PORT		GPIOA
	#define UART_AF				GPIO_AF_7
	#define UART_GPIO_CLK		RCC_AHBPeriph_GPIOA
	#define UART_IRQN			USART2_IRQn
	#define UART_IRQN_FCN		USART2_IRQHandler
#elif UART_USE_PORT == 3
	#define UART_PORT			USART3
	#define UART_TX_PIN			GPIO_Pin_10    ///< PB10 (USART3_TX)
	#define UART_TX_GPIO_SRC	GPIO_PinSource10
	#define UART_RX_PIN			GPIO_Pin_11    ///< PB11 (USART3_RX)
	#define UART_RX_GPIO_SRC	GPIO_PinSource11
	#define UART_GPIO_PORT		GPIOB
	#define UART_AF				GPIO_AF_7
	#define UART_GPIO_CLK		RCC_AHBPeriph_GPIOB
	#define UART_IRQN			USART3_IRQn
	#define UART_IRQN_FCN		USART3_IRQHandler
#elif UART_USE_PORT == 4
	#define UART_PORT			UART4
	#define UART_TX_PIN			GPIO_Pin_10    ///< PC10 (USART3_TX)
	#define UART_TX_GPIO_SRC	GPIO_PinSource10
	#define UART_RX_PIN			GPIO_Pin_11    ///< PC11 (USART3_RX)
	#define UART_RX_GPIO_SRC	GPIO_PinSource11
	#define UART_GPIO_PORT		GPIOC
	#define UART_AF				GPIO_AF_8
	#define UART_GPIO_CLK		RCC_AHBPeriph_GPIOC
#endif

void UART_drvr_init(int baud);

ssize_t UART_read(int file, char *data, ssize_t len);
ssize_t UART_write(int file, char *data, ssize_t len);

#if UART_USE_INTERRUPT

#define UART_BUFF_SIZE	 128	///< UART buffer size in bytes for interrupt mode

void UART_IRQN_FCN (void);

#endif

#endif
