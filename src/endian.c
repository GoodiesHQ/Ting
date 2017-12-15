#include <ting/endian.h>

uint16_t ting_rev16(uint16_t val)
{
    return (((uint16_t)(val & 0x00FF) << 8) |
            ((uint16_t)(val & 0xFF00) >> 8));
}

uint32_t ting_rev32(uint32_t val)
{
    return (((uint32_t)(val & 0x000000FF) << 24) |
            ((uint32_t)(val & 0x0000FF00) << 8)  |
            ((uint32_t)(val & 0x00FF0000) >> 8)  |
            ((uint32_t)(val & 0xFF000000) >> 24));
}

uint16_t ting_le16(uint16_t val)
{
#if defined(TING_BYTEORDER_LE)
    return val;
#elif defined(TING_BYTEORDER_BE)
    return ting_rev16(val);
#endif
}

uint32_t ting_le32(uint32_t val)
{
#if defined(TING_BYTEORDER_LE)
    return val;
#elif defined(TING_BYTEORDER_BE)
    return ting_rev32(val);
#endif
}

uint16_t ting_be16(uint16_t val)
{
#if defined(TING_BYTEORDER_LE)
    return ting_rev16(val);
#elif defined(TING_BYTEORDER_BE)
    return val;
#endif
}

uint32_t ting_be32(uint32_t val)
{
#if defined(TING_BYTEORDER_LE)
    return ting_rev32(val);
#elif defined(TING_BYTEORDER_BE)
    return val;
#endif
}
