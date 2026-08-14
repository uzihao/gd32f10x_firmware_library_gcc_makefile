#include "blink_task.h"
#include "board.h"

#include <stdio.h>

#ifdef USE_FREERTOS
#include "task.h"

static void blink_task(void *parameter)
{
    (void)parameter;
    for (;;) {
        board_led_on();
        printf("freertos heartbeat\r\n");
        vTaskDelay(pdMS_TO_TICKS(1000U));
        board_led_off();
        vTaskDelay(pdMS_TO_TICKS(1000U));
    }
}

BaseType_t blink_task_create(void)
{
    return xTaskCreate(blink_task, "blink", configMINIMAL_STACK_SIZE, NULL,
                       tskIDLE_PRIORITY + 1U, NULL);
}
#else
#include "systick.h"

void blink_task_run_baremetal(void)
{
    for (;;) {
        board_led_on();
        printf("bare heartbeat\r\n");
        delay_1ms(1000U);
        board_led_off();
        delay_1ms(1000U);
    }
}
#endif
