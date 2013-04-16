#include "controller_hexstr.h"
#include "util.h"
#include "lgserial.h"

// Variable initialization can be done here
Controller::Controller()
{
}

// This runs once during program startup
void Controller::setup()
{
	// Setup code here
    LGSerial::init();

    LGSerial::print("00");
    LGSerial::print(asciis_to_byte("00"));

    LGSerial::print("0A");
    LGSerial::print(asciis_to_byte("0A"));

    LGSerial::print("F0");
    LGSerial::print(asciis_to_byte("F0"));

    LGSerial::print("FF");
    LGSerial::print(asciis_to_byte("FF"));

    char printbuf[3];
    printbuf[2] = 0;

    byte_to_asciis(printbuf, 255);
    LGSerial::print("255");
    LGSerial::print(printbuf);

    byte_to_asciis(printbuf, 165);
    LGSerial::print("165 is...");
    LGSerial::print(165 >> 4);
    LGSerial::print(printbuf);
}

// This runs continuously in a loop.
void Controller::loop()
{
    // Main code here
}
