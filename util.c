#include "util.h"

void sleep(int ms)
{
    for(int i=0; i < ms; i++)
        _delay_ms(1);
}


volatile unsigned long timer1_millis;

unsigned long millis ()
{
    unsigned long millis_return;

    // Ensure this cannot be disrupted
    ATOMIC_BLOCK(ATOMIC_FORCEON) {
        millis_return = timer1_millis;
    }

    return millis_return;
}

ISR (TIMER1_COMPA_vect)
{
    timer1_millis++;
}

void init_timer()
{
   cli();

   #ifdef atmega168
   // CTC mode, Clock/8
   TCCR1B |= (1 << WGM12) | (1 << CS11);

   // Load the high byte, then the low byte
   // into the output compare
   OCR1AH = (CTC_MATCH_OVERFLOW >> 8);
   OCR1AL = CTC_MATCH_OVERFLOW;

   // Enable the compare match interrupt
   TIMSK1 |= (1 << OCIE1A);
   #endif

   #ifdef attiny4313
   TCCR1B |= (1 << WGM12) | (1 << CS11);

   OCR1AH = (CTC_MATCH_OVERFLOW >> 8);
   OCR1AL = CTC_MATCH_OVERFLOW;

   TIMSK |= (1 << OCIE1A);
   #endif

   // Now enable global interrupts
   sei();
}


uint8_t asciis_to_byte(char *ascii_chars)
{
   uint8_t retval = 0;

   int rawval = (int)ascii_chars[0];
   // 0-9
   if(rawval >= 48 && rawval <= 57) {
      retval = rawval - 48;
   }

   // A-F
   if(rawval >= 65 && rawval <= 70) {
      retval = rawval - 65 + 10;
   }

   retval = retval << 4; // Shift to MSB

   rawval = (int)ascii_chars[1];
      // 0-9
   if(rawval >= 48 && rawval <= 57) {
      retval += rawval - 48;
   }

   // A-F
   if(rawval >= 65 && rawval <= 70) {
      retval += rawval - 65 + 10;
   }

   return retval;
}

void byte_to_asciis(char *buf, uint8_t val)
{
   uint8_t msb = val >> 4;

   if(msb >=0 && msb <= 9) {
      *buf = (msb + 48);
   }

   if(msb >= 10 && msb <= 15) {
      *buf = (msb + 65 - 10);
   }

   // LSB
   uint8_t lsb = val & 0x0F;

   if(lsb >=0 && lsb <= 9) {
      *(buf + 1) = (lsb + 48);
   }

   if(lsb >= 10 && lsb <= 15) {
      *(buf + 1) = (lsb + 65 - 10);
   }


}

