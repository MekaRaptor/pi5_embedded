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
    kontrol("TC-07b REQ-2 acik, 61.000 C   -> acik", 1, alarm_durumu(61000, 1));
        /* Durum makinesi: normal akış */
    kontrol("TC-08 REQ-3 NORMAL + 57 C      -> NORMAL",
            DURUM_NORMAL, durum_guncelle(DURUM_NORMAL, 1, 57000));
    kontrol("TC-09 REQ-3 ALARM  + 57 C      -> ALARM",
            DURUM_ALARM, durum_guncelle(DURUM_ALARM, 1, 57000));

     /* REQ-4: okuma hatası */
    kontrol("TC-10 REQ-4 NORMAL + hata      -> SENSOR_HATA",
            DURUM_SENSOR_HATA, durum_guncelle(DURUM_NORMAL, 0, 0));
    kontrol("TC-11 REQ-4 ALARM  + hata      -> SENSOR_HATA",
            DURUM_SENSOR_HATA, durum_guncelle(DURUM_ALARM, 0, 0));

    /* REQ-5: kurtarma */
    kontrol("TC-12 REQ-5 HATA   + 50 C      -> NORMAL",
            DURUM_NORMAL, durum_guncelle(DURUM_SENSOR_HATA, 1, 50000));
    kontrol("TC-13 REQ-5 HATA   + 61 C      -> ALARM",
           DURUM_ALARM, durum_guncelle(DURUM_SENSOR_HATA, 1, 61000));

    /* REQ-6: kurtarmada ara bölge, güvenli taraf (sınır değerler) */
    kontrol("TC-14 REQ-6 HATA   + 57 C      -> ALARM",
            DURUM_ALARM, durum_guncelle(DURUM_SENSOR_HATA, 1, 57000));
    kontrol("TC-15 REQ-6 HATA   + 55.000 C  -> ALARM",
            DURUM_ALARM, durum_guncelle(DURUM_SENSOR_HATA, 1, 55000));
    kontrol("TC-16 REQ-6 HATA   + 54.999 C  -> NORMAL",
            DURUM_NORMAL, durum_guncelle(DURUM_SENSOR_HATA, 1, 54999));


    printf("\n%d test basarisiz\n", basarisiz);
    return basarisiz == 0 ? 0 : 1;
}