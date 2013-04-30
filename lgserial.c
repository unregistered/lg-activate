#include "lgserial.h"
#include <stdlib.h>
#include <stdio.h>
#include "util.h"

void LGSerial::init()
{
    int ubrr = 63; // We've determined this, let's not change it

    #ifdef atmega168
    UBRR0 = ubrr ; // Set baud rate
    UCSR0B |= (1 << TXEN0 ); // Turn on transmitter
    UCSR0B |= (1 << RXEN0 ); // Turn on receiver
    UCSR0C = (3 << UCSZ00 ); // Set for async . operation , no parity ,
                             // one stop bit , 8 data bits
    #endif

    #ifdef attiny4313
    // Set baud rate
    UBRRH = (unsigned char)(ubrr >> 8);
    UBRRL = (unsigned char) ubrr;

    // Turn on transmitter
    UCSRB |= (1 << TXEN);

    // Turn on receiver
    UCSRB |= (1 << RXEN);

    // Set for async, no parity, 1 stop bit, 8 data bits
    UCSRC = (3 << UCSZ0);

    #endif

}

void LGSerial::put(char ch)
{
    #ifdef atmega168
	while (( UCSR0A & (1<< UDRE0 )) == 0) ;
    UDR0 = ch;
    #endif

    #ifdef attiny4313
    while (( UCSRA & (1 << UDRIE)) == 0) ;
    UDR = ch;
    #endif
}

void LGSerial::slow_put(char c)
{
    LGSerial::put(c);
    sleep(25);
}

void LGSerial::slow_put(char* str)
{
    for(int i=0; str[i]; i++) {
        LGSerial::put(str[i]);
        sleep(25);
    }
}

void LGSerial::slow_put_pgm(PGM_P str)
{
    char c;
    while( (c = pgm_read_byte(str++)) != 0) {
        LGSerial::put(c);
        sleep(25);
    }
}


void LGSerial::put(char* str)
{
    for(int i = 0; str[i]; i++) {
        LGSerial::put(str[i]);
    }
}

void LGSerial::put_pgm(PGM_P str)
{
    char c;
    while( (c = pgm_read_byte(str++)) != 0) {
        LGSerial::put(c);
    }
}

void LGSerial::print(char* str)
{
    LGSerial::put(str);
    LGSerial::put("\r\n");
}

void LGSerial::print_pgm(PGM_P s)
{
    char c;
    while((c = pgm_read_byte(s++)) != 0) {
        LGSerial::put(c);
    }
    LGSerial::put("\r\n");
}

void LGSerial::print(int n)
{
    #ifdef atmega168
    char str[16];
    sprintf(str, "%d", n);
    print(str);
    #endif

    #ifdef attiny4313
    char* ptr = (char*)&n;
    char str[2];
    for(int i=sizeof(n) - 1; i >= 0; i--) {
        byte_to_asciis(str, *(ptr + i));
        LGSerial::put(str[0]);
        LGSerial::put(str[1]);
    }
    LGSerial::put("\r\n");
    #endif
}

void LGSerial::print_hex(uint64_t &num)
{
    char str[17];
    str[16] = 0;

    char *ptr = (char*)&(num);

    for(int i=0; i<sizeof(uint64_t); i++) {
        byte_to_asciis( (str + i*2), *(ptr + i));
    }

    print(str);
}

void LGSerial::clear_screen()
{
    LGSerial::put("\033[A\033[2K\033[A\033[2K");
}

char LGSerial::get()
{
	while ( !LGSerial::available() ) { ; }

    #ifdef atmega168
	return UDR0 ;
    #endif

    #ifdef attiny4313
    return UDR;
    #endif
}

bool LGSerial::get_with_timeout(char* c, unsigned int timeout_ms)
{
    unsigned long start = millis();
    while(!LGSerial::available()) {
        // Wait
        if(millis() > (start + timeout_ms))
            return false;
    }
    *c = LGSerial::get();
    return true;
}

int LGSerial::get(char* buf, int len)
{
    for(int i=0; i<len; i++) {
        buf[i] = LGSerial::get();
    }

    return len;
}

int LGSerial::get(char* buf, char stopchar, uint8_t maxlen, int timeout)
{
    unsigned long start_time;
    if(timeout > 0)
        start_time = millis();

    uint8_t charcount = 0;
    for(;;) {
        if(timeout > 0) {
            if( (millis() - start_time) > timeout) {
                return charcount;
            }
        }

        if(LGSerial::available()) {
            char c = LGSerial::get();
            buf[charcount++] = c;
            if(c == stopchar) return charcount;
        }
    }

    return maxlen;
}

bool LGSerial::available()
{
    #ifdef atmega168
    return UCSR0A & (1 << RXC0 );
    #endif

    #ifdef attiny4313
    return UCSRA & (1 << RXC );
    #endif
}

void LGSerial::clear()
{
    while(LGSerial::available()) LGSerial::get();
}
