#ifndef NETWORK_H
#define NETWORK_H

#include "lgserial.h"
#include "util.h"

#define LG_ATID "3236"
#define OPERATING_CHANNEL "0D"
#define DISCOVERY_CHANNEL "0F"

typedef enum {
    LGNETWORK_INIT,
    LGNETWORK_DISCOVER,
    LGNETWORK_OPERATE
} network_mode_t;

class LGNetwork
{
public:
    #ifdef USE_NETWORK_SERVER
    static int8_t ap_table_cache[100];
    static uint64_t myUUID;
    #endif

    #ifdef USE_NETWORK_CLIENT
    static uint8_t myShortAddr;
    static uint64_t baseUUID;
    #endif

public:
    LGNetwork();

    network_mode_t currentMode;

    void set_mode(network_mode_t newMode);
    void loop(); // Call this periodically

private:
    void cmd_enter();
    void cmd_exit();

    void cmd_restore_factory();
    void cmd_set_network_id();
    void cmd_set_channel(network_mode_t mode);

    void cmd_set_short_address(uint8_t);
    void cmd_set_target_short_address(uint8_t);
    void cmd_set_target_long_address(uint64_t);

    void cmd_set_coordinator(bool isCoordinator);

    void cmd_get_one_unassociated_device();


#ifdef USE_NETWORK_SERVER
    bool pending_is_empty();
    void pending_clear();
    uint8_t get_next_free_address();

#endif

private:
    char response_buf[16];
};

#endif
