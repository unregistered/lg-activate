#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <math.h>
//#include "adatouch.cpp"
#include "util.h"
//#include "lgserial.h"

void color_bars();
void drawHorizontalLine(uint16_t /*x*/, uint16_t/*y*/, uint16_t /*length*/, uint16_t /*color*/, uint16_t /*thickness*/);
void drawVerticalLine (uint16_t /*x*/, uint16_t /*y*/, uint16_t /*length*/, uint16_t /*color*/, uint16_t /*thickness*/);
void drawDiagonalLine (uint16_t/*x1*/, uint16_t /*y1*/, uint16_t /*x2*/, uint16_t /*y2*/, uint16_t /*color*/);
void drawPixel (uint16_t /*x1*/, uint16_t /*y1*/, uint16_t /*color*/);
void fillScreen(uint16_t /*color*/);
void fillRectangle(uint16_t /*x*/, uint16_t /*y*/, uint16_t /*h*/, uint16_t /*w*/, uint16_t /*color*/, uint16_t /*thick*/);
void makeRectangle(uint16_t /*x*/, uint16_t /*y*/, uint16_t /*h*/, uint16_t /*w*/, uint16_t /*color*/, uint16_t /*thick*/);
void drawChar(uint16_t /*x*/, uint16_t /*y*/, const char c/*h*/, uint16_t /*color*/, uint16_t /*bg*/, uint16_t /*size*/);
void drawString(uint16_t /*x*/, uint16_t /*y*/, char* /*str*/, uint16_t /*color*/, uint16_t /*bg*/, uint16_t /*size*/);
void drawPgmString(uint16_t /*x*/, uint16_t /*y*/, PGM_P /*str*/, uint16_t /*color*/, uint16_t /*bg*/, uint16_t /*size*/);
//void bw_image();
void regout(uint16_t, uint16_t);
void lcdout(uint8_t);
void adafruit_initialize(void);
void reset();
uint16_t color565(uint8_t, uint8_t, uint8_t);
void homeMenu();
void statusMenu();
void devicesMenu();
void settingsMenu();
void scheduleMenu();

// Define some bits in the I/O ports



#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define TEST 0x96a8
#define YELLOW 0xFFE0
#define WHITE 0xFFFF
#define GREY 0x8888

#define LCD_Data_B  0x07        // Bits in Port B for LCD data bits 0-1
#define LCD_Data_D  0xfc        // Bits in Port D for LCD data bits 3-7
#define LCD_Data_BitB0 0x01

#define LCD_CD      (1 << PD3)  // PORTB, bit 4 - LCD register select (0 = command (addr), 1 = data)
#define LCD_RD      (1 << PD5)  // PORTB, bit 3 - LCD /RD (read)
#define LCD_CE      (1 << PD2)  // PORTB, bit 2 - LCD /CE (chip enable)
#define LCD_WR      (1 << PD4)  // PORTB, bit 5 - LCD /WR (write)
#define LCD_RST     (1 << PD6)  // PORTB, bit 7 - LCD /RST (reset)
#define LCD_Bits_C  (LCD_CD|LCD_RD|LCD_CE|LCD_WR|LCD_RST)

#define LCD_CD_0     PORTD &= ~LCD_CD
#define LCD_CD_1     PORTD |= LCD_CD
#define LCD_RD_0     PORTD &= ~LCD_RD
#define LCD_RD_1     PORTD |= LCD_RD
#define LCD_CE_0     PORTD &= ~LCD_CE
#define LCD_CE_1     PORTD |= LCD_CE
#define LCD_WR_0     PORTD &= ~LCD_WR
#define LCD_WR_1     PORTD |= LCD_WR
#define LCD_RST_0    PORTD &= ~LCD_RST
#define LCD_RST_1    PORTD |= LCD_RST

// Define names for the registers

#define ILI932X_START_OSC       0x00
#define ILI932X_DRIV_OUT_CTRL       0x01
#define ILI932X_DRIV_WAV_CTRL       0x02
#define ILI932X_ENTRY_MOD       0x03
#define ILI932X_RESIZE_CTRL     0x04
#define ILI932X_DISP_CTRL1      0x07
#define ILI932X_DISP_CTRL2      0x08
#define ILI932X_DISP_CTRL3      0x09
#define ILI932X_DISP_CTRL4      0x0A
#define ILI932X_RGB_DISP_IF_CTRL1   0x0C
#define ILI932X_FRM_MARKER_POS      0x0D
#define ILI932X_RGB_DISP_IF_CTRL2   0x0F
#define ILI932X_POW_CTRL1       0x10
#define ILI932X_POW_CTRL2       0x11
#define ILI932X_POW_CTRL3       0x12
#define ILI932X_POW_CTRL4       0x13
#define ILI932X_GRAM_HOR_AD     0x20
#define ILI932X_GRAM_VER_AD     0x21
#define ILI932X_RW_GRAM         0x22
#define ILI932X_POW_CTRL7       0x29
#define ILI932X_FRM_RATE_COL_CTRL   0x2B
#define ILI932X_GAMMA_CTRL1     0x30
#define ILI932X_GAMMA_CTRL2     0x31
#define ILI932X_GAMMA_CTRL3     0x32
#define ILI932X_GAMMA_CTRL4     0x35
#define ILI932X_GAMMA_CTRL5     0x36
#define ILI932X_GAMMA_CTRL6     0x37
#define ILI932X_GAMMA_CTRL7     0x38
#define ILI932X_GAMMA_CTRL8     0x39
#define ILI932X_GAMMA_CTRL9     0x3C
#define ILI932X_GAMMA_CTRL10        0x3D
#define ILI932X_HOR_START_AD        0x50
#define ILI932X_HOR_END_AD      0x51
#define ILI932X_VER_START_AD        0x52
#define ILI932X_VER_END_AD      0x53
#define ILI932X_GATE_SCAN_CTRL1     0x60
#define ILI932X_GATE_SCAN_CTRL2     0x61
#define ILI932X_GATE_SCAN_CTRL3     0x6A
#define ILI932X_PART_IMG1_DISP_POS  0x80
#define ILI932X_PART_IMG1_START_AD  0x81
#define ILI932X_PART_IMG1_END_AD    0x82
#define ILI932X_PART_IMG2_DISP_POS  0x83
#define ILI932X_PART_IMG2_START_AD  0x84
#define ILI932X_PART_IMG2_END_AD    0x85
#define ILI932X_PANEL_IF_CTRL1      0x90
#define ILI932X_PANEL_IF_CTRL2      0x92
#define ILI932X_PANEL_IF_CTRL3      0x93
#define ILI932X_PANEL_IF_CTRL4      0x95
#define ILI932X_PANEL_IF_CTRL5      0x97
#define ILI932X_PANEL_IF_CTRL6      0x98

// Define special addresses for doing delays during startup

#define TFTLCD_DELAY_50         0xFF    // 50ms delay
#define TFTLCD_DELAY_200        0xFE    // 200ms delay
