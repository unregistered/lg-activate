
/*************************************************************
 *       adafruit335 - Demonstrate interface to a graphics LCD display
 *
 *       This program will print a message on an Adafruit 335
 *       graphics LCD (ILI9325 controller).  Most of it is based on the
 *       sample code provided by Adafruit in Adafruit_TFTLCD.cpp and
 *       associated files.
 *
 *       Port B, bit 4 (0x10) - output to C/D (cmd/data) input of display
 *               bit 3 (0x08) - output to /RD (read) input of display
 *               bit 2 (0x04) - output to /CS (chip select) input of display
 *               bit 5 (0x20) - output to /WR (write) input of display
 *               bit 7 (0x80) - output to /RST (reset) input of display
 *       Port B, bits 0-1, Port D, bits 2-7 - DB0-DB7 of display.
 *
 * Revision History
 * Date Author Description
 * 04/12/13 A. Weber    Initial release
 *************************************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h> 
#include <math.h> 

void color_bars();
void drawHorizontalLine(uint16_t /*x*/, uint16_t/*y*/, uint16_t /*length*/, uint16_t /*color*/, uint16_t /*thickness*/); 
void drawVerticalLine (uint16_t /*x*/, uint16_t /*y*/, uint16_t /*length*/, uint16_t /*color*/, uint16_t /*thickness*/); 
void drawDiagonalLine (uint16_t/*x1*/, uint16_t /*y1*/, uint16_t /*x2*/, uint16_t /*y2*/, uint16_t /*color*/); 
void drawPixel (uint16_t /*x1*/, uint16_t /*y1*/, uint16_t /*color*/); 
void fillScreen(uint16_t /*color*/);
void fillRectangle(uint16_t /*x*/, uint16_t /*y*/, uint16_t /*h*/, uint16_t /*w*/, uint16_t /*color*/ ); 
void makeRectangle(uint16_t /*x*/, uint16_t /*y*/, uint16_t /*h*/, uint16_t /*w*/, uint16_t /*color*/, uint16_t /*thick*/); 
void drawChar(uint16_t /*x*/, uint16_t /*y*/, unsigned char c/*h*/, uint16_t /*color*/, uint16_t /*bg*/, uint16_t /*size*/); 

//void bw_image();
void regout(uint16_t, uint16_t);
void lcdout(uint8_t);
void initialize(void);
void reset();
uint16_t color565(uint8_t, uint8_t, uint8_t);

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

#define ILI932X_START_OSC		0x00
#define ILI932X_DRIV_OUT_CTRL		0x01
#define ILI932X_DRIV_WAV_CTRL		0x02
#define ILI932X_ENTRY_MOD		0x03
#define ILI932X_RESIZE_CTRL		0x04
#define ILI932X_DISP_CTRL1		0x07
#define ILI932X_DISP_CTRL2		0x08
#define ILI932X_DISP_CTRL3		0x09
#define ILI932X_DISP_CTRL4		0x0A
#define ILI932X_RGB_DISP_IF_CTRL1	0x0C
#define ILI932X_FRM_MARKER_POS		0x0D
#define ILI932X_RGB_DISP_IF_CTRL2	0x0F
#define ILI932X_POW_CTRL1		0x10
#define ILI932X_POW_CTRL2		0x11
#define ILI932X_POW_CTRL3		0x12
#define ILI932X_POW_CTRL4		0x13
#define ILI932X_GRAM_HOR_AD		0x20
#define ILI932X_GRAM_VER_AD		0x21
#define ILI932X_RW_GRAM			0x22
#define ILI932X_POW_CTRL7		0x29
#define ILI932X_FRM_RATE_COL_CTRL	0x2B
#define ILI932X_GAMMA_CTRL1		0x30
#define ILI932X_GAMMA_CTRL2		0x31
#define ILI932X_GAMMA_CTRL3		0x32
#define ILI932X_GAMMA_CTRL4		0x35
#define ILI932X_GAMMA_CTRL5		0x36
#define ILI932X_GAMMA_CTRL6		0x37
#define ILI932X_GAMMA_CTRL7		0x38
#define ILI932X_GAMMA_CTRL8		0x39
#define ILI932X_GAMMA_CTRL9		0x3C
#define ILI932X_GAMMA_CTRL10		0x3D
#define ILI932X_HOR_START_AD		0x50
#define ILI932X_HOR_END_AD		0x51
#define ILI932X_VER_START_AD		0x52
#define ILI932X_VER_END_AD		0x53
#define ILI932X_GATE_SCAN_CTRL1		0x60
#define ILI932X_GATE_SCAN_CTRL2		0x61
#define ILI932X_GATE_SCAN_CTRL3		0x6A
#define ILI932X_PART_IMG1_DISP_POS	0x80
#define ILI932X_PART_IMG1_START_AD	0x81
#define ILI932X_PART_IMG1_END_AD	0x82
#define ILI932X_PART_IMG2_DISP_POS	0x83
#define ILI932X_PART_IMG2_START_AD	0x84
#define ILI932X_PART_IMG2_END_AD	0x85
#define ILI932X_PANEL_IF_CTRL1		0x90
#define ILI932X_PANEL_IF_CTRL2		0x92
#define ILI932X_PANEL_IF_CTRL3		0x93
#define ILI932X_PANEL_IF_CTRL4		0x95
#define ILI932X_PANEL_IF_CTRL5		0x97
#define ILI932X_PANEL_IF_CTRL6		0x98

// Define special addresses for doing delays during startup

#define TFTLCD_DELAY_50			0xFF    // 50ms delay
#define TFTLCD_DELAY_200		0xFE    // 200ms delay

// Set up an array of register addresses and initializaion data

const uint16_t ILI932x_regValues[] PROGMEM = {
    ILI932X_START_OSC,		0x0001,	// Start oscillator
    TFTLCD_DELAY_50,		0,	// 50 millisecond delay
    ILI932X_DRIV_OUT_CTRL,	0x0100,
    ILI932X_DRIV_WAV_CTRL,	0x0700,
    ILI932X_ENTRY_MOD,		0x1030,
    ILI932X_RESIZE_CTRL,	0x0000,
    ILI932X_DISP_CTRL2,		0x0202,
    ILI932X_DISP_CTRL3,		0x0000,
    ILI932X_DISP_CTRL4,		0x0000,
    ILI932X_RGB_DISP_IF_CTRL1,	0x0,
    ILI932X_FRM_MARKER_POS,	0x0,
    ILI932X_RGB_DISP_IF_CTRL2,	0x0,
    ILI932X_POW_CTRL1,		0x0000,
    ILI932X_POW_CTRL2,		0x0007,
    ILI932X_POW_CTRL3,		0x0000,
    ILI932X_POW_CTRL4,		0x0000,
    TFTLCD_DELAY_200,		0,
    ILI932X_POW_CTRL1,		0x1690,
    ILI932X_POW_CTRL2,		0x0227,
    TFTLCD_DELAY_50,		0,
    ILI932X_POW_CTRL3,		0x001A,
    TFTLCD_DELAY_50,		0,
    ILI932X_POW_CTRL4,		0x1800,
    ILI932X_POW_CTRL7,		0x002A,
    TFTLCD_DELAY_50,		0,
    ILI932X_GAMMA_CTRL1,	0x0000,
    ILI932X_GAMMA_CTRL2,	0x0000,
    ILI932X_GAMMA_CTRL3,	0x0000,
    ILI932X_GAMMA_CTRL4,	0x0206,
    ILI932X_GAMMA_CTRL5,	0x0808,
    ILI932X_GAMMA_CTRL6,	0x0007,
    ILI932X_GAMMA_CTRL7,	0x0201,
    ILI932X_GAMMA_CTRL8,	0x0000,
    ILI932X_GAMMA_CTRL9,	0x0000,
    ILI932X_GAMMA_CTRL10,	0x0000,
    ILI932X_GRAM_HOR_AD,	0x0000,
    ILI932X_GRAM_VER_AD,	0x0000,
    ILI932X_HOR_START_AD,	0x0000,
    ILI932X_HOR_END_AD,		0x00EF,
    ILI932X_VER_START_AD,	0X0000,
    ILI932X_VER_END_AD,		0x013F,
    ILI932X_GATE_SCAN_CTRL1,	0xA700,	// Driver Output Control
    ILI932X_GATE_SCAN_CTRL2,	0x0003,	// Driver Output Control
    ILI932X_GATE_SCAN_CTRL3,	0x0000,	// Driver Output Control
    ILI932X_PANEL_IF_CTRL1,	0X0010,	// Panel Interface Control 1
    ILI932X_PANEL_IF_CTRL2,	0X0000,
    ILI932X_PANEL_IF_CTRL3,	0X0003,
    ILI932X_PANEL_IF_CTRL4,	0X1100,
    ILI932X_PANEL_IF_CTRL5,	0X0000,
    ILI932X_PANEL_IF_CTRL6,	0X0000,
    ILI932X_DISP_CTRL1,		0x0133,	// Main screen turn on
};

// Define RGB values for eight color bars

uint8_t red[8] = { 0, 0xff, 0, 0, 0xff, 0, 0xff, 0xff };
uint8_t grn[8] = { 0, 0, 0xff, 0, 0xff, 0xff, 0, 0xff };
uint8_t blu[8] = { 0, 0, 0, 0xff, 0, 0xff, 0xff, 0xff };

// Define values for a 320x240 1 bit/pixel images

#include "adafruit335.img"


int main(void) 
{
	
    DDRB |= LCD_Data_B;         // Set PORTB bits 0-1 for output
	
    DDRD |= 0xffff;         // Set PORTD bits 2-7 for output
	
    PORTC |= LCD_Bits_C;        // Set all the control lines high
    DDRC |= LCD_Bits_C;         // Set control port bits for output
	
    initialize();               // Initialize the LCD display
	
    regout(0x50, 0);            // Set window to the full 240x320 screen
    regout(0x51, 239);
    regout(0x52, 0);
    regout(0x53, 319);
	// Loop between displaying color bars and an image
    
	
	
	
	while (1) 
	{
		//fillScreen(TEST);
		//_delay_ms(1000);
		//fillScreen(BLACK);
		//_delay_ms(1000); 
		//fillScreen(GREEN); 
		//_delay_ms(1000);
		//fillScreen(CYAN);
		//_delay_ms(1000); 
		//fillScreen(YELLOW);
		//_delay_ms(1000); 
		//fillScreen(BLUE);
		//_delay_ms(1000); 
		//fillScreen(RED); 
		
		
		
		//_delay_ms(1000); 
		
		//color_bars();
		_delay_ms(1000);
		
		//bw_image();
		fillScreen(WHITE); 
		_delay_ms(1000); 
		
		
		//drawVerticalLine(10,10,100, RED, 5); 
		//drawHorizontalLine(10,10,300, RED, 5);
		
		makeRectangle(10,10,65,140, MAGENTA, 3); 
		makeRectangle(85,10,65,140, MAGENTA, 3); 
		makeRectangle(160,10,65,140, MAGENTA, 3); 
		makeRectangle(10,160,65,140, MAGENTA, 3); 
		makeRectangle(85,160,65,140, MAGENTA, 3); 
		makeRectangle(160,160,65,140, MAGENTA, 3); 
		_delay_ms(1000);
		
		drawChar(15, 15, 'a', BLACK, WHITE, 1);
		drawChar(15, 25, 'b', BLACK, WHITE, 1);
		drawChar(15, 35, 'c', BLACK, WHITE, 1);
		drawChar(15, 45, 'd', BLACK, WHITE, 1);
		drawChar(15, 55, 'e', BLACK, WHITE, 1);
		drawChar(15, 65, 'f', BLACK, WHITE, 1);
		drawChar(15, 75, 'g', BLACK, WHITE, 1);
		drawChar(15, 85, 'h', BLACK, WHITE, 1);
		drawChar(15, 95, 'i', BLACK, WHITE, 1);
		drawChar(15, 105, 'j', BLACK, WHITE, 1);
		drawChar(15, 115, 'k', BLACK, WHITE, 1);
		drawChar(15, 125, 'l', BLACK, WHITE, 1);
		drawChar(15, 135, 'm', BLACK, WHITE, 1);
		drawChar(15, 145, 'n', BLACK, WHITE, 1);
		drawChar(15, 165, 'o', BLACK, WHITE, 1);
		drawChar(15, 175, 'p', BLACK, WHITE, 1);
		drawChar(15, 185, 'q', BLACK, WHITE, 1);
		drawChar(15, 195, 'r', BLACK, WHITE, 1);
		drawChar(15, 205, 's', BLACK, WHITE, 1);
		drawChar(15, 215, 't', BLACK, WHITE, 1);
		drawChar(15, 225, 'u', BLACK, WHITE, 1);
		drawChar(15, 235, 'v', BLACK, WHITE, 1);
		drawChar(15, 245, 'w', BLACK, WHITE, 1);
		drawChar(15, 255, 'x', BLACK, WHITE, 1);
		drawChar(15, 265, 'y', BLACK, WHITE, 1);
		drawChar(15, 275, 'z', BLACK, WHITE, 1);
		
		drawChar(30, 185, '0', BLACK, WHITE, 1);
		drawChar(30, 195, '1', BLACK, WHITE, 1);
		drawChar(30, 205, '2', BLACK, WHITE, 1);
		drawChar(30, 215, '3', BLACK, WHITE, 1);
		drawChar(30, 225, '4', BLACK, WHITE, 1);
		drawChar(30, 235, '5', BLACK, WHITE, 1);
		drawChar(30, 245, '6', BLACK, WHITE, 1);
		drawChar(30, 255, '7', BLACK, WHITE, 1);
		drawChar(30, 265, '8', BLACK, WHITE, 1);
		drawChar(30, 275, '9', BLACK, WHITE, 1);
		
		
		/*makeRectangle(85,10,65,100, MAGENTA, 5); 
		 makeRectangle(160,10,65,100, MAGENTA, 5);
		 makeRectangle(10,160,65,100, MAGENTA, 5); 
		 makeRectangle(85,160,65,100, MAGENTA, 5); 
		 makeRectangle(160,160,65,100, MAGENTA, 5); */
		/*
		 uint16_t x,y; 
		 for (x = 10; x < 15; x= x+1)
		 {
		 for (y = 10; y<300; y = y++)
		 {
		 drawPixel(x,y,BLUE); 
		 }
		 }
		 for (x = 210; x < 215; x= x+1)
		 {
		 for (y = 10; y<300; y = y++)
		 {
		 drawPixel(x,y,BLUE); 
		 }
		 }
		 
		 for (x = 10; x < 210; x= x+1)
		 {
		 for (y = 10; y<20; y = y++)
		 {
		 drawPixel(x,y,BLUE); 
		 }
		 }
		 for (x = 10; x < 210; x= x+1)
		 {
		 for (y = 300; y<310; y = y++)
		 {
		 drawPixel(x,y,BLUE); 
		 }
		 }*/
		
		//_delay_ms(1000);
		
		
		//drawVerticalLine(10,10, 100, RED);
		//_delay_ms(1000); 
		//drawVerticalLine(10, 310, 100, RED); 
		//_delay_ms(1000); 
		//drawHorizontalLine(10,10,300, RED); 
		//_delay_ms(1000); 
		//drawHorizontalLine(310, 10, 300, RED); 
		
		//drawHorizontalLine(10, 10, 300, RED, RED); 
		_delay_ms(1000); 
		//drawHorizontalLine(110,10,300,RED, RED);
		//_delay_ms(1000);
		
		
		
		//fillScreen(TEST); 
		//_delay_ms(2000); 
		/*
		 fillScreen(MAGENTA); 
		 _delay_ms(1000); 
		 fillScreen(CYAN); 
		 _delay_ms(1000); 
		 fillScreen(GREEN); 
		 _delay_ms(1000); 
		 */
    }
}

/*
 color_bars - draw eight color bars on the full screen 
 */
void color_bars()
{
    uint8_t i, j, k;
    uint16_t color;
    uint8_t hi, lo;
	
    regout(0x20, 0);            // Set addresss counter to top left (0,0)
    regout(0x21, 0);
	
    LCD_CE_0;
	
    LCD_CD_0;
    lcdout(0);
    lcdout(0x22);
    LCD_CD_1;
	
    for (j = 0; j < 8; j++) {
		color = color565(red[j], grn[j], blu[j]);
		hi = color >> 8;
		lo = color;
		
		for (k = 0; k < 40; k++) {
			for (i = 0; i < 240; i++) {
				lcdout(hi);
				lcdout(lo);
			}
		}
    }
	
    LCD_CE_0;
}



/*
 regout - Output a 16-bit register address and 16-bit data value to the LCD
 */
void regout(uint16_t a, uint16_t d)
{
    uint8_t hi, lo;
	
    LCD_CE_0;                   // Enable the chip
	
    hi = a >> 8;
    lo = a & 0xff;
    LCD_CD_0;                   // CD = command
    lcdout(hi);                 // Send high part of reg address
    lcdout(lo);                 // Send low part of reg address
	
    hi = d >> 8;
    lo = d & 0xff;
    LCD_CD_1;                   // CD = data
    lcdout(hi);                 // High part of data
    lcdout(lo);                 // Low part of data
	
    LCD_CE_1;                   // Disable the chip
}

/*
 lcdout - Write a byte to the LCD
 */
void lcdout(uint8_t x)
{   
	//PORTD &= ~(PD7<<1);
	//PORTD |= (PD7 << 1); 
	
	//PORTB |= (PB1<<1); 
	///PORTB &= ~(PB1<<1); 
	int i ;
	for (i = 0; i < 8; i++)
	{
		
		PORTB &= ~(PB1<<1); 
		
		
		PORTB |= x & LCD_Data_BitB0;    // put low two bits in 
		PORTB &= (x | ~LCD_Data_BitB0);
		
		PORTB |= (PB1<<1);
		x = x>>1;
	}
	
    //PORTD |= x & LCD_Data_D;    // put high six bits in D
    //PORTD &= (x | ~LCD_Data_D);
    LCD_WR_0;                   // Toggle the /WR input
    LCD_WR_1;
}

/*
 initialize - Do various things to initialize the LCD.  Initialization
 data is stored in the ILI932x_regValues array as pairs of 16-bit values
 representing the register address and data.
 */
void initialize()
{
    uint16_t a, d;
    uint16_t n;
    const uint16_t *p;
	
    reset();
	
    n = sizeof(ILI932x_regValues) / sizeof(uint16_t);
    n = n/2;
    p = ILI932x_regValues;
	
    while (n--) {
		a = pgm_read_word(p++);
		d = pgm_read_word(p++);
		
		if (a == TFTLCD_DELAY_50)
			_delay_ms(50);
		else if (a == TFTLCD_DELAY_200)
			_delay_ms(200);
		else
			regout(a, d);
    }
}

/*
 reset - Force a reset of the LCD
 */
void reset()
{
    uint8_t i;
	
    PORTD |= 0xffff;        // Set all the control lines high
	
    _delay_ms(100);     // Delay 100ms to let things settle
	
    LCD_RST_0;
    _delay_us(2);      // Extend the reset a bit
    LCD_RST_1;
	
    // Not sure why this is needed, or if it is.
    LCD_CE_0;
    lcdout(0);                                                   
    for (i = 7; i != 0; i--) {
		LCD_WR_0;
		LCD_WR_1;
    }
    LCD_CE_1;
	
    _delay_ms(100);
}

/*
 color565 - Change three R, G, B byte values into a 16-bit color value
 */
uint16_t color565(uint8_t r, uint8_t g, uint8_t b)
{
    return ((r & 0xf8) << 8) | ((g & 0xfc) << 3) | (b >> 3);
}

void drawVerticalLine(uint16_t x, uint16_t y, uint16_t len, uint16_t color, uint16_t thick )
{
	uint16_t i, j; 
	for (i = x; i < x+len+thick; i++)
	{
		for (j = y; j < y+thick; j++)
		{
			drawPixel(i,j,color); 
		}
	}
	
}

void drawHorizontalLine (uint16_t x, uint16_t y, uint16_t len, uint16_t color, uint16_t thick)
{
	uint16_t i, j; 
    for (i = x; i < x+thick; i++)
	{
		for (j = y; j < y+len+thick; j++)
		{
			drawPixel(i,j,color); 
		}
	}
}



void drawDiagonalLine (uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{ // apply bresenham's algorithm ! // 
	
    regout(0x20, 0);            // Set addresss counter to top left (0,0)
    regout(0x21, 0);
	
	uint8_t hi, lo, i; 
	hi = color>>8; 
	lo = color;
	//uint8_t hib,lob;
	
	//hib = bcolor>>8; 
	//lob = bcolor; 
    LCD_CE_0;
	
    LCD_CD_0;
	lcdout(0); 
	lcdout(0x22); 
	LCD_CD_1; 
	
	int deltax = x2-x1; 
	int deltay = y2-y1; 
	double error = 0; 
	double deltaerr = (deltay/deltax); 
	uint16_t y = y1; 
	
	for (i = x1; i<x2; i++)
	{
		lcdout(hi); 
		lcdout(lo); 
		error = error + deltaerr; 
		if (error >= 0.5)
		{
			y = y+1; 
			error = error -1; 
			for (i =0; i<239+y; i++)
			{
				//lcdout(hib);
				//lcdout(lob); 
				
			}
			
		}
		
	}
	
}

void drawPixel (uint16_t x, uint16_t y, uint16_t color)
{
	
	LCD_CE_0; 
	
    regout(0x20, x);            // Set addresss counter to top left (0,0)
    regout(0x21, y);
	regout(0x22, color); 
    LCD_CE_0;
	
    LCD_CD_0;
}

void drawChar(uint16_t x, uint16_t y, unsigned char c, uint16_t color, uint16_t bg, uint16_t size)
{
	int8_t i,j; 
	for (i=0; i<6; i++ ) 
	{
		uint8_t line;
		if (i == 5)
			line = 0x0;
		else
			line = pgm_read_byte(font+(c*5)+i);
		for (j = 7; j>0; j--) 
		{
			if (line & 0x1) 
			{
				if (size == 1) // default size
				{
					drawPixel(x+j, y+i, color);
				}
				//else { // big size
				//fillRect(x+(i*size), y+(j*size), size, size, color);
			}
			else if (bg != color) 
			{
				if (size == 1) // default size
				{
					drawPixel(x+j, y+i, bg);
				}
				else 
				{ // big size
					//fillRect(x+i*size, y+j*size, size, size, bg);
				}
			}
			line >>= 1;
		}
	}
	
}

void fillScreen(uint16_t color)
{
	regout(0x20, 0);            // Set addresss counter to top left (0,0)
    regout(0x21, 0);
	
	regout(0x50, 0);            // Set window to the full 240x320 screen
    regout(0x51, 239);
    regout(0x52, 0);
    regout(0x53, 319);
	
    LCD_CE_0;
	
    LCD_CD_0;
    lcdout(0);
    lcdout(0x22);
    LCD_CD_1;
	uint8_t hi, lo; 
	int k, i; 
	hi = color >> 8;
	lo = color;
	
	
	for (k = 0; k < 320; k++)
	{
		for (i = 0; i < 240; i++) 
		{
			lcdout(hi);
			lcdout(lo);
		}
	}
}

void makeRectangle(uint16_t x, uint16_t y, uint16_t h, uint16_t w, uint16_t color, uint16_t thick)
{
	
	drawVerticalLine(x,y,h,color,thick); 
	drawVerticalLine(x,y+w,h, color, thick); 
	drawHorizontalLine(x,y,w, color, thick);
	drawHorizontalLine(x+h,y,w, color, thick); 
	
}



void fillRectangle(uint16_t x, uint16_t y, uint16_t h, uint16_t w, uint16_t color )
{
	
    regout(0x20, 0);            // Set addresss counter to top left (0,0)
    regout(0x21, 0);
	
    LCD_CE_0;
	
    LCD_CD_0;
}
