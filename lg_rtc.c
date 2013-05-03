#ifdef atmega168

#include "lg_rtc.h"

bool ClockInit()
{
	//Initialize DS1307 Interface
	DS1307Init();

	//Clear CH bit of RTC
#define CH 7

	uint8_t temp;
	if(!DS1307Read(0x00,&temp)) return 0;

	//Clear CH Bit
	temp&=(~(1<<CH));//ch becomes 0

	if(!DS1307Write(0x00,temp)) return 0;

	//Set 12 Hour Mode
	if(!DS1307Read(0x02,&temp)) return 0;

	//Set 12Hour BIT
	temp|=(0b01000000);

	//Write Back to DS1307
	if(!DS1307Write(0x02,temp)) return 0;

	return 1;
}


uint8_t GetSecond()
{
	uint8_t sec,temp;
	//Read the Second Register
	DS1307Read(0x00,&temp);
	sec=(((temp & 0b01110000)>>4)*10)+(temp & 0b00001111);
	return sec;
}


uint8_t GetMinute()
{
	return GetSecond();
	uint8_t min,temp;
	//Read the Minute Register
	DS1307Read(0x01,&temp);
	min=(((temp & 0b01110000)>>4)*10)+(temp & 0b00001111);
	return min;
}

uint8_t GetHour()
{
	uint8_t hr,temp;
	//Read the Hour Register
	DS1307Read(0x02,&temp);
	hr=(((temp & 0b00010000)>>4)*10)+(temp & 0b00001111);//for 12-hour
	return hr;
}

//0 is AM, 1 is PM
uint8_t GetAmPm()
{
	uint8_t am_pm,temp;
	//Read the Hour Register
	DS1307Read(0x02,&temp);
	am_pm=(temp & 0b00100000)>>5;
	return am_pm;
}

uint8_t GetDay()
{
	uint8_t day, temp;
	DS1307Read(0x03,&temp);
	day=(temp & 0b00000111);
	return day;
}

uint8_t GetDate()
{
	uint8_t date, temp;
	DS1307Read(0x04,&temp);
	date=(((temp & 0b00110000)>>4)*10)+(temp & 0b00001111);
	return date;
}

uint8_t GetMonth()
{
	uint8_t month, temp;
	DS1307Read(0x05,&temp);
	month=(((temp & 0b00010000)>>4)*10)+(temp & 0b00001111);
	return month;
}

uint8_t GetYear()
{
	uint8_t year, temp;
	DS1307Read(0x06,&temp);
	year=(((temp & 0b11110000)>>4)*10)+(temp & 0b00001111);
	return year;
}


bool SetSecond(uint8_t sec)
{
	uint8_t temp,result;

	temp=((sec/10)<<4)|(sec%10);
	result=DS1307Write(0x00,temp);

	return result;
}


bool SetMinute(uint8_t min)
{
	uint8_t temp,result;
	temp=((min/10)<<4)|(min%10);
	result=DS1307Write(0x01,temp);
	return result;
}

bool SetHour(uint8_t hr)
{
	uint8_t temp,result,am_pm;
	am_pm=GetAmPm();
	temp=((hr/10)<<4)|(hr%10);
	temp|=0b01000000; //12 Hr Mode

	if(am_pm)
	{
		temp|=0b00100000;
	}
	result=DS1307Write(0x02,temp);

	return result;

}


//0 is AM 1 is PM
bool SetAmPm(bool pm)
{
	uint8_t temp,result;

	DS1307Read(0x02,&temp);

	if(pm)
		temp|=0b00100000;//SET
	else
		temp&=0b11011111;//CLEAR

	result=DS1307Write(0x02,temp);

	return result;

}

bool SetDay(uint8_t day)
{
	uint8_t temp,result;
	temp=(day%10);
	result=DS1307Write(0x03,temp);
	return result;
}

bool SetDate(uint8_t date)
{
	uint8_t temp,result;
	temp=((date/10)<<4)|(date%10);
	result=DS1307Write(0x04,temp);
	return result;
}

bool SetMonth(uint8_t month)
{
	uint8_t temp,result;
	temp=((month/10)<<4)|(month%10);
	result=DS1307Write(0x05,temp);
	return result;
}

bool SetYear(uint8_t year)
{
	uint8_t temp,result;
	temp=((year/10)<<4)|(year%10);
	result=DS1307Write(0x06,temp);
	return result;
}




/***************************************

 Gets the current time as a ascii/text string.

 Example 12:49:22 PM (HH:MM:SS: PM)


 Parameters
 ----------
 Pointer to a string.

 Return Value
 ------------
 0=failure
 1=success

 Example Usage
 -------------

 char time[12];			//The string must be at least 12bytes long
 GetTimeString(time);	//Now the string time contains the current time


 ****************************************/

bool GetTimeString(char *Time)
{
	uint8_t data;

	if(!DS1307Read(0x00,&data))
	{
		//I/O Error
		return 0;
	}

	Time[11]='\0';
	Time[10]='M';
	Time[8]=' ';

	Time[7]=48+(data & 0b00001111);
	Time[6]=48+((data & 0b01110000)>>4);
	Time[5]=':';

	if(!DS1307Read(0x01,&data))
	{
		//I/O Error
		return 0;
	}

	Time[4]=48+(data & 0b00001111);
	Time[3]=48+((data & 0b01110000)>>4);
	Time[2]=':';

	if(!DS1307Read(0x02,&data))
	{
		//I/O Error
		return 0;
	}

	Time[1]=48+(data & 0b00001111);
	Time[0]=48+((data & 0b00010000)>>4);

	//AM/PM
	if(data & 0b00100000)
	{
		Time[9]='P';
	}
	else
	{
		Time[9]='A';
	}

	return 1;//Success

}

void DS1307Init(void)
{
	SoftI2CInit();
}



bool DS1307Read(uint8_t address,uint8_t *data)
{
	uint8_t res;	//result

	//Start
	SoftI2CStart();

	//SLA+W (for dummy write to set register pointer)
	res=SoftI2CWriteByte(0xD0);	//DS1307 address + W

	//Error
	if(!res)	return false;

	//Now send the address of required register
	res=SoftI2CWriteByte(address);

	//Error
	if(!res)	return false;

	//Repeat Start
	SoftI2CStart();

	//SLA + R
	res=SoftI2CWriteByte(DS1307_SLA_R);	//DS1307 Address + R

	//Error
	if(!res)	return false;

	//Now read the value with NACK
	*data=SoftI2CReadByte(0);

	//Error
	if(!res)	return false;

	//STOP
	SoftI2CStop();

	return true;
}



bool DS1307Write(uint8_t address,uint8_t data)
{
	uint8_t res;	//result

	//Start
	SoftI2CStart();

	//SLA+W
	res=SoftI2CWriteByte(DS1307_SLA_W);	//DS1307 address + W

	//Error
	if(!res)	return false;

	//Now send the address of required register
	res=SoftI2CWriteByte(address);

	//Error
	if(!res)	return false;

	//Now write the value
	res=SoftI2CWriteByte(data);

	//Error
	if(!res)	return false;

	//STOP
	SoftI2CStop();

	return true;
}




void SoftI2CInit()
{
	SDAPORT&=(1<<SDA);
	SCLPORT&=(1<<SCL);

	SDADDR&=(~(1<<SDA));
	SCLDDR&=(~(1<<SCL));

}
void SoftI2CStart()
{
	SCLDDR&=(~(1<<SCL));
	_delay_loop_2(5);

	SDADDR|=((1<<SDA));
	_delay_loop_2(5);
}

void SoftI2CStop()
{
	SDADDR|=((1<<SDA));
	_delay_loop_2(5);
	SCLDDR&=(~(1<<SCL));
	_delay_loop_2(3);
	SDADDR&=(~(1<<SDA));
	_delay_loop_2(5);
}

uint8_t SoftI2CWriteByte(uint8_t data)
{

	uint8_t i;

	for(i=0;i<8;i++)
	{
		SCLDDR|=((1<<SCL));
		_delay_loop_2(3);

		if(data & 0x80)
			SDADDR&=(~(1<<SDA));
		else
			SDADDR|=((1<<SDA));

		_delay_loop_2(5);

		SCLDDR&=(~(1<<SCL));
		_delay_loop_2(5);

		while((SCLPIN & (1<<SCL))==0);//pinA and PA0

		data=data<<1;
	}

	//The 9th clock (ACK Phase)
	SCLDDR|=((1<<SCL));
	_delay_loop_2(3);

	SDADDR&=(~(1<<SDA));
	_delay_loop_2(5);

	SCLDDR&=(~(1<<SCL));
	_delay_loop_2(5);

	uint8_t ack=!(SDAPIN & (1<<SDA));
	//set ack == 0 if error
	//means can't be congruent

	SCLDDR|=((1<<SCL));
	_delay_loop_2(5);

	return ack;

}


uint8_t SoftI2CReadByte(uint8_t ack)
{
	uint8_t data=0x00;
	uint8_t i;

	for(i=0;i<8;i++)
	{

		SCLDDR|=((1<<SCL));
		_delay_loop_2(5);
		SCLDDR&=(~(1<<SCL));
		_delay_loop_2(5);

		while((SCLPIN & (1<<SCL))==0);

		if(SDAPIN &(1<<SDA))
			data|=(0x80>>i);

	}

	SCLDDR|=((1<<SCL));
	_delay_loop_2(3);				//Soft_I2C_Put_Ack

	if(ack)
	{
		SDADDR|=((1<<SDA));
	}
	else
	{
		SDADDR&=(~(1<<SDA));
	}
	_delay_loop_2(5);

	SCLDDR&=(~(1<<SCL));
	_delay_loop_2(5);

	SCLDDR|=((1<<SCL));
	_delay_loop_2(5);

	return data;

}


#endif
