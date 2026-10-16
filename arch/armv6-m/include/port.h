#ifndef PORT_H
#define PORT_H

#include "kernel/include/tasks.h"

void zrtos_push_dummy(TCB* new_TCB, zrtos_task_function_t task_function);

#endif