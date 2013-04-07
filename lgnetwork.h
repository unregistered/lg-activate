#include "lgserial.h"

#define ATID "3235"

class LGNetwork
{
public:
    LGNetwork();
    ~LGNetwork();

    // Set up the xbee
    void setup_server();
    void setup_client(unsigned int id);
    void setup_new_client();

private:
    void xbee_setup();

private:
    char response_buf[16];
};
