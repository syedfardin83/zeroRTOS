#include <libopencm3/stm32/rcc.h>

#include "zeroRTOS.h"
#include "zeroRTOS_memory.h"
#include "common-includes.h"

void rcc_setup(void){
    rcc_clock_setup_pll(&rcc_hsi_configs[RCC_CLOCK_CONFIG_HSI_32MHZ]);
}

uint32_t a=0,b=0;
void task1(){
    while(1){
        a++;
    }
}

void task2(){
    while(1){
        b++;
    }
}

int main(){
    rcc_setup();
    zrtos_init();

    //  Task Creation testing
    zrtos_create_task(task1,"Task 1",104,3);
    zrtos_create_task(task2,"Task 2",104,3);

    zrtos_tasks_start();

    while(1){

    }

    return 0;
}