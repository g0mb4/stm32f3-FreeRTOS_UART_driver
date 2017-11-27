/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

/* ST library functions. */
#include <stm32f30x.h>
#include <stm32f3_discovery.h>
#include <stm32f30x_it.h>

#include <drivers/uart.h>

#include <stdio.h>

void task_uart( void *pvParameters ){
	char ch;

	/* echo */
	printf("UART echo started.\n");
	for( ;; ){
			ch = getchar();
			putchar(ch);
	}
}
