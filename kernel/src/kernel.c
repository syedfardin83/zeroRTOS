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

    if(zrtos_tasks_running){
        //  determine next task to be executed
        if(zrtos_current_task==zrtos_n_tasks-1) zrtos_next_task = 0;
        else zrtos_next_task = zrtos_current_task+1;

            ct_TCB = zrtos_tasks_list[zrtos_current_task];
    nt_TCB = zrtos_tasks_list[zrtos_next_task];

    zrtos_current_task = zrtos_next_task;

    //  Set pendsv bit
    zrtos_set_pendsv();
    }else{
        ticks++;
    }
}

void zrtos_init(){
    zrtos_systick_setup();
    zrtos_tasks_init();
    zrtos_memory_init();
    zrtos_nvic_setup();
}