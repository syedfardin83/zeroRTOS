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
    uint32_t* nt_tos = zrtos_tasks_list[zrtos_next_task]->topOfStack;
    
    //  Push rest of the registers to current task stack
    __asm__ volatile(
        "mrs r0, psp \n\t" // loading current task topOfStack in r0

        // Pushing r4 to r7 on current task stack
        "subs r0, r0, #4 \n\t"
        "str r4, [r0] \n\t"
        "subs r0, r0, #4 \n\t"
        "str r5, [r0] \n\t"
        "subs r0, r0, #4 \n\t"
        "str r6, [r0] \n\t"
        "subs r0, r0, #4 \n\t"
        "str r7, [r0] \n\t"

        //  Pushing r8 to r11 on current task stack
        "mov r4, r8 \n\t"
        "mov r5, r9 \n\t"
        "mov r6, r10 \n\t"
        "mov r7, r11 \n\t"

        "subs r0, r0, #4 \n\t"
        "str r4, [r0] \n\t"
        "subs r0, r0, #4 \n\t"
        "str r5, [r0] \n\t"
        "subs r0, r0, #4 \n\t"
        "str r6, [r0] \n\t"
        "subs r0, r0, #4 \n\t"
        "str r7, [r0] \n\t"

    );
    //  Updating current task TCB topOfStack
    zrtos_tasks_list[zrtos_current_task]->topOfStack += 16U; //!!  

    //  Loading r11 to r8 from next task stack
    __asm__ volatile(
        ""
    );



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