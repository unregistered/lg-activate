#include <util/delay.h>

void sleep(int ms)
{
    for(int i=0; i < ms; i++) 
        _delay_ms(1);
}
