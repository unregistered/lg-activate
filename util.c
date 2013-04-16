#include "util.h"

void sleep(int ms)
{
    for(int i=0; i < ms; i++)
        _delay_ms(1);
}

volatile unsigned long timer0_overflow_count = 0;
volatile unsigned long timer0_millis = 0;
static unsigned char timer0_fract = 0;


SIGNAL(TIMER0_OVF_vect)
{
   // copy these to local variables so they can be stored in registers
   // (volatile variables must be read from memory on every access)
   unsigned long m = timer0_millis;
   unsigned char f = timer0_fract;

   m += MILLIS_INC;
   f += FRACT_INC;
   if (f >= FRACT_MAX) {
      f -= FRACT_MAX;
      m += 1;
   }

   timer0_fract = f;
   timer0_millis = m;
   timer0_overflow_count++;
}


unsigned long millis()
{
   unsigned long m;
   uint8_t oldSREG = SREG;

   // disable interrupts while we read timer0_millis or we might get an
   // inconsistent value (e.g. in the middle of a write to timer0_millis)
   cli();
   m = timer0_millis;
   SREG = oldSREG;

   return m;
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
