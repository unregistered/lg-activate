#include <avr/io.h>

void reset_SSDs();
void spin_SSDs();
void update_ssd0(uint8_t ssd_val);
void update_ssd1(uint8_t ssd_val);
void update_LED(uint8_t status);
void update_relay(uint8_t status);
void init_SSDs();

void SSD0_shiftin(bool data);
void SSD1_shiftin(bool data);

