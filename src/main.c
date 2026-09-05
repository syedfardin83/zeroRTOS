#include <libopencm3/stm32/rcc.h>

#include "zeroRTOS.h"

void rcc_setup(void){
    rcc_clock_setup_pll(&rcc_hsi_configs[RCC_CLOCK_CONFIG_HSI_32MHZ]);
}

int main(){
    rcc_setup();
    zeroRTOS_init();

    return 0;
}