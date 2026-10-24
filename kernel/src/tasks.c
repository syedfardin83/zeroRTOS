#include "tasks.h"
#include "memory.h"
#include "zrtos_arch.h"


volatile int zrtos_n_tasks;

TCB* zrtos_ready;
TCB* zrtos_delayed;

void zrtos_add_to_tasks_list(TCB* tcb) {
    tcb->next = 0;

    if (zrtos_ready == 0 || tcb->taskPriority > zrtos_ready->taskPriority) {
        tcb->next = zrtos_ready;
        zrtos_ready = tcb;
    } 

    else {
        TCB* curr_node = zrtos_ready;
        while (curr_node->next != 0 && curr_node->next->taskPriority >= tcb->taskPriority) {
            curr_node = curr_node->next;
        }
        tcb->next = curr_node->next;
        curr_node->next = tcb;
    }
    zrtos_n_tasks++;
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

    // Push dummy register bank to new task stack
    zrtos_push_dummy(new_TCB,task_function);
    
    zrtos_add_to_tasks_list(new_TCB);

    return new_TCB;

}

void zrtos_tasks_init(){
    zrtos_n_tasks=0;

    zrtos_ready = 0;
    zrtos_delayed = 0;
}

TCB* firstTask = 0;
extern TCB* ct_TCB;
extern TCB* nt_TCB;
void zrtos_tasks_start(){
    if(zrtos_n_tasks == 0){
        return;
    }

    firstTask = zrtos_ready;
    ct_TCB = zrtos_ready;
    // nt_TCB = zrtos_ready;
    //start first task
    
    
    // start the systick interrupts
    zrtos_systick_enable();
    zrtos_start_first_task();

    while(1);
}