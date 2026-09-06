#include <libopencm3/stm32/rcc.h>

#include "zeroRTOS.h"
#include "zeroRTOS_memory.h"
#include "common-includes.h"

void rcc_setup(void){
    rcc_clock_setup_pll(&rcc_hsi_configs[RCC_CLOCK_CONFIG_HSI_32MHZ]);
}

void task1(){

}

int main(){
    rcc_setup();
    zrtos_init();

    // Memory testing
    // char* c = (char*)zrtos_malloc(sizeof(char));
    // *c = 'H';

    //  Task Creation testing
    zrtos_create_task(task1,"Task 1",100,3);


    while(1){

    }

    return 0;
}