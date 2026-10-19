#include <stdio.h>
#include <unistd.h>

#define LED_PATH "/sys/class/leds/ACT/"

/* Dosyaya değer yazar; hata olursa -1 döner */
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

int main(void)
{
    if (write_file(LED_PATH "trigger", "none") != 0) {
        return 1;
    }

    for (int i = 0; i < 10; i++) {
        write_file(LED_PATH "brightness", "1");
        usleep(500000);   /* 500 ms */
        write_file(LED_PATH "brightness", "0");
        usleep(500000);
    }

    write_file(LED_PATH "trigger", "mmc0");   /* eski görevine döndür */
    return 0;
}