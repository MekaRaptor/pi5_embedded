# 02 - Sıcaklık Alarmı

Raspberry Pi 5 CPU sıcaklığını okuyan, histerezisli alarm üreten ve
durumu kart üzerindeki ACT LED'i ile gösteren C programı.

## Gereksinimler
| ID | Gereksinim |
|---|---|
| REQ-1 | Sıcaklık 500 ms'de bir okunur ve °C olarak yazdırılır. |
| REQ-2 | Sıcaklık 60 °C'yi geçerse alarm açılır, LED sürekli yanar. |
| REQ-3 | Alarm, sıcaklık 55 °C'nin altına inince kapanır (histerezis). |
| REQ-4 | Sıcaklık okunamazsa hata mesajı verilir, LED hızlı yanıp söner. |

## Yapı
- `alarm.c / alarm.h` – donanımdan bağımsız alarm mantığı
- `sicaklik.c` – donanım katmanı (sensör okuma, LED)
- `test_alarm.c` – birim testleri

## Kullanım
    make                               # derle
    make test                          # birim testleri
    make analiz                        # cppcheck statik analiz
    sudo ./sicaklik                    # gerçek sensör
    sudo ./sicaklik /tmp/sahte_sicaklik  # sahte sensör (hata enjeksiyonu)

## Doğrulama
| Gereksinim | Yöntem | Sonuç |
|---|---|---|
| REQ-1 | Donanım üzerinde gözlem | Geçti |
| REQ-2 | Birim testi TC-01..03 (sınır değer) + donanım | Geçti |
| REQ-3 | Birim testi TC-04..07 (sınır değer) + donanım | Geçti |
| REQ-4 | Hata enjeksiyonu (dosya silme, bozuk veri) | Geçti |

Testlerin hata yakalama yeteneği mutasyon testi ile doğrulandı
(`>`→`>=`, `<`→`<=`, eşik değeri değişikliği; üçü de yakalandı).

## Açık bulgular
1. Sensör tekrar okunabilir olduğunda beklenen davranış tanımlı değil.
2. Hata sırasında ve sonrasında alarm durumunun ne olacağı tanımlı değil.
3. Hata durumunda saniyede 5 mesaj yazılıyor; uzun arızada log şişebilir.
4. REQ-2'deki "geçerse" ifadesi tam 60.000 °C için belirsiz; >60.000 olarak yorumlandı.