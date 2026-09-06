#ifndef MEMORY_H
#define MEMORY_H

#include "common-includes.h"

void* zrtos_malloc(size_t size);
void* zrtos_memset(void* ptr, int c, size_t size);

#endif