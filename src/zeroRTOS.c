#include <libopencm3/cm3/systick.h>
#include <libopencm3/cm3/cortex.h>
#include <libopencm3/cm3/sync.h>
#include <libopencm3/cm3/scb.h>
#include <libopencm3/cm3/nvic.h>

#include "zeroRTOSconfig.h"
#include "common-includes.h"
#include "zeroRTOS_tasks.h"
#include "zeroRTOS_memory.h"

volatile uint64_t ticks=6;

// void zrtos_stack_push()


void zrtos_switch_to_psp(void){
    
}

void zrtos_systick_setup(void){
    systick_set_frequency(ZRTOS_CLOCK_FREQ, ZRTOS_CLOCK_FREQ);
    systick_counter_enable();
    systick_interrupt_enable();
}

TCB* ct_TCB = 0;
TCB* nt_TCB = 0;
void pend_sv_handler(void){
    ct_TCB = zrtos_tasks_list[zrtos_current_task];
    nt_TCB = zrtos_tasks_list[zrtos_next_task];


    //  Push rest of the registers to current task stack
    __asm__ volatile(
        "mrs r0, psp \n\t" // loading current task topOfStack in r0

        // Pushing r4 to r7 on current task stack
        "sub r0, r0, #4 \n\t"
        "str r4, [r0] \n\t"
        "sub r0, r0, #4 \n\t"
        "str r5, [r0] \n\t"
        "sub r0, r0, #4 \n\t"
        "str r6, [r0] \n\t"
        "sub r0, r0, #4 \n\t"
        "str r7, [r0] \n\t"

        //  Pushing r8 to r11 on current task stack
        "mov r4, r8 \n\t"
        "mov r5, r9 \n\t"
        "mov r6, r10 \n\t"
        "mov r7, r11 \n\t"

        "sub  r0, r0, #4  \n\t"
        "str   r4, [r0]    \n\t"
        "sub  r0, r0, #4  \n\t"
        "str   r5, [r0]    \n\t"
        "sub  r0, r0, #4  \n\t"
        "str   r6, [r0]    \n\t"
        "sub  r0, r0, #4  \n\t"
        "str   r7, [r0]    \n\t"

        //  Updating the topOfStack of the current task TCB
        "ldr r2, =(ct_TCB) \n\t"
        "ldr r3, [r2] \n\t"
        "str r0, [r3] \n\t"

        //  Loading r11-r8 and r4-r7 from next task stack
        "ldr r2, =(nt_TCB) \n\t"    //  r2 has pointer to nt_TCB
        "ldr r3, [r2] \n\t"     // r3 has location of next task TCB
        "ldr r0, [r3] \n\t"     //  r0 has value of new task stack pointer

        //  Loading r11 to r8
        "ldr r4, [r0] \n\t"
        "add r0, r0, #4 \n\t"
        "ldr r5, [r0] \n\t"
        "add r0, r0, #4 \n\t"
        "ldr r6, [r0] \n\t"
        "add r0, r0, #4 \n\t"
        "ldr r7, [r0] \n\t"
        "add r0, r0, #4 \n\t"

        "mov r11, r4 \n\t"
        "mov r10, r5 \n\t"
        "mov r9, r6 \n\t"
        "mov r8, r7 \n\t"

        // Load r7 to r4
        "ldr r4, [r0] \n\t"
        "add r0, r0, #4 \n\t"
        "ldr r5, [r0] \n\t"
        "add r0, r0, #4 \n\t"
        "ldr r6, [r0] \n\t"
        "add r0, r0, #4 \n\t"
        "ldr r7, [r0] \n\t"
        "add r0, r0, #4 \n\t"

        // psp changed to new tast stack pointer
        "msr psp, r0  \n\t"     


        //  return 
        "bx lr \n\t"

    );

    // zrtos_tasks_list[zrtos_next_task]->topOfStack = nt_TCB->topOfStack + 16U;
    zrtos_current_task = zrtos_next_task;

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
    zrtos_memory_init();


    nvic_set_priority(NVIC_PENDSV_IRQ,0xff);
    nvic_set_priority(NVIC_SYSTICK_IRQ,0x00);
}