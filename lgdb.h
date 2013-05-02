#include <avr/eeprom.h>

#ifndef LGDB_H
#define LGDB_H

#ifdef USE_NETWORK_CLIENT
extern uint8_t EEMEM lgdb_my_addr;
extern uint64_t EEMEM lgdb_basestation_addr;
extern uint8_t EEMEM lgdb_mode;
#endif

/* Address Tables for basestation */
#define MAX_DEVICE_COUNT 23

#ifdef USE_NETWORK_SERVER
extern uint8_t EEMEM lgdb_device_table[MAX_DEVICE_COUNT];
extern uint16_t EEMEM lgdb_schedule_table[MAX_DEVICE_COUNT*7];
extern uint8_t EEMEM lgdb_sensor_table[MAX_DEVICE_COUNT*7];
#endif

class LGDB
{
public:

#ifdef USE_NETWORK_CLIENT
    static uint8_t read_address();
    static void write_address(uint8_t);

    static uint64_t read_basestation_address();
    static void write_basestation_address(uint64_t);

    static uint8_t read_mode();
    static void write_mode(uint8_t);
#endif

#ifdef USE_NETWORK_SERVER
    // 1 bit use_auto
    // 7 bit On time
    // {
    //   1 bit am/pm_b
    //   4 bit hour 0-12
    //   2 bit minute 0, 15, 30, 45
    // }
    // 1 bit padding
    // 7 bit off time
    static uint16_t read_schedule_table_entry(uint8_t device, uint8_t day_of_week);
    static void write_schedule_table_entry(uint8_t device, uint8_t day_of_week, uint16_t val);

    // sensor id for auto
    static uint8_t read_sensor_table_entry(uint8_t entry, uint8_t day);
    static void write_sensor_table_entry(uint8_t entry, uint8_t day, uint8_t val);

    // 1 bit, (0 actuator, 1 sensor)
    static uint8_t read_device_table_entry(uint8_t entry);
    static void write_device_table_entry(uint8_t entry, uint8_t val);

    static uint8_t read_hour(uint8_t device, uint8_t day, bool on_off_b);
    static uint8_t read_minute(uint8_t device, uint8_t day, bool on_off_b);
#endif


};

#endif
