#include "lgnetwork.h"
#include <string.h>

LGNetwork::LGNetwork() {
    for(unsigned int i=0; i < sizeof(UUID); i++) UUID.bytes[i] = 0;
}

void LGNetwork::setup_server()
{
    xbee_setup(0, -1);
}

void LGNetwork::setup_client(uint16_t id)
{

}

void LGNetwork::setup_new_client()
{
    // Put ourself in searching mode
    xbee_setup(0, -1);
}

void LGNetwork::xbee_setup(uint16_t my_addr, uint64_t destination_addr)
{
    // Guard time of > 1 second
    sleep(1100);
    LGSerial::put("+++"); // Request command mode

    // Expect response 'OK\r'
    LGSerial::get(response_buf, 3);

    // Read the UUID (serial address)
    LGSerial::put("ATSH\r\n"); // Starting with the upper bits
    unsigned int high_bytes_to_write = LGSerial::get(response_buf, '\r', 16) - 1;
    memcpy(UUID.parts.high + (8 - high_bytes_to_write), response_buf, high_bytes_to_write);

    // Now the lower bits
    LGSerial::put("ATSL\r\n");
    unsigned int low_bytes_to_write = LGSerial::get(response_buf, '\r', 16) - 1;
    memcpy(UUID.parts.low + (8 - low_bytes_to_write), response_buf, low_bytes_to_write);

    char str[17];
    str[16] = 0;
    memcpy(str, UUID.bytes, 16);
    LGSerial::print(str);
    return;

    LGSerial::print("Response Buf");
    LGSerial::print(response_buf);
    return;

    LGSerial::put("ATRE\r\n"); // Restore to factory settings
    LGSerial::get(response_buf, 3); // OK\r
    LGSerial::put("ATID" LG_ATID "\r\n");
    LGSerial::get(response_buf, 3); // OK\r

    LGSerial::put("ATCN\r\n"); // Exit command mode
}
