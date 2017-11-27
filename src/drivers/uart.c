/**
  * @file	uart.c
  * @brief	UART and low-level I/O function definitions
  *
  * The GCC C library makes calls to the following functions to perform low-level I/O.
  * These functions are implemented witihin the GCC C library as stub routines with "weak" linkage.
  * If a declaration of any of the above functions appears in your own code, your substitute routine will
  * override the declaration in the library and be used instead of the default (non-functional) routine.
  *
  * @author	gmb
  * @date	2017
  */

#include <drivers/uart.h>

#if UART_USE_INTERRUPT
	static xQueueHandle xRXQueue;
    static xQueueHandle xTXQueue;
#endif

/**
 * UART initialization, sets up the GPIO pins and the defined USART module
 * @param baud baudrate (bps)
 */
void UART_drvr_init(int baud){
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	// turning on the ports
#if UART_USE_PORT == 1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
#elif UART_USE_PORT == 2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
#elif UART_USE_PORT == 3
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
#elif UART_USE_PORT == 4
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
#endif
	RCC_AHBPeriphClockCmd(UART_GPIO_CLK, ENABLE);
	GPIO_PinAFConfig(UART_GPIO_PORT, UART_TX_GPIO_SRC, UART_AF);
	GPIO_PinAFConfig(UART_GPIO_PORT, UART_RX_GPIO_SRC, UART_AF);

	// set up TX and RX pins
	GPIO_InitStructure.GPIO_Pin = UART_TX_PIN;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
 	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
 	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
 	GPIO_Init(UART_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = UART_RX_PIN;
	GPIO_Init(UART_GPIO_PORT, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART_PORT, &USART_InitStructure);

#if UART_USE_INTERRUPT

	/* DOES NOT WORK !!!!
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = UART_IRQN;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_LOWEST_INTERRUPT_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	*/

	NVIC_SetPriority(UART_IRQN, configLIBRARY_LOWEST_INTERRUPT_PRIORITY);
    NVIC_EnableIRQ(UART_IRQN);

    USART_ITConfig(UART_PORT, USART_IT_RXNE, ENABLE);

	xRXQueue = xQueueCreate(UART_BUFF_SIZE, sizeof(char));
	xTXQueue = xQueueCreate(UART_BUFF_SIZE, sizeof(char));
#endif

	// turn on USART
	USART_Cmd(UART_PORT, ENABLE);

	/* turn off buffers, so I/O occurs immediately */
	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
}

/**
 * for 'read()' syscall
 * \n used for gets(), scanf(), ...
 * @param  file file descriptor
 * @param  data data
 * @param  len  data length
 * @return      number of read bytes
 */
ssize_t UART_read(int file, char *data, ssize_t len){
	ssize_t bytes_read = 0;

#if UART_USE_INTERRUPT

	while( xQueueReceive(xRXQueue, data, portMAX_DELAY ) == pdPASS ){
        if(++bytes_read >= len)
            break;
	}

#else

	for (; bytes_read < len; bytes_read++){
		/* waiting for data */
		while(USART_GetFlagStatus(UART_PORT, USART_FLAG_RXNE) == RESET);
		*data = (char)USART_ReceiveData(UART_PORT);
		data++;
	}

#endif

    return bytes_read;
}

/**
 * for 'write()' syscall
 * \n used for puts(), printf(), ...
 * @param  file file descriptor
 * @param  data data
 * @param  len  data length
 * @return      number of written bytes
 */
ssize_t UART_write(int file, char *data, ssize_t len){
	ssize_t bytes_written = 0;

#if UART_USE_INTERRUPT

	while(bytes_written < len){
		xQueueSend(xTXQueue, &data[bytes_written++], ( TickType_t ) 0x10);
	}

	USART_ITConfig(UART_PORT, USART_IT_TXE, ENABLE);

#else

	for(; bytes_written < len; bytes_written++){
		USART_SendData(UART_PORT, (uint8_t) data[bytes_written]);
		/* loop until the end of transmission */
		while(USART_GetFlagStatus(UART_PORT, USART_FLAG_TC) == RESET);
	}

#endif
	return bytes_written;
}

#if UART_USE_INTERRUPT

void UART_IRQN_FCN (void){
	char ch;

	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

	if(USART_GetITStatus(UART_PORT, USART_IT_RXNE) == SET){
		ch = (char)USART_ReceiveData(UART_PORT);
		xQueueSendFromISR(xRXQueue, &ch, &xHigherPriorityTaskWoken);
	}

	if(USART_GetITStatus(UART_PORT, USART_IT_TXE) == SET){
		if(xQueueReceiveFromISR(xTXQueue, &ch, &xHigherPriorityTaskWoken) == pdTRUE){
			USART_SendData(UART_PORT, (uint8_t) ch);
		} else {
			USART_ITConfig(UART_PORT, USART_IT_TXE, DISABLE);
		}
	}

	/*NOTE: The syntax for forcing a context switch within an ISR varies between
	FreeRTOS ports. The portEND_SWITCHING_ISR() macro is provided as part of
	the Corte M3 port layer for this purpose. taskYIELD() must never be called
	from an ISR! */
	portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
}

#endif
