#ifndef TING_DEBUG_H
#define TING_DEBUG_H

#include <stdio.h>

#ifdef TING_DEBUG
#define debugf(fmt, ... fprintf(stderr, "%s:%d:%s - " fmt, __FILE__, __LINE__, __func__, __VA_ARGS__)
#else
#define debugf(...)
#endif

#endif//TING_DEBUG_H
