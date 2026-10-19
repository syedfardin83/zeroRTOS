#ifndef PORT_H
#define PORT_H

//  configs
#define ZRTOS_CLOCK_FREQ          (32000000)


#include "kernel/include/tasks.h"
#include <libopencm3/cm3/scb.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>

void zrtos_push_dummy(TCB* new_TCB, zrtos_task_function_t task_function);
void zrtos_switch_to_thread(TCB* first_task);
void zrtos_set_pendsv(void);
void zrtos_nvic_setup(void);
void zrtos_systick_setup(void);

#endif