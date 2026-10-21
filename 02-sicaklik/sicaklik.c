#include <stdio.h>
#include <unistd.h>
#include "alarm.h"

#define LED_PATH   "/sys/class/leds/ACT/"
#define TEMP_PATH  "/sys/class/thermal/thermal_zone0/temp"

#define OKUMA_OK           0
#define OKUMA_DOSYA_YOK   -1
#define OKUMA_BOZUK_VERI  -2

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

/* Hata mesajı yazmaz, sadece sonuç kodu döner (REQ-7 için) */
static int sicaklik_oku(const char *yol, int *sicaklik_mC)
{
    FILE *f = fopen(yol, "r");
    if (f == NULL) {
        return OKUMA_DOSYA_YOK;
    }
    if (fscanf(f, "%d", sicaklik_mC) != 1) {
        fclose(f);
        return OKUMA_BOZUK_VERI;
    }
    fclose(f);
    return OKUMA_OK;
}

int main(int argc, char *argv[])
{
    sistem_durumu_t durum = DURUM_NORMAL;
    const char *sicaklik_yolu = TEMP_PATH;

    if (argc > 1) {
        sicaklik_yolu = argv[1];
    }
    printf("Okunan dosya: %s\n", sicaklik_yolu);

    if (write_file(LED_PATH "trigger", "none") != 0) {
        return 1;
    }

    while (1) {
        int t = 0;
        int sonuc = sicaklik_oku(sicaklik_yolu, &t);
        sistem_durumu_t yeni = durum_guncelle(durum, sonuc == OKUMA_OK, t);

        /* REQ-7: sadece durum değiştiğinde raporla */
        if (yeni != durum) {
            if (yeni == DURUM_SENSOR_HATA) {
                fprintf(stderr, "HATA: sensor okunamadi (%s)\n",
                        sonuc == OKUMA_DOSYA_YOK ? "dosya yok" : "bozuk veri");
            } else if (durum == DURUM_SENSOR_HATA) {
                printf("BILGI: sensor tekrar okunuyor\n");
            }
        }
        durum = yeni;

        if (durum == DURUM_SENSOR_HATA) {
            /* REQ-4: LED hızlı yanıp söner */
            write_file(LED_PATH "brightness", "1");
            usleep(100000);
            write_file(LED_PATH "brightness", "0");
            usleep(100000);
            continue;
        }

        printf("Sicaklik: %d.%02d C  Durum: %s\n",
               t / 1000, (t % 1000) / 10,
               durum == DURUM_ALARM ? "ALARM" : "normal");

        write_file(LED_PATH "brightness", durum == DURUM_ALARM ? "1" : "0");
        usleep(500000);
    }
}