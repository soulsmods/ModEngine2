#pragma once

#define CLASS_MEMBER(name, type, offset) \
    inline type & ##name () \
    { \
        return *((type *)(((char *)this) + offset)); \
    } \