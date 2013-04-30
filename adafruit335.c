
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
//#include "adatouch.cpp"
#include "util.h"
//#include "lgserial.h"

void color_bars();
void drawHorizontalLine(uint16_t /*x*/, uint16_t/*y*/, uint16_t /*length*/, uint16_t /*color*/, uint16_t /*thickness*/);
void drawVerticalLine (uint16_t /*x*/, uint16_t /*y*/, uint16_t /*length*/, uint16_t /*color*/, uint16_t /*thickness*/);
void drawDiagonalLine (uint16_t/*x1*/, uint16_t /*y1*/, uint16_t /*x2*/, uint16_t /*y2*/, uint16_t /*color*/);
void drawPixel (uint16_t /*x1*/, uint16_t /*y1*/, uint16_t /*color*/);
void fillScreen(uint16_t /*color*/);
void fillRectangle(uint16_t /*x*/, uint16_t /*y*/, uint16_t /*h*/, uint16_t /*w*/, uint16_t /*color*/, uint16_t	/*thick*/);
void makeRectangle(uint16_t /*x*/, uint16_t /*y*/, uint16_t /*h*/, uint16_t /*w*/, uint16_t /*color*/, uint16_t /*thick*/);
void drawChar(uint16_t /*x*/, uint16_t /*y*/, const char c/*h*/, uint16_t /*color*/, uint16_t /*bg*/, uint16_t /*size*/);
void drawString(uint16_t /*x*/, uint16_t /*y*/, const char* /*str*/, uint16_t /*color*/, uint16_t /*bg*/, uint16_t /*size*/);
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
void adafruit_initialize()
{
    DDRB |= LCD_Data_B;
    DDRD |= 0xffff;
    //PORTC |= LCD_Bits_C;
    //DDRC |= LCD_Bits_C;


	//LGSerial::init();
	//LGSerial::put("Hello World!");
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

    regout(0x50, 0);            // Set window to the full 240x320 screen
    regout(0x51, 239);
    regout(0x52, 0);
    regout(0x53, 319);
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

void drawChar(uint16_t x, uint16_t y, const char c, uint16_t color, uint16_t bg, uint16_t size)
{
	int8_t i,j;
	if (size == 1)
	{
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
					drawPixel(x+j, y+i, color);
				}
				else if (bg != color)
				{
					drawPixel(x+j, y+i, bg);
				}
				line >>= 1;
			}
		}
	}
	else
	{
		for (i=0; i<size*5+1; i=i+size )
		{
			uint8_t line;
			if (i == size*5)
				line = 0x0;
			else
				line = pgm_read_byte(font+(c*5)+i/size);
			for (j = size*6+1; j>0; j=j-size)
			{
				if (line & 0x1)
				{
					drawPixel(x+j, y+i, color);
					drawPixel(x+j-1, y+i, color);
					drawPixel(x+j, y+i+1, color);
					drawPixel(x+j-1, y+i+1, color);
				}
				else if (bg != color)
				{
					drawPixel(x+j, y+i, bg);
					drawPixel(x+j-1, y+i, bg);
					drawPixel(x+j, y+i+1, bg);
					drawPixel(x+j-1, y+i+1, bg);
				}
				line >>= 1;
			}
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



void fillRectangle(uint16_t x, uint16_t y, uint16_t h, uint16_t w, uint16_t color, uint16_t thick )
{
	uint16_t i ,j;
	for (i = y+thick; i < y+w; i++)
	{
		drawVerticalLine(x+thick,i,h-thick, color,1);
	}


}

void drawString(uint16_t x, uint16_t y, const char* str, uint16_t color, uint16_t bg, uint16_t size)
{
	uint8_t i;
	for(i=0; str[i]; i++)
	{
        drawChar(x, y + size*6*i, str[i], color, bg, size);
    }
}


void homeMenu()
{

	sleep(100);




	uint16_t schcolor = color565(142,35,35); //red
	uint16_t setcolor = color565(35, 142, 35); // green
	uint16_t devcolor = color565(205,173,0); // yellow
	uint16_t stacolor = color565(51,153,204); //blue
	fillScreen(WHITE);
	makeRectangle(10,10,90,140,BLACK, 4);
	makeRectangle(15,15,90,140,setcolor, 4);
	//makeRectangle(15,15,90,140,MAGENTA, 3);
	makeRectangle(10,160,90,140, BLACK, 4);
	makeRectangle(15,165,90,140,devcolor,4);
	//makeRectangle(15,165,90,140, BLACK, 3);
	makeRectangle(110,10, 90, 140, BLACK, 4);
	makeRectangle(115,15,90,140, schcolor,4);
	//makeRectangle(115,15, 90, 140, BLACK, 3);
	makeRectangle(110,160, 90, 140, BLACK, 4);
	makeRectangle(115,165, 90, 140, stacolor, 4);
	//makeRectangle(115,165, 90, 140, MAGENTA, 3);

	const char * home = "HOME";
	const char * set = "Settings";
	const char * sched = "Schedule";
	const char * dev = "Add Device";
	const char * stats = "Status";
	const char * day = "04/25/13";

	drawString(215,120 , home, BLACK, WHITE, 3);
	drawString(55,35, set, BLACK, setcolor, 2);
	drawString(155,20, dev, BLACK, schcolor, 2);
	drawString(55, 180, sched, BLACK, devcolor, 2);
	drawString(155,200, stats, BLACK, stacolor, 2);

	drawString(224, 1 , day, GREY, WHITE, 2);
	makeRectangle(223,0, 15,100, RED, 2);
	makeRectangle(224, 219, 15, 100, RED, 2);
	/*
	unsigned int minutes = 0;
	unsigned int secs = 0;
	const char* secdisp = "0";
	const char* mindisp = "0";
	const char t = "";*/
	const char * time = "01:23 PM";
	drawString(225, 220, time, GREY, WHITE, 2);
	int x,y;
	/*while(1)
	{
		/*
		sleep(1000);
		TouchScreen ts = TouchScreen(10,10,10,10,10);
		DDRC = 0x0;
		DDRC = 0x5;

		x = ts.readTouchYM();
		//LGSerial::print(y);
		//DDRC |= 0xf;
		DDRC = 0x0;
		DDRC = 0xa;

		y = ts.readTouchXM();
		//LGSerial::print(x);
		*/
		/*
		_delay_ms(100);
		;
		if (secs%2 == 0)
		{
		time = "        ";
		}
		else
		{
		time = "01:24 PM";
		}

			secs++;
		drawString(225, 220, time, GREY, WHITE, 2);
		if (secs > 10)
		{
			minutes++;
			secs = 0;
		}*/
		/*
		if (x > 30 && x<100 && y>0 && y < 130)
		{
			settingsMenu();
		}

		else if (y> 140 && y<220 && x>30 && x<100)
		{
			devicesMenu();
		}
		*/
		//else if (/*x>125 && x<215*/ &&y>60 && y<130)
		//{
				//scheduleMenu();
		//}

		/*
		else if (x>125 && x<215 && y>140 && y<220)
		{
			statusMenu();
		}*;

		 /*
		secdisp += (unsigned char)secdisp;
		mindisp += (unsigned char)minutes;
		t = secdisp + ":" + minutes + "PM";
		unsigned char * temp = &t;
		drawString(215, 210, time, GREY, WHITE, 2);

	}*/
}


void statusMenu()
{
	uint16_t stacolor = color565(51,153,204); //blue
	fillScreen(stacolor);
	makeRectangle(15,15, 200,280, BLACK, 5);
	const char* status = "STATUS";
	drawString(210,50 , status, BLACK, stacolor, 2);



}
void devicesMenu()
{	uint16_t schcolor = color565(142,35,35);
	fillScreen(schcolor);
	makeRectangle(15,15, 200,280, BLACK, 5);
	const char* AddDevices = "ADD ADAPTERS";
	drawString(210,50 , AddDevices, BLACK, schcolor, 2);
}
void settingsMenu()
{	uint16_t setcolor = color565(35,142,35);
	fillScreen(setcolor);
	makeRectangle(15,15, 200,280, BLACK, 5);
	const char* settings = "SETTINGS";
	drawString(210,50 ,settings, BLACK, setcolor, 2);
}
void scheduleMenu()
{
	uint16_t devcolor = color565(205,173,0);
	fillScreen(devcolor);
	makeRectangle(15,15, 200,280, BLACK, 5);
	const char* Adapter = "PICK ADAPTER";
	drawString(210,50 , Adapter, BLACK, devcolor, 2);


}
