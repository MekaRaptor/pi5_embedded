# 03 - ARINC 429 Kodlayıcı / Çözücü

ARINC 429 32 bitlik veri kelimesini alanlarına göre paketleyen ve çözen C kütüphanesi.

## Kelime yapısı
| Alan | Bitler |
|---|---|
| Label (ters bit sırası) | 1–8 |
| SDI | 9–10 |
| Data | 11–29 |
| SSM | 30–31 |
| Parity (tek) | 32 |

## Gereksinimler
| ID | Gereksinim |
|---|---|
| A429-REQ-1 | Kodlayıcı label, SDI, data ve SSM alanlarını standarttaki bit konumlarına yerleştirir. |
| A429-REQ-2 | Kodlayıcı parity bitini, kelimedeki 1'lerin toplam sayısı tek olacak şekilde ayarlar. |
| A429-REQ-3 | Kodlayıcı, alanına sığmayan bir değer gelirse kelime üretmez ve hata döner. |
| A429-REQ-4 | Çözücü bir kelimeden label, SDI, data ve SSM alanlarını ayırır. |
| A429-REQ-5 | Çözücü, parity'si hatalı bir kelimeyi reddeder ve hata döner. |
| A429-REQ-6 | Label, 1–8. bitlere ters bit sırasıyla yerleştirilir (bit 1 = label MSB); çözücü aynı kuralla geri çevirir. |

## Doğrulama
| Gereksinim | Test |
|---|---|
| A429-REQ-1 | TC-02, TC-03, TC-04 |
| A429-REQ-2 | TC-01, TC-05 |
| A429-REQ-3 | TC-06..08 (sınır değer) |
| A429-REQ-4 | TC-09..13 |
| A429-REQ-5 | TC-14 (tek bit hata enjeksiyonu) |
| A429-REQ-6 | TC-02b, TC-02c, TC-10 |

Kod kapsamı (`make kapsam`, GCC 14 gcov): satır %100, dal %100, koşul (masking MC/DC) %100.

## Notlar
- A429-REQ-6 test-önce (TDD) yaklaşımıyla eklendi: beklenen değerler önce
  standarda göre düzeltildi, testlerin kaldığı görüldü, sonra kod düzeltildi.
- İlk sürümde testler ve kod aynı basitleştirilmiş varsayımla yazıldığı için
  label sırası hatası testlerle yakalanmamıştı. Testlerin gereksinimden
  bağımsız türetilmesi gerektiğinin bir örneği.