#ifndef TING_ENDIAN_H
#define TING_ENDIAN_H

#include <stdint.h>
#include <endian.h>

#if __BYTE_ORDER == __LITTLE_ENDIAN
#define TING_BYTEORDER_LE
#elif __BYTE_ORDER == __BIG_ENDIAN
#define TING_BYTEORDER_BE
#else
#error "Unsupported Byte Order or Fix Endian Defines"
#endif

/*
 * Reverses a 16/32 bit unsigned number
 */
#define ting_rev16(val) ( \
    ((uint16_t)((val) & 0x00FF) << 8) | \
    ((uint16_t)((val) & 0xFF00) >> 8))

#define ting_rev32(val) ( \
    ((uint32_t)((val) & 0x000000FF) << 24) | \
    ((uint32_t)((val) & 0x0000FF00) << 8)  | \
    ((uint32_t)((val) & 0x00FF0000) >> 8)  | \
    ((uint32_t)((val) & 0xFF000000) >> 24));

#if defined(TING_BYTEORDER_LE)
#define ting_le16(val) (val)
#define ting_le32(val) (val)
#define ting_be16(val) ting_rev16(val)
#define ting_be32(val) ting_rev32(val)
#elif defined(TING_BYTEORDER_BE)
#define ting_le16(val) ting_rev16(val)
#define ting_le32(val) ting_rev32(val)
#define ting_be16(val) (val)
#define ting_be32(val) (val)
#endif
#endif //TING_ENDIAN_H
