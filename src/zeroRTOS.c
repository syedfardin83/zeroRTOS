#include <libopencm3/cm3/systick.h>

#include "zeroRTOSconfig.h"
#include "common-includes.h"

volatile uint64_t ticks=6;

void zeroRTOS_systick_setup(void){
    systick_set_frequency(SYSTICK_FREQ, CLOCK_FREQ);
    systick_counter_enable();
    systick_interrupt_enable();
}

void sys_tick_handler(void){
    ticks++;
}

void zeroRTOS_init(){
    zeroRTOS_systick_setup();
}