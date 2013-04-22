#include "lgnetwork.h"
#include "lgdb.h"
#include <string.h>

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

    // Populate table
    for(int i=0; i < sizeof(LGNetwork::ap_table_cache); i++) {
        LGNetwork::ap_table_cache[i] = LGDB::read_ap_table_entry(i);
    }
    #endif

    #ifdef USE_NETWORK_CLIENT
    LGNetwork::myShortAddr = LGDB::read_address();
    LGNetwork::baseUUID = LGDB::read_basestation_address();
    #endif
}

void LGNetwork::set_mode(network_mode_t newMode)
{
    cmd_enter();
    cmd_setup();
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
            char ascii_id[17];
            for(int i=0; i < sizeof(ascii_id); i++) ascii_id[i] = '0';
            ascii_id[16] = 0;

            LGSerial::put("ATSH\r\n"); // Starting with the upper bits
            unsigned int high_chars_to_write = LGSerial::get(response_buf, '\r', 16) - 1;
            memcpy(ascii_id + (8 - high_chars_to_write), response_buf, high_chars_to_write);

            // Now the lower bits
            LGSerial::put("ATSL\r\n");
            unsigned int low_chars_to_write = LGSerial::get(response_buf, '\r', 16) - 1;
            memcpy(ascii_id + 8 + (8 - low_chars_to_write), response_buf, low_chars_to_write);
            LGSerial::put("Ascii_id: ");
            LGSerial::put(ascii_id);
            LGSerial::put("\r\n");

            while(LGSerial::available()) {
                LGSerial::get();
            }

            uint8_t *p = (uint8_t*)&(LGNetwork::myUUID); // Ptr to current byte

            for(int i=0; i < 16; i+=2) {
                // Convert to bin
                uint8_t byte = asciis_to_byte(response_buf + i);
                *(p++) = byte;
            }


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
                LGSerial::print("Found client:");
                LGSerial::put("Address: "); LGSerial::print_hex(next_client.address);

                // We can operate on the pending device, by assigning it an address
                int8_t next_address = (int8_t)get_next_free_address();
                if(next_address >= 0 && next_address < 100) {
                    // We found a valid address

                    // Send it the association packet
                    dhcp_packet_t p;
                    p.packet.base_address = LGNetwork::myUUID;
                    p.packet.short_address = next_address;

                    // Point ourselves to our target.
                    // cmd_enter();
                    // cmd_set_target_long_address(next_client.address);
                    // cmd_exit();

                    // Packet header
                    LGSerial::put("DATA-DHCP");

                    // Send the packet body
                    for(int i=0; i < sizeof(dhcp_packet_t); i++) {
                        LGSerial::put(p.bytes[i]);
                    }

                    char response = 0;
                    unsigned long start_time = millis();
                    while(!LGSerial::available()) {
                        // Wait for a response
                        if(LGSerial::available()) {
                            response = LGSerial::get();
                        }

                        if(millis() > (start_time + 3*SAFE_GUARD_TIME)) {
                            LGSerial::print("Give up, no response.");
                            break; // Give up
                        }
                    }

                    if(response) {
                        // We clear it since we're done
                        pending_clear();

                        // Now commit it
                        LGNetwork::ap_table_cache[next_address] = (uint8_t)next_client.identifier;
                        // Persist to EEPROM
                    }

                }

            }

        #endif

        #if USE_NETWORK_CLIENT

            if(LGSerial::available()) {
                bool matches = scan_for_header("DATA-DHCP", 100);

                if(matches) {
                    // Receive packet
                    dhcp_packet_t p;
                    for(int i=0; i < sizeof(dhcp_packet_t); i++) {
                        p.bytes[i] = LGSerial::get();
                        if(p.bytes[i] == 0 && i == 0) {
                            return;
                        }
                    }

                    // Set my vars
                    LGNetwork::baseUUID = p.packet.base_address;
                    LGNetwork::myShortAddr = p.packet.short_address;

                    // Persist to eeprom
                    LGDB::write_address(LGNetwork::myShortAddr);
                    LGDB::write_basestation_address(LGNetwork::baseUUID);

                    // Reply to the server
                    cmd_enter();
                    cmd_set_target_long_address(LGNetwork::baseUUID);
                    cmd_set_short_address(LGNetwork::myShortAddr);
                    cmd_exit();
                    LGSerial::put('1');
                }
            }
        #endif
    } else { // LGNETWORK_OPERATE

    }

}



void LGNetwork::cmd_enter()
{
    if(currentMode == LGNETWORK_INIT)
        sleep(1100); // The uninitialized module has a 1s wait time
    else
        sleep(SAFE_GUARD_TIME);

    LGSerial::put("+++");
    LGSerial::get(response_buf, 3); // Expect response 'OK\r'
}

void LGNetwork::cmd_exit()
{
    LGSerial::put("ATAC\r\n"); // Apply changes
    LGSerial::get(response_buf, 3);
    // LGSerial::put("ATWR\r\n"); // Write to memory
    // LGSerial::get(response_buf, 3);
    LGSerial::put("ATCN\r\n");
    LGSerial::get(response_buf, 3);
}

void LGNetwork::cmd_setup()
{
    LGSerial::put("ATRE\r\n"); // Restore to factory settings
    LGSerial::get(response_buf, 3); // OK\r
    LGSerial::put("ATID" LG_ATID "\r\n"); // Set network ID to LG_ATID
    LGSerial::get(response_buf, 3); // OK\r
    // LGSerial::put("ATGT" GUARD_TIME "\r\n"); // Set guard time
    // LGSerial::get(response_buf, 3);
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
    char sendbuf[] = "ATMY 0000\r\n";
    if(addr == 0xff) {
        sendbuf[5] = 'F';
        sendbuf[6] = 'F';
        sendbuf[7] = 'F';
        sendbuf[8] = 'F';
    } else {
        byte_to_asciis(sendbuf + 7, addr);
    }
    LGSerial::put(sendbuf);
    LGSerial::get(response_buf, 3);
}

void LGNetwork::cmd_set_target_short_address(uint8_t addr)
{
    LGSerial::put("ATDH 0000\r\n"); // Set high byte to 0
    LGSerial::get(response_buf, 3);

    char sendbuf[] = "ATDL 0000\r\n";
    if(addr == 0xff) {
        sendbuf[5] = 'F';
        sendbuf[6] = 'F';
        sendbuf[7] = 'F';
        sendbuf[8] = 'F';
    } else {
        byte_to_asciis(sendbuf + 7, addr);
    }
    LGSerial::put(sendbuf);
    LGSerial::get(response_buf, 3);
}

void LGNetwork::cmd_set_target_long_address(uint64_t addr)
{
    char sendbuf[] = "ATDH 0000\r\n";
    byte_to_asciis(sendbuf + 7, addr >> 8);
    LGSerial::put(sendbuf);
    LGSerial::get(response_buf, 3);

    sendbuf[2] = 'D';
    sendbuf[3] = 'L';
    byte_to_asciis(sendbuf + 7, addr);
    LGSerial::put(sendbuf);
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

bool LGNetwork::scan_for_header(char* signature, unsigned int timeout)
{
    unsigned long start_time = millis();

    char *templ = signature;
    while(true) {
        if( (start_time + timeout) > millis() ) {
            return false;
        }

        if(*templ == 0) {
            // Null char
            LGSerial::put('m');
            return true;
        }

        if(LGSerial::available()) {
            // We can get data
            char c = LGSerial::get();
            LGSerial::put(c);
            if(c == *templ) {
                LGSerial::put('o');
                templ++;
            } else {
                LGSerial::put('b');
                templ = signature;
            }
        }
    }

}
