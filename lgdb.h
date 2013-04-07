#include <avr/eeprom.h>

#define DB_MY_ADDR 0
#define DB_BASESTATION_ADDR (void*)2
#define DB_MODE (void*)6

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
