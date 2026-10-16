#include "arch/armv6-m/include/port.h"

void zrtos_push_dummy(TCB* new_TCB, zrtos_task_function_t task_function){


        *(--new_TCB->topOfStack) = (uint32_t)(1U << 24);
        *(--new_TCB->topOfStack) = (uint32_t)task_function;
    
        for(int i=1;i<=14;i++) *(--new_TCB->topOfStack) = (uint32_t)0x00;

}