#include <libopencm3/cm3/systick.h>
#include <libopencm3/cm3/cortex.h>
#include <libopencm3/cm3/sync.h>
#include <libopencm3/cm3/scb.h>
#include <libopencm3/cm3/nvic.h>

#include "zeroRTOSconfig.h"
#include "common-includes.h"
#include "zeroRTOS_tasks.h"

volatile uint64_t ticks=6;

// void zrtos_stack_push()


void zrtos_switch_to_psp(void){
    
}

void zrtos_systick_setup(void){
    systick_set_frequency(ZRTOS_CLOCK_FREQ, ZRTOS_CLOCK_FREQ);
    systick_counter_enable();
    systick_interrupt_enable();
}

void pend_sv_handler(void){
    
}

void sys_tick_handler(void){

    if(zrtos_tasks_running){
        //  determine next task to be executed
        if(zrtos_current_task==zrtos_n_tasks-1) zrtos_next_task = 0;
        else zrtos_next_task = zrtos_current_task+1;

        //  Set pendsv bit
        SCB_ICSR |= SCB_ICSR_PENDSVSET;
    }else{
        ticks++;
    }
}

void zrtos_init(){
    zrtos_systick_setup();
    zrtos_tasks_init();

    nvic_set_priority(NVIC_PENDSV_IRQ,0xff);
    nvic_set_priority(NVIC_SYSTICK_IRQ,0x00);
}