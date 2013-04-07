#include "controller_db.h"

// Variable initialization can be done here
Controller::Controller()
{
}

// This runs once during program startup
void Controller::setup()
{
    // Setup code here
    LGSerial::init();

    LGSerial::print("DB Test");

    // Main code here
    int addr = LGDB::read_address();
    LGSerial::print(addr);

    LGDB::write_address(13);
    addr = LGDB::read_address();
    LGSerial::print(addr);

    int ba = LGDB::read_basestation_address();
    LGSerial::print("Basestation Address");
    LGSerial::print(ba);
    LGDB::write_basestation_address(32);
    ba = LGDB::read_basestation_address();
    LGSerial::print(ba);
}

// This runs continuously in a loop.
void Controller::loop()
{
}
