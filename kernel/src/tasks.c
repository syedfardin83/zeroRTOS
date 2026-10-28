#include "tasks.h"
#include "memory.h"
#include "zrtos_arch.h"
#include "kernel/include/zrtos_config.h"


volatile int zrtos_n_tasks;

TCB* zrtos_ready;
TCB* zrtos_delayed;

void zrtos_add_to_ready_list(TCB* tcb) {
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
    
    zrtos_add_to_ready_list(new_TCB);

    return new_TCB;

}

void zrtos_idle_task(void){
    while(1){
        __asm__ volatile(
            "wfi\n\t"
        );
    }
}

void zrtos_tasks_init(){
    zrtos_n_tasks=0;

    zrtos_ready = 0;
    zrtos_delayed = 0;

    zrtos_create_task(zrtos_idle_task,"ZRTOS IDLE TASK",ZRTOS_IDLE_TASK_STACK_SIZE,0);
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

void zrtos_delay(uint64_t n){
    ct_TCB->wake_tick = zrtos_ticks+n;

    TCB* curr_TCB = zrtos_ready;

    //  Remove the ct_TCB from the ready list
    TCB* prev = 0;
    TCB* curr = zrtos_ready;

    while (curr && curr != ct_TCB) {
        prev = curr;
        curr = curr->next;
    }

    if (curr == ct_TCB) {
        if (prev) {
            prev->next = curr->next;
        } else {
            zrtos_ready = curr->next;
        }
        curr->next = 0;
    }

    // Add the ct_TCB to delayed list
    curr_TCB = zrtos_delayed;
    if (zrtos_delayed == 0 || ct_TCB->wake_tick < zrtos_delayed->wake_tick) {
        ct_TCB->next = zrtos_delayed;
        zrtos_delayed = ct_TCB;
    } else {
        TCB* curr = zrtos_delayed;
        while (curr->next != 0 && curr->next->wake_tick <= ct_TCB->wake_tick) {
            curr = curr->next;
        }
        ct_TCB->next = curr->next;
        curr->next = ct_TCB;
    }

    nt_TCB = zrtos_ready;
    zrtos_set_pendsv();

}

