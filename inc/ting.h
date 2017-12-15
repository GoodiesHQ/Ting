#ifndef TING_TING_H
#define TING_TING_H

#include <ting/gre.h>
#include <ting/packet.h>
#include <ting/endian.h>
#include <stddef.h>
#include <stdbool.h>

typedef bool(*ting_feature_init)(void);
typedef void(*ting_feature_process)(char *buf, uint16_t size);

typedef struct
{
    ting_feature_init init;
    ting_feature_process process;
} ting_feature_t;

ting_feature_t ting_features[] = {
    {.init = ting_feature_gre_init, .process = ting_feature_gre_process},
};

#endif
