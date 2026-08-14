#include "FreeRTOS.h"
#include "task.h"

void vApplicationStackOverflowHook(TaskHandle_t task, char *task_name)
{
    (void)task;
    (void)task_name;

    taskDISABLE_INTERRUPTS();
    for (;;) {
    }
}
