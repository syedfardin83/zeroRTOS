#include "arch/armv7-m/include/port.h"

void zrtos_systick_setup(void){
    systick_set_frequency(ZRTOS_SYSTICK_FREQ, ZRTOS_CLOCK_FREQ);
    systick_counter_enable();
}

void zrtos_systick_enable(void){
    systick_interrupt_enable();
}

void zrtos_nvic_setup(void){
    nvic_set_priority(NVIC_PENDSV_IRQ,0xff);
    nvic_set_priority(NVIC_SYSTICK_IRQ,0x00);
}

void zrtos_push_dummy(TCB* new_TCB, zrtos_task_function_t task_function){


        *(--new_TCB->topOfStack) = (uint32_t)(1U << 24);
        *(--new_TCB->topOfStack) = (uint32_t)task_function;
    
        for(int i=1;i<=14;i++) *(--new_TCB->topOfStack) = (uint32_t)0x00;

}

void zrtos_set_pendsv(void){
    SCB_ICSR |= SCB_ICSR_PENDSVSET;
}

__attribute__((naked)) void zrtos_start_first_task(void){
    __asm__ volatile(
        "ldr r2, =(ct_TCB)\n\t"
        "ldr r1, [r2]\n\t" // r1 = ct_TCB
        "ldr r0, [r1]\n\t"

        // Change control bit
        "mrs r2, control \n\t"
        "movs r1, #2 \n\t"
        "orr r2, r1 \n\t"
        "msr control, r2 \n\t"
        "isb \n\t"

        //  Take care of the next set of regs
        "add r0, r0, #4 \n\t"
        "add r0, r0, #4 \n\t"
        "add r0, r0, #4 \n\t"
        "add r0, r0, #4 \n\t"
        "add r0, r0, #4 \n\t"
        "add r0, r0, #4 \n\t"
        "add r0, r0, #4 \n\t"
        "add r0, r0, #4 \n\t"
        "add r0, r0, #4 \n\t"
        "add r0, r0, #4 \n\t"
        "add r0, r0, #4 \n\t"
        "add r0, r0, #4 \n\t"
        "add r0, r0, #4 \n\t"
        "add r0, r0, #4 \n\t"
        "ldr r1, [r0]\n\t"
        "add r0, r0, #4 \n\t"
        "add r0, r0, #4 \n\t"

        "msr psp, r0\n\t"

        "bx r1 \n\t"


    );
}

// As the pendsv handler is pure assembly function...
__attribute__((naked)) void pend_sv_handler(void){

    //  Push rest of the registers to current task stack
    __asm__ volatile(
        "mrs r0, psp \n\t" // loading current task topOfStack in r0

        // Pushing r11 to r4 on current task stack
        "sub r0, r0, #4 \n\t"
        "str r11, [r0] \n\t"
        "sub r0, r0, #4 \n\t"
        "str r10, [r0] \n\t"
        "sub r0, r0, #4 \n\t"
        "str r9, [r0] \n\t"
        "sub r0, r0, #4 \n\t"
        "str r8, [r0] \n\t"
        "sub r0, r0, #4 \n\t"
        "str r7, [r0] \n\t"
        "sub r0, r0, #4 \n\t"
        "str r6, [r0] \n\t"
        "sub r0, r0, #4 \n\t"
        "str r5, [r0] \n\t"
        "sub r0, r0, #4 \n\t"
        "str r4, [r0] \n\t"

        //  Updating the topOfStack of the current task TCB
        "ldr r2, =(ct_TCB) \n\t"
        "ldr r3, [r2] \n\t"
        "str r0, [r3] \n\t"

        //  Loading r11-r8 and r4-r7 from next task stack
        "ldr r1, =(nt_TCB) \n\t"    //  r2 has pointer to nt_TCB
        "ldr r3, [r1] \n\t"     // r3 has location of next task TCB
        "ldr r0, [r3] \n\t"     //  r0 has value of new task stack pointer

        //  Change value of ct_TCB to nt_TCB
        "str r3, [r2]\n\t"

        //  Loading r11 to r8
        "ldr r4, [r0] \n\t"
        "add r0, r0, #4 \n\t"
        "ldr r5, [r0] \n\t"
        "add r0, r0, #4 \n\t"
        "ldr r6, [r0] \n\t"
        "add r0, r0, #4 \n\t"
        "ldr r7, [r0] \n\t"
        "add r0, r0, #4 \n\t"
        "ldr r8, [r0] \n\t"
        "add r0, r0, #4 \n\t"
        "ldr r9, [r0] \n\t"
        "add r0, r0, #4 \n\t"
        "ldr r10, [r0] \n\t"
        "add r0, r0, #4 \n\t"
        "ldr r11, [r0] \n\t"
        "add r0, r0, #4 \n\t"


        // psp changed to new task stack pointer
        "msr psp, r0  \n\t"  
        
        //  return 
        "bx lr \n\t"

    );

}