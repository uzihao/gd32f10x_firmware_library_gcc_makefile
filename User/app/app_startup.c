#include "app_startup.h"
#include "blink_task.h"
#include "board.h"
#include "systick.h"

#include <stdio.h>

#ifdef USE_FREERTOS
#include "FreeRTOS.h"
#include "task.h"
#endif

void app_startup(void)
{
#ifndef USE_FREERTOS
    systick_config();
#endif

    board_init();
    printf("GD32F103 booted: USART0 PA9, 115200 8N1\r\n");

#ifdef USE_FREERTOS
    if (pdPASS != blink_task_create()) {
        while (1) {
        }
    }
    vTaskStartScheduler();
    while (1) {
    }
#else
    blink_task_run_baremetal();
#endif
}
