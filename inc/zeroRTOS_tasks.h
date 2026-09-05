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

void zrtos_create_task(void);


#endif