#include "lgdb.h"
#include "lgserial.h"

#ifdef USE_NETWORK_SERVER
uint8_t EEMEM lgdb_device_table[MAX_DEVICE_COUNT];
uint16_t EEMEM lgdb_schedule_table[MAX_DEVICE_COUNT*7];
uint8_t EEMEM lgdb_adapter_table[MAX_DEVICE_COUNT];
#endif

#ifdef USE_NETWORK_CLIENT
uint8_t EEMEM lgdb_my_addr;
uint64_t EEMEM lgdb_basestation_addr;
uint8_t EEMEM lgdb_mode;
#endif



#ifdef USE_NETWORK_CLIENT

uint8_t LGDB::read_address()
{
    return eeprom_read_byte(&lgdb_my_addr);
}
void LGDB::write_address(uint8_t val)
{
    eeprom_write_byte(&lgdb_my_addr, val);
}

uint64_t LGDB::read_basestation_address()
{
    uint64_t data;
    eeprom_read_block(&data, &lgdb_basestation_addr, sizeof(uint64_t));
    return data;
}
void LGDB::write_basestation_address(uint64_t val)
{
    eeprom_write_block(&val, &lgdb_basestation_addr, sizeof(uint64_t));
}

uint8_t LGDB::read_mode()
{
    return eeprom_read_byte(&lgdb_mode);
}
void LGDB::write_mode(uint8_t val)
{
    eeprom_write_byte(&lgdb_mode, val);
}

#endif

#ifdef USE_NETWORK_SERVER
uint16_t LGDB::read_schedule_table_entry(uint8_t entry, uint8_t day_of_week)
{
    return eeprom_read_word(lgdb_schedule_table + (entry*7 + day_of_week));
}
void LGDB::write_schedule_table_entry(uint8_t entry, uint8_t day_of_week, uint16_t val)
{
    eeprom_write_word(lgdb_schedule_table + (entry*7 + day_of_week), val);
}

uint8_t LGDB::read_device_table_entry(uint8_t entry)
{
    return eeprom_read_byte(lgdb_device_table + entry);
}
void LGDB::write_device_table_entry(uint8_t entry, uint8_t val)
{
    eeprom_write_byte(lgdb_device_table + entry, val);
}

uint8_t LGDB::read_adapter_table_entry(uint8_t entry)
{
    return eeprom_read_byte(lgdb_adapter_table + entry);
}
void LGDB::write_adapter_table_entry(uint8_t entry, uint8_t val)
{
    eeprom_write_byte(lgdb_adapter_table + entry, val);
}

uint8_t LGDB::read_hour(uint8_t device, uint8_t day, bool on_off_b)
{
    uint16_t data = LGDB::read_schedule_table_entry(device, day);
    if(on_off_b)
        data = data >> 8;

    return (data & 0x007F) >> 2;
}
uint8_t LGDB::read_minute(uint8_t device, uint8_t day, bool on_off_b)
{
    uint16_t data = LGDB::read_schedule_table_entry(device, day);
    if(on_off_b)
        data = data >> 8;

    return 15*(data & 0x0003);
}
void LGDB::incr_time(uint8_t device, uint8_t day, bool on_off_b)
{
    uint16_t data = LGDB::read_schedule_table_entry(device, day);
    uint8_t hour = data;
    if(on_off_b)
        hour = data >> 8;

    hour &= 0x7F;
    if(++hour == 96)
        hour = 0;

    if(on_off_b){
        LGSerial::put("Before:");
        LGSerial::print(data);
        data &= 0xC0FF;
        LGSerial::put("After");
        LGSerial::print(data);
        uint16_t mask = hour;
        mask = mask << 8;
        data |= mask;
    } else {
        data &= 0xFFC0;
        data += hour;
    }

    LGDB::write_schedule_table_entry(device, day, data);
}
void LGDB::decr_time(uint8_t device, uint8_t day, bool on_off_b)
{
    uint16_t data = LGDB::read_schedule_table_entry(device, day);
    int8_t hour = data;
    if(on_off_b)
        hour = data >> 8;

    hour &= 0x7F;
    if(--hour < 0)
        hour = 96;

    if(on_off_b){
        data &= 0xC0FF;
        data |= (uint16_t)(hour << 8);
    } else {
        data &= 0xFFC0;
        data += hour;
    }

    LGDB::write_schedule_table_entry(device, day, data);
}

#endif
