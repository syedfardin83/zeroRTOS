#include "zeroRTOS_tasks.h"
#include "zeroRTOS_memory.h"

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

    strcpy(new_TCB->taskName,task_name);

    zrtos_add_to_tasks_list(new_TCB);

    if(zrtos_n_tasks!=1){
        // Push dummy register bank to new task stack
        *(--new_TCB->topOfStack) = (uint32_t)(1U << 24);
        *(--new_TCB->topOfStack) = (uint32_t)task_function;
    
        for(int i=1;i<=14;i++) *(--new_TCB->topOfStack) = (uint32_t)0x00;
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

    __asm__ volatile(
        //  Change psp to topOfStack of task0
        "ldr r3, =(firstTask) \n\t"
        "ldr r2, [r3]\n\t"
        "ldr r1, [r2]\n\t"
        "msr psp, r1\n\t"
        
        //  Change control bit
        "mrs r0, control \n\t"
        "movs r1, #2\n\t"
        "orr r0, r1\n\t"
        "msr control, r0\n\t"
        "isb"
    );

    //  Call the first function
    zrtos_tasks_list[0]->task_function();
}