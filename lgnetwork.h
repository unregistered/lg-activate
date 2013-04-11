#ifndef NETWORK_H
#define NETWORK_H

#include "lgserial.h"
#include "util.h"
#include <stdlib.h>

#define LG_ATID "3235"
#define OPERATING_CHANNEL "0D"
#define DISCOVERY_CHANNEL "0F"

typedef enum {
    LGNETWORK_INIT,
    LGNETWORK_DISCOVER,
    LGNETWORK_OPERATE
} network_mode_t;

typedef union {
    struct parts {
        char high[8];
        char low[8];
    } parts;
    char bytes[16];
    uint64_t value;
    struct intparts {
        uint32_t high;
        uint32_t low;
    } intparts;
} uuid_t;

class LGNetwork
{
public:
    #ifdef USE_NETWORK_SERVER
    static uint16_t ap_table_cache[100];
    #endif
public:
    LGNetwork();

    network_mode_t currentMode;

    void set_mode(network_mode_t newMode);
    void loop(); // Call this periodically

private:
    void xbee_setup(uint16_t my_addr, uint64_t dest_addr);
    void cmd_enter();
    void cmd_exit();

    void cmd_restore_factory();
    void cmd_set_network_id();
    void cmd_set_channel(network_mode_t mode);

    void cmd_set_short_address(uint16_t addr);
    void cmd_set_target_short_address(uint16_t addr);
    void cmd_set_target_long_address(uint64_t addr);

    void cmd_set_coordinator(bool isCoordinator);

    uint64_t cmd_get_one_unassociated_device();

private:
    char response_buf[16];
};

#endif
