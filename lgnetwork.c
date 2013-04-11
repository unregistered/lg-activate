#include "lgnetwork.h"
#include <string.h>
#include <stdio.h>
#include "lgdb.h"

static uuid_t myUUID;

#ifdef USE_NETWORK_SERVER
uint16_t LGNetwork::ap_table_cache[100];
#endif

#ifdef USE_CLIENT_SERVER
static uuid_t baseUUID;
#endif

LGNetwork::LGNetwork() : currentMode(LGNETWORK_INIT) {
    for(unsigned int i=0; i < sizeof(myUUID); i++) myUUID.bytes[i] = 48;

    #ifdef USE_NETWORK_SERVER
    for(unsigned int i=0; i < sizeof(ap_table_cache); i++) LGNetwork::ap_table_cache[i] = -1;
    #endif
}

void LGNetwork::set_mode(network_mode_t newMode)
{
    cmd_enter();

    cmd_restore_factory();
    cmd_set_network_id();

    cmd_set_channel(newMode);

    if(newMode == LGNETWORK_OPERATE) {
        #ifdef USE_NETWORK_SERVER
            cmd_set_short_address(-1);


        #elif USE_NETWORK_CLIENT
            uint16_t short_addr = LGDB::read_address();
            cmd_set_short_address(short_addr); // It will either be something we want, or -1


        #endif
    } else { // LGNETWORK_DISCOVER
        #ifdef USE_NETWORK_SERVER
            cmd_set_short_address(-1);
            cmd_set_target_short_address(-1);
            cmd_set_coordinator(true);

            // ATND node discovery
        #elif USE_NETWORK_CLIENT
            cmd_set_short_address(-1);
            cmd_set_target_short_address(-1);
            cmd_set_coordinator(false);
        #endif
    }

    cmd_exit();

    currentMode = newMode;
}

void LGNetwork::loop()
{
    if(currentMode == LGNETWORK_DISCOVER) {
        #ifdef USE_NETWORK_SERVER
            cmd_enter();
            uint64_t client_addr = cmd_get_one_unassociated_device();
            LGSerial::print("Found client:");
            LGSerial::print(client_addr);
            cmd_exit();
        #endif
    } else { // LGNETWORK_OPERATE

    }
}
// void LGNetwork::setup_server()
// {
//     xbee_setup(0, -1);

//     // Get AP entries
//     for(int i=0; i < sizeof(ap_table_cache); i++)
//     {
//         ap_table_cache[i] = LGDB::read_ap_table_entry(i);
//     }
// }

// void LGNetwork::setup_client(uint16_t id)
// {

// }

// void LGNetwork::setup_new_client()
// {
//     // Put ourself in searching mode
//     xbee_setup(0, -1);
// }

void LGNetwork::cmd_enter()
{
    // long time_to_wait = millis() - (LGSerial::last_command + 1100);
    // if(time_to_wait > 0)
    //     sleep(time_to_wait); // We need to wait longer to satisfy the guard time
    sleep(1100);
    LGSerial::put("+++");
    LGSerial::get(response_buf, 3); // Expect response 'OK\r'
}

void LGNetwork::cmd_exit()
{
    LGSerial::put("ATAC\r\n");
    LGSerial::get(response_buf, 3);
    LGSerial::put("ATCN\r\n");
    LGSerial::get(response_buf, 3);
}

void LGNetwork::cmd_restore_factory()
{
    LGSerial::put("ATRE\r\n"); // Restore to factory settings
    LGSerial::get(response_buf, 3); // OK\r
}

void LGNetwork::cmd_set_network_id()
{
    LGSerial::put("ATID" LG_ATID "\r\n"); // Set network ID to LG_ATID
    LGSerial::get(response_buf, 3); // OK\r
}

void LGNetwork::cmd_set_channel(network_mode_t mode)
{
    if(mode == LGNETWORK_DISCOVER)
        LGSerial::put("ATCH" DISCOVERY_CHANNEL "\r\n");
    else // LGNETWORK_OPERATE
        LGSerial::put("ATCH" OPERATING_CHANNEL "\r\n");

    LGSerial::get(response_buf, 3);
}

void LGNetwork::cmd_set_short_address(uint16_t addr)
{
    sprintf(response_buf, "ATMY%04X\r\n", addr);
    LGSerial::put(response_buf);
    LGSerial::get(response_buf, 3);
}

void LGNetwork::cmd_set_target_short_address(uint16_t addr)
{
    LGSerial::put("ATDH0000\r\n"); // Set high byte to 0
    LGSerial::get(response_buf, 3);

    sprintf(response_buf, "ATDL%04X\r\n", addr); // Set low byte to target
    LGSerial::put(response_buf);
    LGSerial::get(response_buf, 3);
}

void LGNetwork::cmd_set_target_long_address(uint64_t addr)
{
    sprintf(response_buf, "ATDH%04X\r\n", (addr >> 8)); // Set low byte to target
    LGSerial::put(response_buf);
    LGSerial::get(response_buf, 3);

    sprintf(response_buf, "ATDL%04X\r\n", addr); // Set low byte to target
    LGSerial::put(response_buf);
    LGSerial::get(response_buf, 3);
}

uint64_t LGNetwork::cmd_get_one_unassociated_device()
{
    LGSerial::put("ATND\r\n");
    LGSerial::get(response_buf, '\r', 16);
    LGSerial::put(response_buf);
    // LGSerial::get(response_buf, '\r', 16);
    // LGSerial::put(response_buf);
    // LGSerial::get(response_buf, '\r', 16);
    // LGSerial::put(response_buf);
    // LGSerial::get(response_buf, '\r', 16);
    // LGSerial::put(response_buf);
    // LGSerial::get(response_buf, '\r', 16);
    // LGSerial::put(response_buf);
    // LGSerial::get(response_buf, '\r', 16);
    // LGSerial::put(response_buf);


    return 0;
}

void LGNetwork::cmd_set_coordinator(bool isCoordinator)
{
    if(isCoordinator) {
        LGSerial::put("ATCE1\r\n");
        LGSerial::get(response_buf, 3);

        LGSerial::put("ATA2 01\r\n"); // Coordinator auto-associate
        LGSerial::get(response_buf, 3);
    } else {
        LGSerial::put("ATCE0\r\n");
        LGSerial::get(response_buf, 3);

        LGSerial::put("ATA1 01\r\n"); // Autoassociate
        LGSerial::get(response_buf, 3);
    }

}

void LGNetwork::xbee_setup(uint16_t my_addr, uint64_t destination_addr)
{
    LGNetwork::cmd_enter();
    // Guard time of > 1 second
    sleep(1100);
    LGSerial::put("+++"); // Request command mode

    // Expect response 'OK\r'
    LGSerial::get(response_buf, 3);

// #ifdef USE_NETWORK_SERVER
//     // Read the UUID (serial address)
//     LGSerial::put("ATSH\r\n"); // Starting with the upper bits
//     unsigned int high_bytes_to_write = LGSerial::get(response_buf, '\r', 16) - 1;
//     memcpy(myUUID.parts.high + (8 - high_bytes_to_write), response_buf, high_bytes_to_write);

//     // Now the lower bits
//     LGSerial::put("ATSL\r\n");
//     unsigned int low_bytes_to_write = LGSerial::get(response_buf, '\r', 16) - 1;
//     memcpy(myUUID.parts.low + (8 - low_bytes_to_write), response_buf, low_bytes_to_write);
// #endif

    // Now set some settings
    LGSerial::put("ATRE\r\n"); // Restore to factory settings
    LGSerial::get(response_buf, 3); // OK\r
    LGSerial::put("ATID" LG_ATID "\r\n"); // Set network ID to LG_ATID
    LGSerial::get(response_buf, 3); // OK\r

    // Set my address
    LGSerial::put("ATMY FFFF\r\n"); // Set my address
    LGSerial::get(response_buf, 3);

    // Set target address
    LGSerial::put("ATDH0\r\n");
    LGSerial::get(response_buf, 3);

    LGSerial::put("ATDLFFFF\r\n"); // Broadcast
    LGSerial::get(response_buf, 3);

    LGSerial::put("ATCN\r\n"); // Exit command mode
}
