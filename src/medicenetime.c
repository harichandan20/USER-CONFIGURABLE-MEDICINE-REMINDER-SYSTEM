#include <lpc21xx.h>        // LPC2148 register definitions
#include "types.h"          // User defined data types
#include "delay.h"          // Delay functions
#include "medicene.h"       // Medicine related function declarations
#include "myLCDdefines.h"   // LCD functions and definitions

// Variables to store medicine alarm times
u8 m1hour,m1min,m1sec,      // Medicine 1 time
   m2hour,m2min,m2sec,      // Medicine 2 time
   m3hour,m3min,m3sec,      // Medicine 3 time
   alarm=0;                 // Alarm flag

// Function to check whether current RTC time matches any medicine time
unsigned int checkmedicene()
{
	// Compare RTC time with all medicine times
	if(((HOUR==m1hour)&&(MIN==m1min)&&(SEC==m1sec)) ||
	   ((HOUR==m2hour)&&(MIN==m2min)&&(SEC==m2sec)) ||
	   ((HOUR==m3hour)&&(MIN==m3min)&&(SEC==m3sec)))
	{
		return 1;     // Medicine time reached
	}
	return 0;        // No medicine time match
}


// Function to display all medicine times on LCD
void view_med_times()
{
	CmdLCD(0x01);    // Clear LCD

	/* MEDICINE 1 */

	CmdLCD(0x80);    // Move cursor to first line
	StrLCD("MED1 ");

	// Check if medicine time is not set
	if(m1hour==0 && m1min==0 && m1sec==0)
	{
		StrLCD("NOT SET");
	}
	else
	{
		// Display MED1 time in HH:MM format
		CharLCD(m1hour/10+'0');
		CharLCD(m1hour%10+'0');
		CharLCD(':');
		CharLCD(m1min/10+'0');
		CharLCD(m1min%10+'0');
	}

	/* MEDICINE 2 */

	CmdLCD(0xC0);   // Move cursor to second line
	StrLCD("MED2 ");

	// Check if medicine time is not set
	if(m2hour==0 && m2min==0 && m2sec==0)
	{
		StrLCD("NOT SET");
	}
	else
	{
		// Display MED2 time in HH:MM format
		CharLCD(m2hour/10+'0');
		CharLCD(m2hour%10+'0');
		CharLCD(':');
		CharLCD(m2min/10+'0');
		CharLCD(m2min%10+'0');
	}

	delay_s(3);     // Wait for 3 seconds

	CmdLCD(0x01);   // Clear LCD

	/* MEDICINE 3 */

	CmdLCD(0x80);   // Move cursor to first line
	StrLCD("MED3 ");

	// Check if medicine time is not set
	if(m3hour==0 && m3min==0 && m3sec==0)
	{
		StrLCD("NOT SET");
	}
	else
	{
		// Display MED3 time
		CharLCD(m3hour/10+'0');
		CharLCD(m3hour%10+'0');
		CharLCD(':');
		CharLCD(m3min/10+'0');
		CharLCD(m3min%10+'0');
	}

	delay_s(3);     // Wait for 3 seconds

	CmdLCD(0x01);   // Clear LCD
}


// Function to set time for Medicine 1
void setmedtime1(u8 h,u8 m,u8 s)
{
	m1hour=h;
	m1min=m;
	m1sec=s;
}


// Function to set time for Medicine 2
void setmedtime2(u8 h,u8 m,u8 s)
{
	m2hour=h;
	m2min=m;
	m2sec=s;
}


// Function to set time for Medicine 3
void setmedtime3(u8 h,u8 m,u8 s)
{
	m3hour=h;
	m3min=m;
	m3sec=s;
}


// Function to display MED1 full time (HH:MM:SS)
void displaymed1()
{
	CmdLCD(0X80);  // Move cursor to first line

	CharLCD(m1hour/10+48);
	CharLCD(m1hour%10+48);
	CharLCD(':');
	CharLCD(m1min/10+48);
	CharLCD(m1min%10+48);
	CharLCD(':');
	CharLCD(m1sec/10+48);
	CharLCD(m1sec%10+48);
}


// Function to display MED2 full time
void displaymed2()
{
	CmdLCD(0X80);

	CharLCD(m2hour/10+48);
	CharLCD(m2hour%10+48);
	CharLCD(':');
	CharLCD(m2min/10+48);
	CharLCD(m2min%10+48);
	CharLCD(':');
	CharLCD(m2sec/10+48);
	CharLCD(m2sec%10+48);
}


// Function to display MED3 full time
void displaymed3()
{
	CmdLCD(0X80);

	CharLCD(m3hour/10+48);
	CharLCD(m3hour%10+48);
	CharLCD(':');
	CharLCD(m3min/10+48);
	CharLCD(m3min%10+48);
	CharLCD(':');
	CharLCD(m3sec/10+48);
	CharLCD(m3sec%10+48);
}

