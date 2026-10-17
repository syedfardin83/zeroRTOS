#ifndef PORT_H
#define PORT_H

//  configs
#define ZRTOS_CLOCK_FREQ          (32000000)


#include "kernel/include/tasks.h"
#include <libopencm3/cm3/scb.h>

void zrtos_push_dummy(TCB* new_TCB, zrtos_task_function_t task_function);
void zrtos_switch_to_thread(TCB* first_task);
void zrtos_set_pendsv(void);

#endif