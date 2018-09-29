Projekt termometr na ATTINY85/4X z wyswietlaczem Nokia 5110

Lista części :
1. ATTINY85 (lub ATTINY45, albo mniejszy, kod < 2048 bajt). Jest opcja dla kontrolera ATTINY13 ale mniejsze fonty bo miejsca we FLASH
2. Czujnik temperatury LM36/TMP36 (pomiar -50 - 100st C) lub LM35 (pomiar 0-100st C)
3. Wyswietlacz Nokia 5510/3110 ( wyświetlacz graficzny 84x48, chip zgodny z PCD 8544)
4. Kondensator 100nF + 10 uF ( opcja)

Cechy :

- zasilanie 2 x baterie AA (LR6). Moduł Nokia LCD pracuje na zasilaniu 3.3V
- pobór prądu ok 2-2.5 mA
- mikrokontroler przechodzi w Power Saving Mode po wykonaniu pomiaru i wysłaniu wyniku na LCD Nokia
- praca ponad miesiąc na komplecie baterii AA
- Pomiary wykonywane są raz na 4 sekundy
- Mozliwość edycji własnych fontów dla cyfr i znaków specjalnych

kod obsługi wyświetlacza ze strony : https://blog.thegaragelab.com/nokia-5110-lcd-displays-on-the-attiny/
kod obsługi czujnika temperatury ze strony : http://blog.podkalicki.com/100-projects-on-attiny13/


Podłaczenie 

mikrokontroler ATTINY13/85/4X :
PINY ATTINY:
1 (ADC0) PB5 - nie używany, możliwość doczepienia następnego czujnika temperatury (kabewl Vout) i raportowania z dwóch czujników
2 (ADC3) PB3 - do Vout czujnika LM35 / TMP36 
3 PB4 - do pinu CLK wyswietlacza Nokia 5110 LCD
4 GND - do "-" zasilania 2x AA
5 PB0 - do pinu RST/RESET wyswietlacza Nokia 5110 LCD
6 PB1 - do pinu DC wyswietlacza Nokia 5110 LCD
7 PB2 - do pinu DIN / DATA IN wyswietlacza Nokia 5110 LCD
8 VCC - do "+" zasilania baterii 2x AA
Pomiędzy piny #4 i pin#8 wpinamy kondensator 100nF/10uF

PINY czujnika temperatury LM35 / TMP36 :
VSS - do "+" baterii 2xAA
Vout - do pinu #2 układu ATTINY
GND - do "-" baterii 2xAA

PINY LCD Nokia 5110 :
CS - do "-" zasilania baterii
BL - do "-" zasilania baterii
DC - do pinu #6 attiny
RESET / RST- do pinu #5 attiny
DIN/DATA IN - do pinu #7 attiny
CLK - do pinu #3 attiny
VCC - do "+" zasilania baterii AA
GND - do "-" zasilania baterii AA

 
