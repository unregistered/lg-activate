#ifndef NETWORK_H
#define NETWORK_H

#include "lgserial.h"
#include "util.h"
#include <stdlib.h>

#define LG_ATID "3235"
#define OPERATING_CHANNEL 13
#define DISCOVERY_CHANNEL 15

typedef union {
    struct parts {
        char high[8];
        char low[8];
    } parts;
    char bytes[16];
    uint32_t value;
} uuid_t;

class LGNetwork
{
public:
    LGNetwork();

    uuid_t basestation_UUID;

    // Set up the xbee
    void setup_server();
    void setup_client(uint16_t id);
    void setup_new_client();

private:
    void xbee_setup(uint16_t, uint64_t);

private:
    char response_buf[16];
};

#endif
