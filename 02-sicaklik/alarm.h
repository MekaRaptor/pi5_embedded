#ifndef ALARM_H
#define ALARM_H

#define ESIK_UST_mC  60000
#define ESIK_ALT_mC  55000

typedef enum {
    DURUM_NORMAL = 0,
    DURUM_ALARM,
    DURUM_SENSOR_HATA
} sistem_durumu_t;

int alarm_durumu(int sicaklik_mC, int onceki_durum);

sistem_durumu_t durum_guncelle(sistem_durumu_t onceki,
                               int okuma_basarili,
                               int sicaklik_mC);

#endif