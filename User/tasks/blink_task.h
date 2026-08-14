#ifndef TASKS_BLINK_TASK_H
#define TASKS_BLINK_TASK_H

#ifdef USE_FREERTOS
#include "FreeRTOS.h"

BaseType_t blink_task_create(void);
#else
void blink_task_run_baremetal(void);
#endif

#endif /* TASKS_BLINK_TASK_H */
