#ifndef TASKS_H
#define TASKS_H

#include "common-includes.h"

typedef struct TaskControlBlock{

    volatile uint32_t* topOfStack;
    uint32_t taskPriority;
    uint32_t* taskStack;   // Lowest value of stack address

    char taskName[ZRTOS_TASK_NAME_MAX_LEN];

    uint32_t* endOfStack;  //  Highest value of stack address

} TCB;

typedef void (*zrtos_task_function_t)(void);

void zrtos_tasks_init();
TCB* zrtos_create_task(zrtos_task_function_t task_function,
                        const char* task_name,
                        uint8_t task_stack_size,
                        uint32_t task_priority
);


#endif