#include <avr/eeprom.h>

#ifdef USE_NETWORK_CLIENT
extern uint8_t EEMEM lgdb_my_addr;
extern uint64_t EEMEM lgdb_basestation_addr;
extern uint8_t EEMEM lgdb_mode;
#endif

/* Address Tables for basestation */
#ifdef USE_NETWORK_SERVER
extern uint16_t EEMEM lgdb_ap_table[100];
#endif

class LGDB
{
public:
    LGDB();

#ifdef USE_NETWORK_CLIENT
    static uint8_t read_address();
    static void write_address(uint8_t);

    static uint64_t read_basestation_address();
    static void write_basestation_address(uint64_t);

    static uint8_t read_mode();
    static void write_mode(uint8_t);
#endif

#ifdef USE_NETWORK_SERVER
    // 1 bit type - (0 actuator, 1 sensor)
    // 1 bit (see below)
    // For Actuators
    //      means is_auto (1 for auto, 0 for just on)
    // For Sensors
    //      must be 0, 1 is invalid
    // Note: a 11 in the first 2 bit positions means empty entry. So convenient
    // that it initializies to 0xff
    //
    // 7 bit On time (15 min increments)
    // 7 bit Off time (15 min increments)
    static uint16_t read_ap_table_entry(uint8_t entry);
    static void write_ap_table_entry(uint8_t entry, uint16_t val);
#endif

private:
    template<typename T>
    static T read(void *addr) {
        T data;
        eeprom_read_block(&data, addr, sizeof(T));
        return data;
    }

    template<typename T>
    static void write(void *addr, T val) {
        eeprom_write_block(&val, addr, sizeof(T));
    }
};
