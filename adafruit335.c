
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

#include "adafruit335.h"
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

// uint8_t red[8] = { 0, 0xff, 0, 0, 0xff, 0, 0xff, 0xff };
// uint8_t grn[8] = { 0, 0, 0xff, 0, 0xff, 0xff, 0, 0xff };
// uint8_t blu[8] = { 0, 0, 0, 0xff, 0, 0xff, 0xff, 0xff };

// Define values for a 320x240 1 bit/pixel images

#include "adafruit335.img"


/*
 color_bars - draw eight color bars on the full screen
 */
// void color_bars()
// {
//     uint8_t i, j, k;
//     uint16_t color;
//     uint8_t hi, lo;

//     regout(0x20, 0);            // Set addresss counter to top left (0,0)
//     regout(0x21, 0);

//     LCD_CE_0;

//     LCD_CD_0;
//     lcdout(0);
//     lcdout(0x22);
//     LCD_CD_1;

//     for (j = 0; j < 8; j++) {
// 		color = color565(red[j], grn[j], blu[j]);
// 		hi = color >> 8;
// 		lo = color;

// 		for (k = 0; k < 40; k++) {
// 			for (i = 0; i < 240; i++) {
// 				lcdout(hi);
// 				lcdout(lo);
// 			}
// 		}
//     }

//     LCD_CE_0;
// }



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
static uint8_t last_lcdout = 0x00;
void lcdout(uint8_t x)
{
	//PORTD &= ~(PD7<<1);
	//PORTD |= (PD7 << 1);

	//PORTB |= (PB1<<1);
	///PORTB &= ~(PB1<<1);

    // if(last_lcdout != x) {
        for (uint8_t i = 0; i < 8; i++)
        {
            PORTB &= ~(PB1<<1); // Clk to low

            PORTB |= x & LCD_Data_BitB0;    // put low two bit in
            PORTB &= (x | ~LCD_Data_BitB0);

            PORTB |= (PB1<<1); // Clk to high
            x = x>>1;
        }

        // last_lcdout = x;
    // }

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

void drawString(uint16_t x, uint16_t y, char* str, uint16_t color, uint16_t bg, uint16_t size)
{
    uint8_t i;
    for(i=0; str[i]; i++)
    {
        drawChar(x, y + size*6*i, str[i], color, bg, size);
    }
}

void drawPgmString(uint16_t x, uint16_t y, PGM_P str, uint16_t color, uint16_t bg, uint16_t size)
{
    char c;
    uint8_t i;
    while( (c = pgm_read_byte(str++)) != 0) {
        drawChar(x, y + size*6*i, c, color, bg, size);
        i++;
    }
}

void drawCircle(uint16_t x0, uint16_t y0, uint16_t radius, uint16_t color)
{
  int16_t x = radius, y = 0;
  int16_t xChange = 1 - radius*2;
  int16_t yChange = 0;
  int16_t radiusError = 0;

  while(x >= y)
  {
    drawPixel(x + x0, y + y0, color);
    drawPixel(y + x0, x + y0, color);
    drawPixel(-x + x0, y + y0, color);
    drawPixel(-y + x0, x + y0, color);
    drawPixel(-x + x0, -y + y0, color);
    drawPixel(-y + x0, -x + y0, color);
    drawPixel(x + x0, -y + y0, color);
    drawPixel(y + x0, -x + y0, color);

    y++;
    radiusError += yChange;
    yChange += 2;
    if(((radiusError << 1) + xChange) > 0)
    {
      x--;
      radiusError += xChange;
      xChange += 2;
    }
  }
}

