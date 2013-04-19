#include <avr/io.h>


void reset_SSDs();
void spin_SSDs();
void update_ssd0(int ssd_val);
void update_ssd1(int ssd_val);
void update_LED(int status);
void update_relay(int status);
void init_SSDs();

void SSD0_shiftin(bool data);
void SSD1_shiftin(bool data);
