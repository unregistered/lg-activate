#include "lgnetwork.h"
#include <string.h>
#include <stdio.h>
#include "lgdb.h"

#ifdef USE_NETWORK_SERVER
int8_t LGNetwork::ap_table_cache[100];
uint64_t LGNetwork::myUUID;

typedef struct {
    uint64_t address;
    char identifier;
} client_candidate_t;

static client_candidate_t next_client;

#endif

#ifdef USE_NETWORK_CLIENT
uint64_t LGNetwork::baseUUID;
uint8_t LGNetwork::myShortAddr;
#endif

typedef union {
    struct packet {
        uint64_t base_address;
        uint8_t short_address;
    } packet;
    char bytes[9];
} dhcp_packet_t;

LGNetwork::LGNetwork() : currentMode(LGNETWORK_INIT) {
    #ifdef USE_NETWORK_SERVER
    next_client.identifier = 0;
    #endif

    #ifdef USE_NETWORK_CLIENT
    LGNetwork::myShortAddr = -1;
    LGNetwork::baseUUID = 0;
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
            cmd_set_coordinator(true);

        #elif USE_NETWORK_CLIENT
            uint16_t short_addr = LGDB::read_address();
            cmd_set_short_address(short_addr); // It will either be something we want, or -1
            cmd_set_coordinator(false);

        #endif
    } else { // LGNETWORK_DISCOVER
        #ifdef USE_NETWORK_SERVER
            cmd_set_short_address(-1);
            cmd_set_target_short_address(-1);
            cmd_set_coordinator(true);

            // Get my uuid
            // Read the UUID (serial address)
            // LGSerial::put("ATSH\r\n"); // Starting with the upper bits
            // unsigned int high_bytes_to_write = LGSerial::get(response_buf, '\r', 16) - 1;
            // char *p = (LGNetwork::myUUID)
            // for(int i=0; i < high_bytes_to_write; i++) {
            //     // Convert to bin

            // }
            // memcpy(myUUID.parts.high + (8 - high_bytes_to_write), response_buf, high_bytes_to_write);

            // // Now the lower bits
            // LGSerial::put("ATSL\r\n");
            // unsigned int low_bytes_to_write = LGSerial::get(response_buf, '\r', 16) - 1;
            // memcpy(myUUID.parts.low + (8 - low_bytes_to_write), response_buf, low_bytes_to_write);

        #elif USE_NETWORK_CLIENT
            cmd_set_short_address(-1);
            cmd_set_target_short_address(-1);
            cmd_set_coordinator(false);

            #ifdef CLIENT_SENSOR
                LGSerial::put("ATNI s\r\n");
                LGSerial::get(response_buf, 3);
            #elif CLIENT_ACTUATOR
                LGSerial::put("ATNI a\r\n");
                LGSerial::get(response_buf, 3);
            #endif
        #endif
    }

    cmd_exit();

    currentMode = newMode;
}

void LGNetwork::loop()
{
    if(currentMode == LGNETWORK_DISCOVER) {
        #ifdef USE_NETWORK_SERVER

            if(pending_is_empty()){
                cmd_enter();

                cmd_get_one_unassociated_device();

                cmd_exit();
            } else {
                LGSerial::print("Found client.");

                // We can operate on the pending device, by assigning it an address
                int8_t next_address = (int8_t)get_next_free_address();
                if(next_address >= 0 && next_address < 100) {
                    // We found a valid address

                    // Send it the association packet
                    dhcp_packet_t p;
                    p.packet.base_address = LGNetwork::myUUID;
                    p.packet.short_address = next_address;

                    // Point ourselves to our target.
                    cmd_enter();
                    cmd_set_target_long_address(next_client.address);
                    cmd_exit();

                    // Send the packet body
                    for(int i=0; i < sizeof(dhcp_packet_t); i++) {
                        LGSerial::put(p.bytes[i]);
                    }

                    // We clear it since we're done
                    pending_clear();

                    // Now commit it
                    LGNetwork::ap_table_cache[next_address] = (uint8_t)next_client.identifier;
                    // Persist to EEPROM
                }

            }

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
    LGSerial::put("ATWR\r\n");
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

void LGNetwork::cmd_set_short_address(uint8_t addr)
{
    sprintf(response_buf, "ATMY %04X\r\n", addr);
    LGSerial::put(response_buf);
    LGSerial::get(response_buf, 3);
}

void LGNetwork::cmd_set_target_short_address(uint8_t addr)
{
    LGSerial::put("ATDH 0000\r\n"); // Set high byte to 0
    LGSerial::get(response_buf, 3);

    sprintf(response_buf, "ATDL %04X\r\n", addr); // Set low byte to target
    LGSerial::put(response_buf);
    LGSerial::get(response_buf, 3);
}

void LGNetwork::cmd_set_target_long_address(uint64_t addr)
{
    sprintf(response_buf, "ATDH %04X\r\n", (addr >> 8)); // Set low byte to target
    LGSerial::put(response_buf);
    LGSerial::get(response_buf, 3);

    sprintf(response_buf, "ATDL %04X\r\n", addr); // Set low byte to target
    LGSerial::put(response_buf);
    LGSerial::get(response_buf, 3);
}

void LGNetwork::cmd_get_one_unassociated_device()
{
    #ifdef USE_NETWORK_SERVER

    LGSerial::put("ATND\r\n");

    int len = 0;
    for(int i=0; i < sizeof(response_buf); i++) response_buf[i] = 0;
    int bytes_read = LGSerial::get(response_buf, '\r', 16) - 1; // ATMY My Addr
    if(!bytes_read) {
        // Then there are no nodes discovered
        pending_clear();
        return;
    }

    // Get address
    char ascii_address[16];
    for(int i=0; i < sizeof(ascii_address); i++)
        ascii_address[i] = '0';

    len = LGSerial::get(response_buf, '\r', 16) - 1; // ATSH Serial High, - 1 for \r
    memcpy(ascii_address + (8 - len), response_buf, len);
    len = LGSerial::get(response_buf, '\r', 16) - 1; // ATSL
    memcpy(ascii_address + 8 + (8 - len), response_buf, len);

    // Set the numerical value
    char *ptr = (char*)&(next_client.address);
    for(int i=0; i < sizeof(ascii_address); i+=2) {
        *(ptr++) = (uint8_t)asciis_to_byte(ascii_address + i);
    }

    LGSerial::get(response_buf, '\r', 16); // RSSI

    // We want to know the device type
    len = LGSerial::get(response_buf, '\r', 16) - 1; // NI
    next_client.identifier = response_buf[0];

    LGSerial::get(response_buf, 1); // \r

    // It'll be over when we get 3 \r chars in a row
    int consecutive_returns = 2; // We saw 2 so far..
    do {
        char next = LGSerial::get(); // Get next char
        if(next == '\r') {
            consecutive_returns += 1;
        } else {
            consecutive_returns = 0;
        }
    } while(consecutive_returns != 3);

    #endif
}

#ifdef USE_NETWORK_SERVER
bool LGNetwork::pending_is_empty()
{
    return (next_client.identifier == 0);
}

void LGNetwork::pending_clear()
{
    next_client.identifier = 0;
    next_client.address = 0;
}

uint8_t LGNetwork::get_next_free_address()
{
    for(int i=0; i < sizeof(LGNetwork::ap_table_cache); i++) {
        if(LGNetwork::ap_table_cache[i] == -1) {
            // Then we're open
            return i;
        }
    }

    return -1;
}

#endif

void LGNetwork::cmd_set_coordinator(bool isCoordinator)
{
    if(isCoordinator) {
        LGSerial::put("ATCE1\r\n");
        LGSerial::get(response_buf, 3);

        LGSerial::put("ATA2 04\r\n"); // Coordinator auto-associate
        LGSerial::get(response_buf, 3);
    } else {
        LGSerial::put("ATCE0\r\n");
        LGSerial::get(response_buf, 3);

        LGSerial::put("ATA1 04\r\n"); // Autoassociate
        LGSerial::get(response_buf, 3);
    }

}
