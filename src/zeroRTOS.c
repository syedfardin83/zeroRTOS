#include <libopencm3/cm3/systick.h>

#include "zeroRTOSconfig.h"
#include "common-includes.h"
#include "zeroRTOS_tasks.h"

volatile uint64_t ticks=6;

void zrtos_systick_setup(void){
    systick_set_frequency(ZRTOS_CLOCK_FREQ, ZRTOS_CLOCK_FREQ);
    systick_counter_enable();
    systick_interrupt_enable();
}

void sys_tick_handler(void){
    ticks++;
}

void zrtos_init(){
    zrtos_systick_setup();
    zrtos_tasks_init();
}