#ifndef TASKS_H
#define TASKS_H

#include "common-includes.h"
#include "tasks.h"


typedef void (*zrtos_task_function_t)(void);

typedef struct TaskControlBlock{

    volatile uint32_t* topOfStack;
    uint32_t taskPriority;

    char taskName[ZRTOS_TASK_NAME_MAX_LEN];

    uint32_t taskStackSize;

    zrtos_task_function_t task_function;
    
    uint64_t wake_tick;

    struct TaskControlBlock *next;


} TCB;

typedef struct{
    uint32_t count;
    TCB* tcb;
} zrtos_sem_t;


void zrtos_tasks_init();
void zrtos_tasks_start();
void zrtos_add_to_ready_list(TCB* tcb);
TCB* zrtos_create_task(zrtos_task_function_t task_function,
                        const char* task_name,
                        uint32_t task_stack_size,
                        uint32_t task_priority
);

void zrtos_delay(uint64_t n);

zrtos_sem_t* zrtos_create_sem(uint32_t initial_count);
void zrtos_sem_take(zrtos_sem_t* sem);
void zrtos_sem_give(zrtos_sem_t* sem);

extern TCB* zrtos_ready;
extern TCB* zrtos_delayed;
extern volatile uint64_t zrtos_ticks;

#endif