#ifndef NETWORK_H
#define NETWORK_H

#include "lgserial.h"
#include "util.h"
#include <avr/pgmspace.h>

#define LG_ATID "3236"
#define OPERATING_CHANNEL "0D"
#define DISCOVERY_CHANNEL "0F"

#define GUARD_TIME "64" // 1000
#define SAFE_GUARD_TIME 1100

typedef enum {
    LGNETWORK_INIT,
    LGNETWORK_DISCOVER,
    LGNETWORK_OPERATE,
    LGNETWORK_DISCOVER_READY
} network_mode_t;

typedef union {
    struct packet {
        uint64_t base_address;
        uint8_t short_address;
    } packet;
    char bytes[9];
} dhcp_packet_t;

typedef union {
    struct packet {
        uint8_t short_address;
        uint8_t system_mode;
    } packet;
    char bytes[2];
} command_packet_t;

class LGNetwork
{
public:
    #ifdef USE_NETWORK_SERVER
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

    void force_disconnect(); // Call this before re-programming

public:
    void cmd_wait_for_ok();
    void cmd_enter();
    void cmd_reset();
    void cmd_exit();
    void cmd_persist();
    void cmd_dissociate();

    void cmd_setup();
    void cmd_set_channel(network_mode_t mode);

    void cmd_set_short_address(uint8_t);
    void cmd_set_target_short_address(uint8_t);
    void cmd_set_target_long_address(const uint64_t &);
    #ifdef USE_NETWORK_CLIENT
    void cmd_set_long_address_to_basestation();
    #endif

    void cmd_set_coordinator();

    void cmd_get_one_unassociated_device();

    /**
    * Searches for a header in incoming serial data
    * @return whether the header was found
    */
    bool scan_for_header(char* signature, unsigned int timeout);


#ifdef USE_NETWORK_SERVER
    bool pending_is_empty();
    void pending_clear();
    uint8_t get_next_free_address();

    // Returns -1 if no valid entries or
    // 0-99 for the next address to program
    int8_t get_next_target_address();
#endif

private:
    char response_buf[24];
};

extern LGNetwork network;

#endif
