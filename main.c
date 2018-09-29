
/*--------------------------------------------------------------------------*
* Digital thermometer on LM35, ATTINY13 with NOKIA 5110
* with power saving mode, ultra low power, must be powered from 3,3V ( LM7803)
*--------------------------------------------------------------------------*/


#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>

 
/** Glyph data (stored in PROGMEM)
 */
extern const uint8_t SMALL_FONT[] PROGMEM;


// SMALL FONTS only fit into progmem

const uint8_t SMALL_FONT[] PROGMEM = {
  0x3e, 0x51, 0x49, 0x45, 0x3e, // 30 0
  0x00, 0x42, 0x7f, 0x40, 0x00, // 31 1
  0x42, 0x61, 0x51, 0x49, 0x46, // 32 2
  0x21, 0x41, 0x45, 0x4b, 0x31, // 33 3
  0x18, 0x14, 0x12, 0x7f, 0x10, // 34 4
  0x27, 0x45, 0x45, 0x45, 0x39, // 35 5
  0x3c, 0x4a, 0x49, 0x49, 0x30, // 36 6
  0x01, 0x71, 0x09, 0x05, 0x03, // 37 7
  0x36, 0x49, 0x49, 0x49, 0x36, // 38 8
  0x06, 0x49, 0x49, 0x29, 0x1e, // 39 9
  0x08, 0x08, 0x08, 0x08, 0x08  // 2d -
  };


// Nokia LCD pin numbers
#define LCD_MOSI  PINB2
#define LCD_SCK   PINB4
#define LCD_CD    PINB1
#define LCD_RESET PINB0

/** Number of columns */
#define LCD_COL 84

/** Number of text rows */
#define LCD_ROW 6


/** Transfer data to a slave (MSB first)
 *
 * @param sck the pin to use for the SCK output
 * @param mosi the pin to use for the MOSI output
 * @param data the data to transfer
 * @param bits the number of bits to transfer
 */
void sspiOutMSB(uint8_t sck, uint8_t mosi, uint16_t data, uint8_t bits) {
  uint16_t mask = (1 << (bits - 1));
  uint8_t output = (1 << mosi);
  uint8_t clock = (1 << sck);
  while(bits) {
    // Set data
    if(data&mask)
      PORTB |= output;
    else
      PORTB &= ~output;
    // Bring the clock high
    PORTB |= clock;
    // Move to the next bit
    mask = mask >> 1;
    bits--;
    // Bring the clock low again
    PORTB &= ~clock;
    }
  }



/** Send a data byte to the LCD
 *
 * @param data the data byte to send.
 */
void lcdData(uint8_t data) {
  // Bring CD high
  PORTB |= (1 << LCD_CD);
  // Send the data
  sspiOutMSB(LCD_SCK, LCD_MOSI, data, 8);
  }

/** Send a command byte to the LCD
 *
 * @param cmd the command byte to send.
 */
void lcdCommand(uint8_t cmd) {
  // Bring CD low
  PORTB &= ~(1 << LCD_CD);
  // Send the data
  sspiOutMSB(LCD_SCK, LCD_MOSI, cmd, 8);
  }


/** Write a single character
 */

void lcdPrintChar(uint8_t row, uint8_t col, uint8_t ch) {

  // Set the starting address
  const uint8_t *chdata = SMALL_FONT + (ch * 5);

    lcdCommand(0x80 | col);
    lcdCommand(0x40 | (row % LCD_ROW));
    // And send the column data
   for(uint8_t pixels = 0; pixels < 5; pixels++, chdata++) {
    uint8_t data = pgm_read_byte_near(chdata);
    lcdData(data);
    // for double sized font put
   // lcdData(data);
    };
  // Add the padding byte
// if(col < LCD_COL)
   lcdData(0x00);
  }


/* MAIN PROGRAM 
*/


// Watchdog generated interrupt in program
// used for polling ADC data and displaying LCD info
//
ISR(WDT_vect) {

	// while interrupt do ADC and send output to LCD
        int temp;
        uint8_t firstdig;
        uint8_t seconddig;
	uint8_t low, high;

        ADMUX = _BV(MUX0)|_BV(MUX1); // ADC3
        ADMUX &= ~_BV(REFS0); // explicit set VCC as reference voltage (3V = 3000mV)
 	// ADMUX |= _BV(REFS0); // 1.1 V reference voltage  by setting 1 to bit REFS0
	ADCSRA |= _BV(ADEN);  // Enable ADC
	ADCSRA |= _BV(ADSC);  // Run single conversion
	while(bit_is_set(ADCSRA, ADSC)); // Wait conversion is done

	// Read values from ADC
	low = ADCL;
        high = ADCH;

       // clear nokia LCD
	  lcdCommand(0x80);
	  lcdCommand(0x40);
	  // Fill in the whole display with ZEROS
	  for(uint16_t index = 0; index < (LCD_COL * LCD_ROW); index++)
 	   lcdData(0x00);
          
         // measure voltage from LM35 with VREF = 3000 mV
         	temp =  (  (  ((high << 8) | low) * 3000UL) >> 10);
         // measure with internal Vref 1.1V, but it is not reliable when powering down  
         //	temp =  ((((high << 8) | low) * 1100UL) >> 10);

         //  correction for LM36 - TMP36 readings -0,5V
	   if (temp > 499) {
			temp = temp - 500;
		}
            else 
                   {
                // display MINUS sign if temperature below zero
                        temp = 500 - temp;
                        lcdPrintChar(2, 30, 10);
		  }

            
                firstdig = temp / 100;
        //        seconddig = (temp - (firstdig * 100))/ 10;
                seconddig = temp %10;  

               lcdPrintChar(2, 40, firstdig);
               lcdPrintChar(2, 47, seconddig);
               
         // ADCSRA &= ~(1<<ADEN); // switchoff ADC
       
}



int main(void)
{

          DDRB &= ~_BV(PB3); // set data pin as ADC INPUT

          // LCD INITIALIZATION
 	/** Initialise the LCD
 	*
 	* Sets up the pins required to communicate with the LCD screen and then does
 	* the actual chipset initialisation. 
 	*/
 	 // Set up the output pins, ensuring they are all 'low' to start
 	 uint8_t val = (1 << LCD_SCK) | (1 << LCD_MOSI) | (1 << LCD_RESET) | (1 << LCD_CD);
 	 PORTB &= ~val;
 	 DDRB |= val;
 	 // Do a hard reset on the LCD
 	 //_delay_ms(10);
 	 PORTB |= (1 << LCD_RESET);

 	 // Initialise the LCD
 	 lcdCommand(0x21);  // LCD Extended Commands.
 	 lcdCommand(0xA1);  // Set LCD Vop (Contrast).
 	 lcdCommand(0x04);  // Set Temp coefficent.
 	 lcdCommand(0x14);  // LCD bias mode 1:48.
 	 lcdCommand(0x20);  // LCD Normal commands
 	 lcdCommand(0x0C);  // Normal display, horizontal addressing

          //set timer to 1 sec for measurements of LM35
           WDTCR |= (0<<WDP3) | (1<<WDP2) | (1<<WDP1) | (0<<WDP0);

          // set timer to 0.5s
          // WDTCR |= (1<<WDP2) | (1<<WDP0);

          // set timer to 4 sec
          // WDTCR |= (1<<WDP3);

          // Set watchdog timer in interrupt mode
          WDTCR |= (1<<WDTIE);
          WDTCR |= (0<<WDE);
          
          sei(); // Enable global interrupts

          set_sleep_mode(SLEEP_MODE_PWR_DOWN); 
    
for (;;) {
		sleep_mode();   // go to sleep and wait for interrupt...
	}
}

