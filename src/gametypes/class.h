#pragma once

#include <stdint.h>

#define class_offset(type, offset) \
    *((type*)(((uintptr_t)this) + offset))
