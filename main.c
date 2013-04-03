/* Name: main.c
 * Author: Team 3 LG
 * Copyright: All Rights Reserved.
 */

#include <avr/io.h>
#include <util/delay.h>
#include "lgserial.h"

// Sleep for ms milliseconds
void sleep(int ms)
{
    for(int i=0; i < ms; i++) 
        _delay_ms(1);
}

int main(void)
{
	LGSerial::init(63); // (9830400/(16*9600))-1;
	
    // print_serial("Hello World!");
    // Guard time of > 1 second
    sleep(1200);
    LGSerial::put("+++"); // Request command mode
    sleep(1200);
    
    // Expect response 'OK\r'
    char buf[3];
    buf[0] = LGSerial::get();
    
    if(buf[0] == 'O') {
        // We have entered command mode
        // LGSerial::print("ATID");
        // // usart_in(buf, 3);
        // sleep(1200);
        for(;;) { LGSerial::put('k'); }
    }
    
    for(;;) { LGSerial::put('o'); }
    
    // LGSerial::print(buf);
    
    
	
    for(;;){
        /* insert your main loop code here */
        LGSerial::put('h');
        // char input = usart_in();
        // usart_out(input);
				  
    }

    return 0;   /* never reached */
}
