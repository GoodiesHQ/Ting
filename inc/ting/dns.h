#ifndef TING_DNS_H
#define TING_DNS_H

#include <ting/packet.h>

/* Note:
 *
 * This will only handle A records.
 */

char ting_buf_dns[TING_PKT_BUF_SIZE];

bool ting_feature_dns_init(void);
void ting_feature_dns_process(char *buffer, uint16_t size);

#endif//TING_DNS_H
