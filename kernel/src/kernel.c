#include "zrtos_config.h"
#include "common-includes.h"
#include "tasks.h"
#include "memory.h"
#include "zrtos_arch.h"

volatile uint64_t zrtos_ticks=0;

TCB* ct_TCB = 0;
TCB* nt_TCB = 0;

//  PendSV handler in port.c

void sys_tick_handler(void){
    zrtos_ticks++;

    //  Check for ready tasks
    while(zrtos_delayed && zrtos_delayed->wake_tick <= zrtos_ticks){
        //  Remove it from delayed list
        TCB* temp = zrtos_delayed;
        temp->next = 0;
        zrtos_delayed = zrtos_delayed->next;
    
        zrtos_add_to_ready_list(temp);
    }

    //  determine next task to be executed
    if (ct_TCB && ct_TCB->next && (ct_TCB->next->taskPriority == zrtos_ready->taskPriority)) {
        nt_TCB = ct_TCB->next;
    } else {
        nt_TCB = zrtos_ready;
    }

    //  Set pendsv bit
    zrtos_set_pendsv();

}

void zrtos_init(){
    zrtos_systick_setup();
    zrtos_tasks_init();
    zrtos_memory_init();
    zrtos_nvic_setup();
}