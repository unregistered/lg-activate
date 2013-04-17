#include "lgserial.h"
#include <stdlib.h>
#include <stdio.h>
#include "util.h"

unsigned long LGSerial::last_command;

void LGSerial::init()
{
    LGSerial::last_command = 0;

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

    LGSerial::last_command = millis();
}

void LGSerial::put(char* str)
{
    for(int i = 0; str[i]; i++) {
        LGSerial::put(str[i]);
    }
}

void LGSerial::print(char* str)
{
    LGSerial::put(str);
    LGSerial::put("\r\n");
}

void LGSerial::print(int n)
{
    char str[16];
    sprintf(str, "%d", n);
    print(str);
}

void LGSerial::clear_screen()
{
    LGSerial::put("\033[A\033[2K\033[A\033[2K");
}

char LGSerial::get()
{
	while ( !LGSerial::available() ) { ; }
	return UDR0 ;
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

int LGSerial::get(char* buf, char stopchar, int maxlen)
{
    for(int i=0; i<maxlen; i++) {
        buf[i] = LGSerial::get();
        if(buf[i] == 13) return i + 1;
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
