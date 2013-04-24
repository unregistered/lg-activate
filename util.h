#ifndef UTIL_H
#define UTIL_H

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

#define SYSTEM_ON 0
#define SYSTEM_OFF 1
#define SYSTEM_AUTO 2

extern uint16_t StackCount();

// Sleep for a variable amount of time
void sleep(int ms);


uint8_t asciis_to_byte(char *ascii_char);
void byte_to_asciis(char *buf, uint8_t val);

#define CTC_MATCH_OVERFLOW ((F_CPU / 1000) / 8)
unsigned long millis ();
void init_timer();

#endif
