#include "lgserial.h"
#include <stdlib.h>

void LGSerial::init()
{
    int ubrr = 63; // We've determined this, let's not change it

    UBRR0 = ubrr ; // Set baud rate
    UCSR0B |= (1 << TXEN0 ); // Turn on transmitter
    UCSR0B |= (1 << RXEN0 ); // Turn on receiver
    UCSR0C = (3 << UCSZ00 ); // Set for async . operation , no parity ,
                             // one stop bit , 8 data bits
}

void LGSerial::put(char ch)
{
	while (( UCSR0A & (1<< UDRE0 )) == 0);
	UDR0 = ch;
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
    str[0] = 0;
    itoa(n, str, 10);
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

int LGSerial::get(char* buf, int len)
{
    for(int i=0; i<len; i++) {
        buf[i] = LGSerial::get();
    }

    return len;
}

bool LGSerial::available()
{
    return UCSR0A & (1 << RXC0 );
}
