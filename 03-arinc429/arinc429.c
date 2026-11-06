#include "arinc429.h"

#define SDI_KAYMA    8u
#define DATA_KAYMA  10u
#define SSM_KAYMA   29u
#define PARITY_BIT  31u

#define LABEL_MASKE  0xFFu
#define SDI_MASKE    0x3u
#define DATA_MASKE   0x7FFFFu    /* 19 bit */
#define SSM_MASKE    0x3u

/* Bir sayıdaki 1 bitlerini sayar */
static unsigned int bir_say(uint32_t x)
{
    unsigned int n = 0;
    while (x != 0u) {
        n += x & 1u;
        x >>= 1;
    }
    return n;
}

int a429_kodla(const a429_alanlar_t *a, uint32_t *kelime)
{
    uint32_t k = 0;

    /* A429-REQ-3: alana sığmayan değer reddedilir */
    if (a->sdi > SDI_MASKE || a->data > DATA_MASKE || a->ssm > SSM_MASKE) {
        return A429_ALAN_HATASI;
    }

    /* A429-REQ-1: alanları yerleştir */
    k |= (uint32_t)a->label;
    k |= (uint32_t)a->sdi << SDI_KAYMA;
    k |= a->data << DATA_KAYMA;
    k |= (uint32_t)a->ssm << SSM_KAYMA;

    /* A429-REQ-2: tek parity. 1 sayısı çiftse parity bitini yak */
    if (bir_say(k) % 2u == 0u) {
        k |= 1u << PARITY_BIT;
    }

    *kelime = k;
    return A429_OK;
}

int a429_coz(uint32_t kelime, a429_alanlar_t *a)
{
    /* A429-REQ-5: tek parity kontrolü */
    if (bir_say(kelime) % 2u == 0u) {
        return A429_PARITY_HATASI;
    }

    /* A429-REQ-4: alanları ayır */
    a->label = (uint8_t)(kelime & LABEL_MASKE);
    a->sdi   = (uint8_t)((kelime >> SDI_KAYMA) & SDI_MASKE);
    a->data  = (kelime >> DATA_KAYMA) & DATA_MASKE;
    a->ssm   = (uint8_t)((kelime >> SSM_KAYMA) & SSM_MASKE);

    return A429_OK;
}