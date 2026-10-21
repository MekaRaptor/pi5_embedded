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
static int sicaklik_oku(const char *yol, int *sicaklik_mC)
{
    FILE *f = fopen(yol, "r");
    if (f == NULL) {
        perror(yol);
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

int main(int argc, char *argv[])
{
    int alarm = 0;
    const char *sicaklik_yolu = TEMP_PATH;   /* varsayılan: gerçek sensör */

    if (argc > 1) {
        sicaklik_yolu = argv[1];             /* verilirse: sahte sensör */
    }
    printf("Okunan dosya: %s\n", sicaklik_yolu);

    if (write_file(LED_PATH "trigger", "none") != 0) {
        return 1;
    }

    while (1) {
        int t;
        if (sicaklik_oku(sicaklik_yolu, &t) != 0) {
            /* REQ-4: hata durumu, LED hızlı yanıp söner */
            write_file(LED_PATH "brightness", "1");
            usleep(100000);   /* 100 ms */
            write_file(LED_PATH "brightness", "0");
            usleep(100000);
            continue;
        }

        alarm = alarm_durumu(t, alarm);
        printf("Sicaklik: %d.%02d C  Alarm: %s\n",
               t / 1000, (t % 1000) / 10, alarm ? "ACIK" : "kapali");

        /* alarm değerine göre LED'i yak veya söndür */
        write_file(LED_PATH "brightness", alarm ? "1" : "0");

        usleep(500000);
    }
}