#include <avr/eeprom.h>

#define DB_MY_ADDR 0 // 2 bytes
#define DB_BASESTATION_ADDR (void*)2 // 4 bytes
#define DB_MODE (void*)6 // 1 byte

/* Address Tables for basestation */
#define DB_AP_TABLE_START (void*)64 // 100 * 2 = 200 bytes

class LGDB
{
public:
    LGDB();

    static uint16_t read_address();
    static void write_address(uint16_t);

    static uint64_t read_basestation_address();
    static void write_basestation_address(uint64_t);

    static uint8_t read_mode();
    static void write_mode(uint8_t);

#ifdef USE_NETWORK_SERVER
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
