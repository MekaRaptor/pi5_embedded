#include <stdio.h>
#include <unistd.h>
#include "alarm.h"

#define LED_PATH   "/sys/class/leds/ACT/"
#define TEMP_PATH  "/sys/class/thermal/thermal_zone0/temp"

//#define ESIK_UST_mC  60000   /* 60.0 °C: alarm başlar  */
//#define ESIK_ALT_mC  55000   /* 55.0 °C: alarm biter   */

static int write_file(const char *path, const char *value)
{
    FILE *f = fopen(path, "w");
    if (f == NULL) {
        perror(path);
        return -1;
    }
    if (fputs(value, f) == EOF) {
        perror(path);
        fclose(f);
        return -1;
    }
    if (fclose(f) != 0) {
        perror(path);
        return -1;
    }
    return 0;
}

/* Sıcaklığı milidereceler cinsinden okur.
   Başarılıysa 0 döner ve değeri *sicaklik_mC'ye yazar, hata olursa -1 döner. */
static int sicaklik_oku(int *sicaklik_mC)
{
    FILE *f = fopen(TEMP_PATH, "r");
    if (f == NULL) {
        perror(TEMP_PATH);
        return -1;
    }
    /* fscanf ile dosyadan bir tam sayı oku. fscanf okuduğu değer sayısını döner; 1 değilse hata say. */
    if (fscanf(f, "%d", sicaklik_mC) != 1) {
        fprintf(stderr, "Sicaklik degeri okunamadi\n");
        fclose(f);
        return -1;
    }
    fclose(f);
    return 0;
}

/* Donanıma dokunmayan saf mantık: alarm açık mı olmalı?
   onceki_durum: 1 = alarm açıktı, 0 = kapalıydı */
// int alarm_durumu(int sicaklik_mC, int onceki_durum)
// {
//     /* REQ-2 ve REQ-3 (histerezis) */
//     if(onceki_durum == 0 && sicaklik_mC > ESIK_UST_mC) {
//         return 1; // Alarm aç
//     } else if(onceki_durum == 1 && sicaklik_mC < ESIK_ALT_mC) {
//         return 0; // Alarm kapa
//     }
//     return onceki_durum;
// }

int main(void)
{
    int alarm = 0;

    if (write_file(LED_PATH "trigger", "none") != 0) {
        return 1;
    }

    while (1) {
        int t;
        if (sicaklik_oku(&t) != 0) {
            /* TODO: REQ-4, LED'i hızlı yakıp söndür */
            write_file(LED_PATH "brightness", "1");
            usleep(100000);   /* 100 ms */
            write_file(LED_PATH "brightness", "0");
            usleep(100000);
            continue;
        }

        alarm = alarm_durumu(t, alarm);
        printf("Sicaklik: %d.%02d C  Alarm: %s\n",
               t / 1000, (t % 1000) / 10, alarm ? "ACIK" : "kapali");

        /*alarm değerine göre LED'e "1" veya "0" yaz */
        write_file(LED_PATH "brightness", alarm ? "1" : "0");

        usleep(500000);
    }
}