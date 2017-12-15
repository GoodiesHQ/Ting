#include <ting.h>
#include <stdio.h>
#include <inttypes.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <libnet.h>

#define VAL 12345u

int main()
{
    fprintf(stdout, "Original Value: %u\n", VAL);

    fputs("16 bit:\n", stdout);
    fprintf(stdout, "1 x LE: %" PRIu16 "\n", ting_le16(VAL));
    fprintf(stdout, "2 x LE: %" PRIu16 "\n", ting_le16(ting_le16(VAL)));
    fprintf(stdout, "1 x BE: %" PRIu16 "\n", ting_be16(VAL));
    fprintf(stdout, "2 x BE: %" PRIu16 "\n", ting_be16(ting_be16(VAL)));

    fputc('\n', stdout);

    fputs("32 bit:\n", stdout);
    fprintf(stdout, "1 x LE: %" PRIu32 "\n", ting_le32(VAL));
    fprintf(stdout, "2 x LE: %" PRIu32 "\n", ting_le32(ting_le32(VAL)));
    fprintf(stdout, "1 x BE: %" PRIu32 "\n", ting_be32(VAL));
    fprintf(stdout, "2 x BE: %" PRIu32 "\n", ting_be32(ting_be32(VAL)));
    return 0;
}