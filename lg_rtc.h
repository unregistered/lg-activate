#ifdef atmega168

#include <avr/io.h>
#include <util/delay.h>

#define DS1307_SLA_W 0XD0
#define DS1307_SLA_R 0XD1


bool ClockInit();
uint8_t GetSecond();
uint8_t GetMinute();
uint8_t GetHour();
uint8_t GetAmPm();
uint8_t GetDay();
uint8_t GetDate();
uint8_t GetMonth();
uint8_t GetYear();
bool SetSecond(uint8_t sec);
bool SetMinute(uint8_t min);
bool SetHour(uint8_t hr);
bool SetAmPm(bool pm);
bool SetDay(uint8_t day);
bool SetDate(uint8_t date);
bool SetMonth(uint8_t month);
bool SetYear(uint8_t year);
//Time as string e.g. "12:45:33 PM"
bool GetTimeString(char *Time);

//Ask user to enter Current Time using LCD and Keypad
//Then saves them to DS1307 RTC module.
bool SetTimeAuto();

void DS1307Init(void);
bool DS1307Write(uint8_t address,uint8_t data);
bool DS1307Read(uint8_t address,uint8_t *data);


#define SCLPORT	PORTC	//TAKE PORTD as SCL OUTPUT WRITE
#define SCLDDR	DDRC	//TAKE DDRB as SCL INPUT/OUTPUT configure
#define SCLPIN	PINC	//TAKE PORTD TO READ DATA

#define SDAPORT	PORTC	//TAKE PORTD as SDA OUTPUT WRITE
#define SDADDR	DDRC	//TAKE PORTD as SDA INPUT configure
#define SDAPIN	PINC	//TAKE PORTD TO READ DATA


#define SCL	PC5		//PORTD.0 PIN AS SCL PIN
#define SDA	PC4		//PORTD.1 PIN AS SDA PIN


#define SOFT_I2C_SDA_LOW	SDADDR|=((1<<SDA))
#define SOFT_I2C_SDA_HIGH	SDADDR&=(~(1<<SDA))

#define SOFT_I2C_SCL_LOW	SCLDDR|=((1<<SCL))
#define SOFT_I2C_SCL_HIGH	SCLDDR&=(~(1<<SCL))

void SoftI2CInit();
void SoftI2CStart();
void SoftI2CStop();
uint8_t SoftI2CWriteByte(uint8_t data);
uint8_t SoftI2CReadByte(uint8_t ack);

#endif
