#include <stdio.h>
#include "alarm.h"
//boundary test 
static int basarisiz = 0;

static void kontrol(const char *ad, int beklenen, int gercek)
{
    if (beklenen == gercek) {
        printf("[GECTI] %s\n", ad);
    } else {
        printf("[KALDI] %s -> beklenen %d, gelen %d\n", ad, beklenen, gercek);
        basarisiz++;
    }
}

int main(void)
{
    /* REQ-2: alarm kapalıyken üst eşik */
    kontrol("TC-01 REQ-2 kapali, 50.000 C  -> kapali", 0, alarm_durumu(50000, 0));
    kontrol("TC-02 REQ-2 kapali, 60.000 C  -> kapali", 0, alarm_durumu(60000, 0));
    kontrol("TC-03 REQ-2 kapali, 60.001 C  -> acik",   1, alarm_durumu(60001, 0));

    /* REQ-3: alarm açıkken alt eşik */
    kontrol("TC-04 REQ-3 acik,   57.000 C  -> acik",   1, alarm_durumu(57000, 1));
    kontrol("TC-05 REQ-3 acik,   55.000 C  -> acik",   1, alarm_durumu(55000, 1));
    kontrol("TC-06 REQ-3 acik,   54.999 C  -> kapali", 0, alarm_durumu(54999, 1));

    /* REQ-3: ara bölgede kapalı durum korunur */
    kontrol("TC-07 REQ-3 kapali, 57.000 C  -> kapali", 0, alarm_durumu(57000, 0));

    printf("\n%d test basarisiz\n", basarisiz);
    return basarisiz == 0 ? 0 : 1;
}