#ifndef MEMORY_H
#define MEMORY_H

#include "common-includes.h"

void* zrtos_malloc(size_t size);
void zrtos_memory_init();

#endif