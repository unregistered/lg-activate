#include "controller_basestation.h"

Controller::Controller()
{
}

void Controller::setup()
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
}

void Controller::loop()
{
    LGSerial::put("Loop");
}