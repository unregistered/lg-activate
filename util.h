#ifndef UTIL_H
#define UTIL_H

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

// Sleep for a variable amount of time
void sleep(int ms);


uint8_t asciis_to_byte(char *ascii_char);
void byte_to_asciis(char *buf, uint8_t val);



#define CTC_MATCH_OVERFLOW ((F_CPU / 1000) / 8)
unsigned long millis ();
void init_timer();

#endif
