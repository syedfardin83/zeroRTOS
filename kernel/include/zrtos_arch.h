#pragma once

#if defined(ZRTOS_ARCH_ARMV6M)
#include "arch/armv6-m/include/port.h"
#elif defined(ZRTOS_ARCH_ARMV7M)
#include "arch/armv7-m/include/port.h"
#else
#error "Processor not defined."
#endif