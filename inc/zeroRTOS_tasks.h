#ifndef TASKS_H
#define TASKS_H

#include "common-includes.h"

extern TCB* zrtos_tasks_list[ZRTOS_TASKS_LIST_SIZE];

extern volatile uint8_t zrtos_tasks_running;

extern volatile int zrtos_n_tasks;
extern volatile int zrtos_current_task;
extern volatile int zrtos_next_task;

typedef struct TaskControlBlock{

    volatile uint32_t* topOfStack;
    uint32_t taskPriority;

    char taskName[ZRTOS_TASK_NAME_MAX_LEN];

    uint32_t taskStackSize;

    zrtos_task_function_t task_function;

} TCB;

typedef void (*zrtos_task_function_t)(void);

void zrtos_tasks_init();
void zrtos_tasks_start();
TCB* zrtos_create_task(zrtos_task_function_t task_function,
                        const char* task_name,
                        uint32_t task_stack_size,
                        uint32_t task_priority
);


#endif