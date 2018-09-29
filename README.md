# thermometer-ATTINY13-NokiaLCD
Low power digital thermometer on ATTINY13+LM35(TMP36) and Nokia 5110 LCD module. Works more than month on single 2xAA battery pack (3V). ATTINY will utilize watchdog for deep sleep and standby.

How to connect Nokia LCD to ATTINY13: LCD_MOSI to PINB2, LCD_SCK to PINB4, LCD_CD to PINB1, LCD_RESET to PINB0. 

Connect LM35/TMP3g Vout to pin PB3 of ATTINY13.

Nokia LCD VSS/VCC/GND, VDD/VCC/GND of LM35/TMP36 and VCC/GND pins of ATTINY13 must be connected to +/- of 2xAA(1.5V) battery pack.
