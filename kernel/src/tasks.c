#include "tasks.h"
#include "memory.h"
#include "zrtos_arch.h"

//List of TCB pointers
TCB* zrtos_tasks_list[ZRTOS_TASKS_LIST_SIZE];
volatile uint8_t zrtos_tasks_running;

volatile int zrtos_n_tasks;
volatile int zrtos_current_task;
volatile int zrtos_next_task;

void zrtos_add_to_tasks_list(TCB* tcb){  
    for(int i=0;i<ZRTOS_TASKS_LIST_SIZE;i++){
        if(zrtos_tasks_list[i]==NULL){
            zrtos_tasks_list[i]=tcb;
            zrtos_n_tasks++;
            if(zrtos_n_tasks==1) zrtos_current_task=0;
            return;
        }
    }
}

TCB* zrtos_create_task(zrtos_task_function_t task_function,
                        const char* task_name,
                        uint32_t task_stack_size,
                        uint32_t task_priority
){
    TCB* new_TCB = (TCB*)zrtos_malloc(sizeof(TCB));

    if(new_TCB==NULL) return 0;

    //  Allocate stack
    uint32_t* new_stack = (uint32_t*)zrtos_stack_malloc(task_stack_size + 16);
    new_TCB->topOfStack = new_stack;
    new_TCB->taskStackSize = task_stack_size;
    new_TCB->task_function = task_function;

    new_TCB->taskPriority = task_priority;

    int i = 0;
    while (task_name[i] != '\0' && i < ZRTOS_TASK_NAME_MAX_LEN - 1) {
        new_TCB->taskName[i] = task_name[i];
        i++;
    }
    new_TCB->taskName[i] = '\0';

    zrtos_add_to_tasks_list(new_TCB);

    if(zrtos_n_tasks!=1){
        // Push dummy register bank to new task stack

        zrtos_push_dummy(new_TCB,task_function);
    }
    


    return new_TCB;

}

void zrtos_tasks_init(){
    zrtos_tasks_running = 0;
    zrtos_memset(zrtos_tasks_list,0,sizeof(TCB*)*ZRTOS_TASKS_LIST_SIZE);
    zrtos_n_tasks=0;
    zrtos_current_task=0;
    zrtos_next_task=0;
}

TCB* firstTask = 0;
void zrtos_tasks_start(){
    zrtos_tasks_running = 1;
    firstTask = zrtos_tasks_list[0];


    zrtos_current_task = 0;

    //  Switch to thread mode from handler mode
    zrtos_switch_to_thread(firstTask);

    //  Call the first function
    zrtos_tasks_list[0]->task_function();
}