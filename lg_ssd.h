#include <avr/io.h>

void Display0_SSD0();
void Display0_SSD1();
void Display1_SSD0();
void Display1_SSD1();
void Display2_SSD0();
void Display2_SSD1();
void Display3_SSD0();
void Display3_SSD1();
void Display4_SSD0();
void Display4_SSD1();
void Display5_SSD0();
void Display5_SSD1();
void Display6_SSD0();
void Display6_SSD1();
void Display7_SSD0();
void Display7_SSD1();
void Display8_SSD0();
void Display8_SSD1();
void Display9_SSD0();
void Display9_SSD1();
void reset_SSDs();
void spin_SSDs(int times);
void update_ssd0(int ssd_val);
void update_ssd1(int ssd_val);
void update_LED(int status);
void update_relay(int status);
void init_SSDs();
