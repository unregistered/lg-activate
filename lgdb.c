#include "lgdb.h"
#include "lgserial.h"

#ifdef USE_NETWORK_SERVER
uint16_t EEMEM lgdb_ap_table[100];
#endif

#ifdef USE_NETWORK_CLIENT
uint8_t EEMEM lgdb_my_addr;
uint64_t EEMEM lgdb_basestation_addr;
uint8_t EEMEM lgdb_mode;
#endif


LGDB::LGDB()
{

}

#ifdef USE_NETWORK_CLIENT

uint8_t LGDB::read_address()
{
    return read<uint8_t>((char*)&lgdb_my_addr);
}
void LGDB::write_address(uint8_t val)
{
    write<uint8_t>((char*)&lgdb_my_addr, val);
}

uint64_t LGDB::read_basestation_address()
{
    return read<uint64_t>((char*)&lgdb_basestation_addr);
}
void LGDB::write_basestation_address(uint64_t val)
{
    write<uint64_t>((char*)&lgdb_basestation_addr, val);
}

uint8_t LGDB::read_mode()
{
    return read<uint8_t>((char*)&lgdb_mode);
}
void LGDB::write_mode(uint8_t val)
{
    write<uint8_t>((char*)&lgdb_mode, val);
}

#endif

#ifdef USE_NETWORK_SERVER
uint16_t LGDB::read_ap_table_entry(uint8_t entry)
{
    char* addr = (char*)(lgdb_ap_table + entry);
    return read<uint16_t>(addr);
}
void LGDB::write_ap_table_entry(uint8_t entry, uint16_t val)
{
    char* addr = (char*)(lgdb_ap_table + entry);
    write<uint16_t>(addr, val);
}
#endif
