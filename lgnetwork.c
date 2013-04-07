#include "lgnetwork.h"
#include "util.h"

LGNetwork::LGNetwork() {
    return;
}

LGNetwork::~LGNetwork() {
    return;
}

void LGNetwork::setup_server()
{
    xbee_setup();
}

void LGNetwork::setup_client(unsigned int id)
{

}

void LGNetwork::setup_new_client()
{

}

void LGNetwork::xbee_setup()
{
    // Guard time of > 3 seconds
    sleep(3000);
    LGSerial::put("+++"); // Request command mode

    // Expect response 'OK\r'
    LGSerial::get(response_buf, 3);

    LGSerial::put("ATRE\r\n"); // Restore to factory settings
    LGSerial::get(response_buf, 3); // OK\r
    LGSerial::put("ATID" ATID "\r\n");
    LGSerial::get(response_buf, 3); // OK\r
    LGSerial::put("ATCN\r\n"); // Exit command mode
}
