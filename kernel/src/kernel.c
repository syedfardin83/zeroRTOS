#include <libopencm3/cm3/systick.h>
#include <libopencm3/cm3/cortex.h>
#include <libopencm3/cm3/sync.h>
#include <libopencm3/cm3/scb.h>
#include <libopencm3/cm3/nvic.h>

#include "zrtos_config.h"
#include "common-includes.h"
#include "tasks.h"
#include "memory.h"
#include "zrtos_arch.h"

volatile uint64_t ticks=6;

// void zrtos_stack_push()

void zrtos_systick_setup(void){
    systick_set_frequency(ZRTOS_SYSTICK_FREQ, ZRTOS_CLOCK_FREQ);
    systick_counter_enable();
    systick_interrupt_enable();
}

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
        SCB_ICSR |= SCB_ICSR_PENDSVSET;
    }else{
        ticks++;
    }
}

void zrtos_init(){
    zrtos_systick_setup();
    zrtos_tasks_init();
    zrtos_memory_init();


    nvic_set_priority(NVIC_PENDSV_IRQ,0xff);
    nvic_set_priority(NVIC_SYSTICK_IRQ,0x00);
}