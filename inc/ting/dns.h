#ifndef TING_DNS_H
#define TING_DNS_H

#include <ting/packet.h>

/* Note:
 *
 * This will only handle A records.
 */

enum
{
    TING_DNS_RESPONSE_QUERY = 0,
    TING_DNS_RESPONSE_ANSWER = 1,
};

enum
{
    TING_DNS_TYPE_A     = 0x01,
    //TING_DNS_AAA, // TODO: think about it, maybe...
};

#define TING_DNS_MAX_SIZE 576
char ting_buf_dns[TING_DNS_MAX_SIZE];

typedef struct
{
    const char *host;
    uint32_t addr;
} ting_dns_host;

struct dnshdr
{
    uint16_t    id;
#if defined(TING_BYTEORDER_LE)
    uint16_t    recursion_desired:      1,
                truncated:              1,
                authoritative_answer:   1,
                opcode:                 4,
                response:               1,
                response_code:          4,
                checking_disabled:      1,
                authentic_data:         1,
                unused:                 1,
                recursion_available:    1;
#elif defined(TING_BYTEORDER_BE)
    uint16_t    response:               1,
                opcode:                 4,
                authoritative_answer:   1,
                truncated:              1,
                recursion_desired:      1,
                recursion_available:    1,
                unused:                 1,
                authentic_data:         1,
                checking_disabled:      1,
                response_code:          4;
#endif
    uint16_t question_count;
    uint16_t answer_count;
    uint16_t authority_count;
    uint16_t resource_count;
};

bool ting_feature_dns_init(void);
void ting_feature_dns_process(char *buffer, uint16_t size);

#endif//TING_DNS_H
