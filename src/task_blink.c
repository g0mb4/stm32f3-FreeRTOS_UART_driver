/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

/* ST library functions. */
#include <stm32f30x.h>
#include <stm32f3_discovery.h>
#include <stm32f30x_it.h>

#define BLINK_DELAY		(100)
#define BLINK_DELAY_RAW       	( ( portTickType ) BLINK_DELAY / portTICK_RATE_MS )

void task_blink( void *pvParameters ){
	portTickType xLastExecutionTime;

	/// Initialise xLastExecutionTime so the first call to vTaskDelayUntil() works correctly
	xLastExecutionTime = xTaskGetTickCount();

	for( ;; ){
		STM_EVAL_LEDToggle(LED3);

		vTaskDelayUntil( &xLastExecutionTime, BLINK_DELAY_RAW );
	}
}
