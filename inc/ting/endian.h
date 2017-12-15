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
uint16_t ting_rev16(uint16_t val);
uint32_t ting_rev32(uint32_t val);

/*
 * Converts a 16/32 bit number from the CPU endianness to little endian
 */
uint16_t ting_le16(uint16_t val);
uint32_t ting_le32(uint32_t val);

/*
 * Converts a 16/32 bit number from the CPU endianness to big endian
 */
uint16_t ting_be16(uint16_t val);
uint32_t ting_be32(uint32_t val);

#endif //TING_ENDIAN_H
