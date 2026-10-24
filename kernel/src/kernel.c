#include "zrtos_config.h"
#include "common-includes.h"
#include "tasks.h"
#include "memory.h"
#include "zrtos_arch.h"

volatile uint64_t ticks=6;

TCB* ct_TCB = 0;
TCB* nt_TCB = 0;

//  PendSV handler in port.c

void sys_tick_handler(void){

    //  determine next task to be executed
    if(ct_TCB->next) nt_TCB = ct_TCB->next;
    else nt_TCB = zrtos_ready;

    //  Set pendsv bit
    zrtos_set_pendsv();

}

void zrtos_init(){
    zrtos_systick_setup();
    zrtos_tasks_init();
    zrtos_memory_init();
    zrtos_nvic_setup();
}