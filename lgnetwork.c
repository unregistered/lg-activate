#include "lgnetwork.h"
#include "lgdb.h"
#include <string.h>

#ifdef USE_NETWORK_SERVER
uint64_t LGNetwork::myUUID;

uint8_t last_commanded_device_address;

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

LGNetwork::LGNetwork() : currentMode(LGNETWORK_INIT) {
    #ifdef USE_NETWORK_SERVER
    next_client.identifier = 0;

    #endif

    #ifdef USE_NETWORK_CLIENT
    LGNetwork::myShortAddr = LGDB::read_address();
    LGNetwork::baseUUID = LGDB::read_basestation_address();
    #endif
}

void LGNetwork::set_mode(network_mode_t newMode)
{
    cmd_enter();
    cmd_reset();
    cmd_setup();
    cmd_set_channel(newMode);

    if(newMode == LGNETWORK_OPERATE) {
        #ifdef USE_NETWORK_SERVER
            cmd_set_short_address(-1);
            cmd_set_target_short_address(-1);
            cmd_set_coordinator();

        #elif USE_NETWORK_CLIENT
            uint16_t short_addr = LGDB::read_address();
            cmd_set_short_address(short_addr); // It will either be something we want, or -1
            cmd_set_coordinator();

        #endif
    } else { // LGNETWORK_DISCOVER
        #ifdef USE_NETWORK_SERVER
            cmd_set_short_address(-1);
            cmd_set_target_short_address(-1);
            cmd_set_coordinator();

            // Get my uuid
            // Read the UUID (serial address)
            char ascii_id[17];
            for(int i=0; i < sizeof(ascii_id); i++) ascii_id[i] = '0';
            ascii_id[16] = 0;

            LGSerial::print_pgm( PSTR("ATSH") ); // Starting with the upper bits
            uint8_t high_chars_to_write = LGSerial::get(response_buf, '\r', 16) - 1;
            memcpy(ascii_id + (8 - high_chars_to_write), response_buf, high_chars_to_write);

            // Now the lower bits
            LGSerial::print_pgm( PSTR("ATSL") );
            uint8_t low_chars_to_write = LGSerial::get(response_buf, '\r', 16) - 1;
            memcpy(ascii_id + 8 + (8 - low_chars_to_write), response_buf, low_chars_to_write);

            uint8_t *p = (uint8_t*)&(LGNetwork::myUUID); // Ptr to current byte

            for(int i=0; i < 16; i+=2) {
                // Convert to bin
                *(p++) = asciis_to_byte(ascii_id + i);
            }


        #elif USE_NETWORK_CLIENT
            cmd_set_short_address(-1);
            cmd_set_target_short_address(-1);
            cmd_set_coordinator();

            #ifdef CLIENT_SENSOR
                LGSerial::print("ATNI s");
                LGSerial::get(response_buf, 3);
            #elif CLIENT_ACTUATOR
                LGSerial::print("ATNI a");
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

                    sleep(3000);

                    // Packet header
                    LGSerial::slow_put("SYN");

                    // Send the packet body
                    for(int i=0; i < sizeof(dhcp_packet_t); i++) {
                        LGSerial::slow_put(p.bytes[i]);
                    }

                    bool response = scan_for_header("SAK", 10000);

                    if(response) {
                        // Now commit it to EEPROM
                        LGDB::write_ap_table_entry(next_address, 0xe3);

                        // Send final ack
                        sleep(1000);
                        LGSerial::slow_put("ACK");
                    }

                    // We clear it since we're done
                    pending_clear();


                }

            }

        #endif

        #if USE_NETWORK_CLIENT

            if(LGSerial::available()) {
                bool matches = scan_for_header("SYN", 1000);

                if(matches) {
                    // Receive packet
                    char *ptr = (char*)&(LGNetwork::baseUUID);
                    for(uint8_t i=0; i < sizeof(LGNetwork::baseUUID); i++) {
                        char c = LGSerial::get();;
                        *(ptr++) = c;
                    }
                    LGNetwork::myShortAddr = LGSerial::get();

                    // Reply to the server
                    cmd_enter();
                    cmd_set_long_address_to_basestation();
                    cmd_set_short_address(LGNetwork::myShortAddr);
                    LGSerial::print_pgm( PSTR("ATDA") ); // Force dissassociation to re-associate
                    LGSerial::get(response_buf, 3);
                    cmd_exit();

                    // Give it time to associate
                    sleep(3000);

                    // SYN-ACK
                    LGSerial::slow_put_pgm( PSTR("SAK") );

                    // Hopefully the server saw us
                    matches = scan_for_header("ACK", 3000);
                    if(matches) {
                        // Persist to eeprom
                        LGDB::write_address(LGNetwork::myShortAddr);
                        LGDB::write_basestation_address(LGNetwork::baseUUID);

                        currentMode = LGNETWORK_DISCOVER_READY;

                    } else {
                        cmd_enter();
                        cmd_set_short_address(-1);
                        cmd_exit();
                    }

                }
            }
        #endif
    } else { // LGNETWORK_OPERATE
        #ifdef USE_NETWORK_SERVER
            int8_t next_to_program = get_next_target_address();
            if(next_to_program >= 0) {
                uint16_t data = LGDB::read_ap_table_entry(next_to_program);

                command_packet_t p;
                p.packet.short_address = next_to_program;
                p.packet.system_mode = 0;

                // Header
                LGSerial::slow_put_pgm( PSTR("CMD") );

                // Body
                for(uint8_t i=0; i < sizeof(command_packet_t); i++) {
                    LGSerial::slow_put(p.bytes[i]);
                }

                // Remember our success
                last_commanded_device_address = next_to_program;
            }
        #endif

    }

}



void LGNetwork::cmd_enter()
{
    if(currentMode == LGNETWORK_INIT)
        sleep(1100); // The uninitialized module has a 1s wait time
    else
        sleep(SAFE_GUARD_TIME);

    // Clear serial buffer
    while(LGSerial::available()) LGSerial::get();

    LGSerial::put_pgm( PSTR("+++") );
    LGSerial::get(response_buf, 3); // Expect response 'OK\r'
}

void LGNetwork::cmd_exit()
{
    LGSerial::print_pgm( PSTR("ATAC") ); // Apply changes
    LGSerial::get(response_buf, 3);
    LGSerial::print_pgm( PSTR("ATCN") );
    LGSerial::get(response_buf, '\r', 16, 500);
}

void LGNetwork::cmd_persist()
{
    LGSerial::print_pgm( PSTR("ATWR") ); // Write to memory
    LGSerial::get(response_buf, 3);
}

void LGNetwork::cmd_reset()
{
    LGSerial::print_pgm( PSTR("ATRE") ); // Restore to factory settings
    LGSerial::get(response_buf, 3); // OK\r
}

void LGNetwork::cmd_setup()
{
    LGSerial::print_pgm( PSTR("ATID" LG_ATID) ); // Set network ID to LG_ATID
    LGSerial::get(response_buf, 3); // OK\r
    // LGSerial::put("ATGT" GUARD_TIME "\r\n"); // Set guard time
    // LGSerial::get(response_buf, 3);
}

void LGNetwork::cmd_set_channel(network_mode_t mode)
{
    if(mode == LGNETWORK_DISCOVER)
        LGSerial::print_pgm( PSTR("ATCH" DISCOVERY_CHANNEL) );
    else // LGNETWORK_OPERATE
        LGSerial::print_pgm( PSTR("ATCH" OPERATING_CHANNEL) );

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
    LGSerial::print_pgm( PSTR("ATDH 0000") ); // Set high byte to 0
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

void LGNetwork::cmd_set_target_long_address(const uint64_t &addr)
{
    char sendbuf[] = "ATDH00000000\r\n";
    char* data = (char*)&(addr);
    for(uint8_t i=0; i < 8; i+=2) {
        byte_to_asciis(sendbuf + 4 + i, *data);
        data++;
    }
    LGSerial::put(sendbuf);
    LGSerial::get(response_buf, 3);

    sendbuf[2] = 'D';
    sendbuf[3] = 'L';
    for(uint8_t i=0; i < 8; i+=2) {
        byte_to_asciis(sendbuf + 4 + i, *data);
        data++;
    }
    LGSerial::put(sendbuf);
    LGSerial::get(response_buf, 3);
}

#ifdef USE_NETWORK_CLIENT
void LGNetwork::cmd_set_long_address_to_basestation()
{
    char sendbuf[] = "ATDH00000000\r\n";
    char* data = (char*)&(LGNetwork::baseUUID);
    for(uint8_t i=0; i < 8; i+=2) {
        byte_to_asciis(sendbuf + 4 + i, *data);
        data++;
    }
    LGSerial::put(sendbuf);
    LGSerial::get(response_buf, 3);

    // sendbuf[2] = 'D';
    // sendbuf[3] = 'L';
    // for(uint8_t i=0; i < 8; i+=2) {
    //     byte_to_asciis(sendbuf + 4 + i, *data);
    //     data++;
    // }
    // LGSerial::put(sendbuf);
    // LGSerial::get(response_buf, 3);
}
#endif

void LGNetwork::cmd_get_one_unassociated_device()
{
    #ifdef USE_NETWORK_SERVER

    LGSerial::print_pgm( PSTR("ATND") );

    int len = LGSerial::get(response_buf, '\r', 16) - 1; // ATMY
    if(!len) {
        // Then there are no nodes discovered
        LGSerial::print("NO NODES");
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
    for(int i=0; i < sizeof(lgdb_ap_table); i++) {
        if(LGDB::read_ap_table_entry(i) == 0xffff) {
            // Then we're open
            return i;
        }
    }

    return -1;
}

int8_t LGNetwork::get_next_target_address()
{
    uint8_t start_searching_at = last_commanded_device_address + 1;
    if(start_searching_at > 99)
        start_searching_at = 0; // Wrap-around to 0

    for(
        uint8_t i=start_searching_at; // Start searching at the last addr + 1, with wraparound
        i != last_commanded_device_address; // Keep searching until we reach this address
        i = (i == 99) ? 0 : i+1// Increment but also wrap around
    ) {
        if(LGDB::read_ap_table_entry(i) != 0xffff) {
            return i;
        }
    }

    if(LGDB::read_ap_table_entry(last_commanded_device_address) == 0xffff) // We have no open entries
        return -1;
    else // It's something
        return last_commanded_device_address;
}

#endif

void LGNetwork::cmd_set_coordinator()
{
    #ifdef USE_NETWORK_SERVER
        LGSerial::print_pgm( PSTR("ATCE1") );
        LGSerial::get(response_buf, 3);

        LGSerial::print_pgm( PSTR("ATA204") ); // Coordinator auto-associate
        LGSerial::get(response_buf, 3);
    #endif

    #ifdef USE_NETWORK_CLIENT
        LGSerial::print_pgm( PSTR("ATCE0") );
        LGSerial::get(response_buf, 3);

        LGSerial::print_pgm( PSTR("ATA104") ); // Autoassociate
        LGSerial::get(response_buf, 3);
    #endif

}

bool LGNetwork::scan_for_header(char* signature, unsigned int timeout)
{
    volatile unsigned long start_time = millis();

    char *templ = signature;
    while(true) {
        unsigned long thetime = millis();
        if( (thetime - start_time) > timeout ) {
            return false;
        }

        if(*templ == '\0') {
            // Null char
            return true;
        }

        if(LGSerial::available()) {
            // We can get data
            char c = LGSerial::get();

            if(c == *templ) {
                templ++;
            } else {
                templ = signature;
            }
        }
    }

}
