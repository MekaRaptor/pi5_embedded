#include "alarm.h"

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