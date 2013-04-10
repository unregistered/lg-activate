#include "lgdb.h"

LGDB::LGDB()
{

}

uint16_t LGDB::read_address()
{
    return read<uint16_t>(DB_MY_ADDR);
}
void LGDB::write_address(uint16_t val)
{
    write<uint16_t>(DB_MY_ADDR, val);
}

uint64_t LGDB::read_basestation_address()
{
    return read<uint64_t>(DB_BASESTATION_ADDR);
}
void LGDB::write_basestation_address(uint64_t val)
{
    write<uint64_t>(DB_BASESTATION_ADDR, val);
}

uint8_t LGDB::read_mode()
{
    return read<uint8_t>(DB_MODE);
}
void LGDB::write_mode(uint8_t val)
{
    write<uint8_t>(DB_MODE, val);
}

#ifdef USE_NETWORK_SERVER
uint16_t LGDB::read_ap_table_entry(uint8_t entry)
{
    char* addr = static_cast<char*>(DB_AP_TABLE_START) + entry;
    return read<uint16_t>(addr);
}
void LGDB::write_ap_table_entry(uint8_t entry, uint16_t val)
{
    char* addr = static_cast<char*>(DB_AP_TABLE_START) + entry;
    write<uint16_t>(addr, val);
}
#endif
