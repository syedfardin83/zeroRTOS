#include "zeroRTOS_tasks.h"
#include "zeroRTOS_memory.h"

//List of TCB pointers
static TCB* zrtos_tasks_list[ZRTOS_TASKS_LIST_SIZE];

void zrtos_add_to_tasks_list(TCB* tcb){
    for(int i=0;i<ZRTOS_TASKS_LIST_SIZE;i++){
        if(zrtos_tasks_list[i]==NULL){
            zrtos_tasks_list[i]=tcb;
            return;
        }
    }
}

TCB* zrtos_create_task(zrtos_task_function_t task_function,
                        const char* task_name,
                        const size_t task_stack_size,
                        uint32_t task_priority
){
    TCB* new_TCB = (TCB*)zrtos_malloc(sizeof(TCB));

    if(new_TCB==NULL) return 0;

    //  Allocate stack
    uint32_t* new_stack = (uint32_t*)zrtos_malloc(task_stack_size);
    new_TCB->taskStack = new_stack;
    new_TCB->endOfStack=new_stack+task_stack_size;

    new_TCB->taskPriority = task_priority;

    strcpy(new_TCB->taskName,task_name);

    return new_TCB;

}

void zrtos_tasks_init(){
    zrtos_memset(zrtos_tasks_list,0,sizeof(TCB*)*ZRTOS_TASKS_LIST_SIZE);
}