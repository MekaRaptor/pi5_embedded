#include <stdio.h>
#include "arinc429.h"

static int basarisiz = 0;

static void kontrol(const char *ad, unsigned long beklenen, unsigned long gercek)
{
    if (beklenen == gercek) {
        printf("[GECTI] %s\n", ad);
    } else {
        printf("[KALDI] %s -> beklenen 0x%08lX, gelen 0x%08lX\n",
               ad, beklenen, gercek);
        basarisiz++;
    }
}

int main(void)
{
    uint32_t k = 0;
    int r;
    a429_alanlar_t a;
    a429_alanlar_t c;

    /* ---- Kodlama: REQ-1, REQ-2 ---- */
    a = (a429_alanlar_t){ 0, 0, 0, 0 };
    a429_kodla(&a, &k);
    kontrol("TC-01 REQ-2 tum alanlar 0, parity=1", 0x80000000UL, k);

    a = (a429_alanlar_t){ 0203, 0, 0, 0 };   /* 0203 octal = 0x83 */
    a429_kodla(&a, &k);
    kontrol("TC-02 REQ-1 label 203 (octal)", 0x000000C1UL, k);
    
    a = (a429_alanlar_t){ 0001, 0, 0, 0 };   /* 0000 0001 -> 1000 0000 */
    a429_kodla(&a, &k);
    kontrol("TC-02b REQ-6 label 001 -> 0x80", 0x00000080UL, k);

    a = (a429_alanlar_t){ 0200, 0, 0, 0 };   /* 1000 0000 -> 0000 0001 */
    a429_kodla(&a, &k);
    kontrol("TC-02c REQ-6 label 200 -> 0x01", 0x00000001UL, k);

    a = (a429_alanlar_t){ 0203, 1, 1, 3 };
    a429_kodla(&a, &k);
    kontrol("TC-03 REQ-1 label 203, SDI 1, data 1, SSM 3", 0x600005C1UL, k);

    a = (a429_alanlar_t){ 0, 0, 0x7FFFF, 0 };
    a429_kodla(&a, &k);
    kontrol("TC-04 REQ-1 data en buyuk deger", 0x1FFFFC00UL, k);

    a = (a429_alanlar_t){ 0xFF, 3, 0x7FFFF, 3 };
    a429_kodla(&a, &k);
    kontrol("TC-05 REQ-2 tum alanlar dolu, parity=0", 0x7FFFFFFFUL, k);

    /* ---- Kodlama: REQ-3, sınır değerler ---- */
    a = (a429_alanlar_t){ 0203, 4, 0, 0 };
    kontrol("TC-06 REQ-3 SDI=4 reddedilir",
            (unsigned long)A429_ALAN_HATASI, (unsigned long)a429_kodla(&a, &k));

    a = (a429_alanlar_t){ 0203, 0, 0x80000, 0 };
    kontrol("TC-07 REQ-3 data 20 bit reddedilir",
            (unsigned long)A429_ALAN_HATASI, (unsigned long)a429_kodla(&a, &k));

    a = (a429_alanlar_t){ 0203, 0, 0, 4 };
    kontrol("TC-08 REQ-3 SSM=4 reddedilir",
            (unsigned long)A429_ALAN_HATASI, (unsigned long)a429_kodla(&a, &k));

    /* ---- Çözme: REQ-4 ---- */
    r = a429_coz(0x600005C1UL, &c);
    kontrol("TC-09 REQ-4 coz: sonuc OK", A429_OK, (unsigned long)r);
    kontrol("TC-10 REQ-4 coz: label", 0203, c.label);
    kontrol("TC-11 REQ-4 coz: SDI",   1,    c.sdi);
    kontrol("TC-12 REQ-4 coz: data",  1,    c.data);
    kontrol("TC-13 REQ-4 coz: SSM",   3,    c.ssm);

    /* ---- Çözme: REQ-5, tek bit hatası ---- */
    r = a429_coz(0x600005C1UL ^ (1UL << 12), &c);
    kontrol("TC-14 REQ-5 tek bit bozuk -> parity hatasi",
            (unsigned long)A429_PARITY_HATASI, (unsigned long)r);

    printf("\n%d test basarisiz\n", basarisiz);
    return basarisiz == 0 ? 0 : 1;
}