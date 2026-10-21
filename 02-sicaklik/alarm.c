#include "alarm.h"

/* REQ-2, REQ-3: histerezisli alarm kararı */
int alarm_durumu(int sicaklik_mC, int onceki_durum)
{
    if (onceki_durum == 0 && sicaklik_mC > ESIK_UST_mC) {
        return 1;
    }
    if (onceki_durum == 1 && sicaklik_mC < ESIK_ALT_mC) {
        return 0;
    }
    return onceki_durum;
}

/* Sistem durum makinesi */
sistem_durumu_t durum_guncelle(sistem_durumu_t onceki,
                               int okuma_basarili,
                               int sicaklik_mC)
{
    int onceki_alarm;

    /* REQ-4: okuma hatası her durumdan SENSOR_HATA'ya götürür */
    if (!okuma_basarili) {
        return DURUM_SENSOR_HATA;
    }

    if (onceki == DURUM_NORMAL) {
        onceki_alarm = 0;
    } else {
        /* DURUM_ALARM ya da DURUM_SENSOR_HATA.
           REQ-6: hatadan dönüşte güvenli taraf, alarm açık varsayılır */
        onceki_alarm = 1;
    }

    /* REQ-5: başarılı okumada normal değerlendirmeye dönülür */
    return alarm_durumu(sicaklik_mC, onceki_alarm) ? DURUM_ALARM : DURUM_NORMAL;
}