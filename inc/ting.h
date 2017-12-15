#ifndef TING_TING_H
#define TING_TING_H

#include <ting/endian.h>
#include <ting/packet.h>
#include <ting/gre.h>
#include <stddef.h>
#include <stdbool.h>

typedef bool(*ting_feature_init)(void);
typedef void(*ting_feature_process)(char *buf, size_t size);

#endif
