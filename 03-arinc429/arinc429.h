#ifndef ARINC429_H
#define ARINC429_H

#include <stdint.h>

#define A429_OK            0
#define A429_ALAN_HATASI  -1
#define A429_PARITY_HATASI -2

typedef struct {
    uint8_t  label;   /* 8 bit, genelde octal yazılır: 0203 */
    uint8_t  sdi;     /* 2 bit: 0..3 */
    uint32_t data;    /* 19 bit: 0..0x7FFFF */
    uint8_t  ssm;     /* 2 bit: 0..3 */
} a429_alanlar_t;

int a429_kodla(const a429_alanlar_t *a, uint32_t *kelime);
int a429_coz(uint32_t kelime, a429_alanlar_t *a);

#endif