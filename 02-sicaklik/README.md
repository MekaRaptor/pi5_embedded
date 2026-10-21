# 02 - Sıcaklık Alarmı

Raspberry Pi 5 CPU sıcaklığını okuyan, histerezisli alarm üreten ve
durumu kart üzerindeki ACT LED'i ile gösteren C programı.

## Gereksinimler
| ID | Gereksinim |
|---|---|
| REQ-1 | Sıcaklık 500 ms'de bir okunur ve °C olarak yazdırılır. |
| REQ-2 | Sıcaklık 60.000 °C'den büyükse alarm açılır, LED sürekli yanar. |
| REQ-3 | Alarm, sıcaklık 55.000 °C'nin altına inince kapanır (histerezis). |
| REQ-4 | Sıcaklık okunamazsa sistem SENSOR_HATA durumuna geçer, LED hızlı yanıp söner. |
| REQ-5 | Sensör tekrar okunabilir olduğunda sistem ilk başarılı okumada normal çalışmaya döner. |
| REQ-6 | Sensör hatasından dönüşte sıcaklık ara bölgedeyse (55–60 °C) alarm açık kabul edilir (fail-safe). |
| REQ-7 | Sensör hatası, hataya girildiğinde bir kez raporlanır; sensör geri gelince bir kez bilgi verilir. |

## Tasarım
Sistem üç durumlu bir durum makinesidir: NORMAL, ALARM, SENSOR_HATA.
Durum geçiş mantığı (`durum_guncelle`) donanımdan bağımsızdır ve birim testleriyle doğrulanır.

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
| Gereksinim | Yöntem | Test | Sonuç |
|---|---|---|---|
| REQ-1 | Donanım üzerinde gözlem | – | Geçti |
| REQ-2 | Birim testi (sınır değer) + donanım | TC-01..03 | Geçti |
| REQ-3 | Birim testi (sınır değer) + donanım | TC-04..09 | Geçti |
| REQ-4 | Birim testi + hata enjeksiyonu | TC-10..11 | Geçti |
| REQ-5 | Birim testi + hata enjeksiyonu | TC-12..13 | Geçti |
| REQ-6 | Birim testi (sınır değer) + hata enjeksiyonu | TC-14..16 | Geçti |
| REQ-7 | Hata enjeksiyonu, çıktı gözlemi | – | Geçti |

Testlerin hata yakalama yeteneği mutasyon testi ile doğrulandı

## Kod kapsamı (alarm.c)
GCC 14 gcov ile ölçülmüştür (`make kapsam`).

| Ölçüt | Sonuç |
|---|---|
| Satır (statement) | %100 |
| Dal (decision) | %100 |
| Koşul (masking MC/DC, `-fcondition-coverage`) | %100 |

Not: GCC, masking MC/DC ölçer. Kısa devre (`&&`) nedeniyle
değerlendirilmeyen koşullar maskelenmiş sayılır.

## Statik analiz sapmaları
| Uyarı | Yer | Gerekçe |
|---|---|---|
| constParameter | `sicaklik.c`, `main` | `main` imzası C standardınca sabittir. |


# Kapatılan bulgular
| # | Bulgu | Çözüm |
|---|---|---|
| 1 | Sensör geri geldiğinde davranış tanımsızdı | REQ-5 eklendi |
| 2 | Hata sonrası alarm durumu tanımsızdı | REQ-6 eklendi (fail-safe) |
| 3 | Hata durumunda log taşması | REQ-7 eklendi |
| 4 | "60 °C'yi geçerse" ifadesi belirsizdi | REQ-2 netleştirildi |