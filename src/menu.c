#include <lpc21xx.h>          // LPC2148 register definitions
#include "delay.h"            // Delay functions
#include "myLCDdefines.h"     // LCD control definitions
#include "myKPMdefines.h"     // Keypad definitions
#include "myRTCdefines.h"     // RTC related functions
#include "menu.h"             // Menu function declarations
#include "medicene.h"         // Medicine time functions

/* ---------------- DATE INPUT ---------------- */

// Function to read date from keypad and validate it (1-31)
u32 read_date()
{
        u32 d;

        while(1)
        {
                d = read2digits();    // Read 2-digit input from keypad

                if(d>=1 && d<=31)     // Valid date check
                        return d;

                // Display error message if invalid date
                CmdLCD(0x01);
                CmdLCD(0x80);
                StrLCD("INVALID DATE");
                CmdLCD(0xC0);
                StrLCD("ENTER 1-31");
                delay_s(1);
        }
}

/* ---------------- MONTH INPUT ---------------- */

// Function to read month from keypad and validate it (1-12)
u32 read_month()
{
        u32 m;

        while(1)
        {
                m = read2digits();   // Read 2-digit month

                if(m>=1 && m<=12)    // Valid month check
                        return m;

                // Display error message if invalid month
                CmdLCD(0x01);
                CmdLCD(0x80);
                StrLCD("INVALID MONTH");
                CmdLCD(0xC0);
                StrLCD("ENTER 1-12");
                delay_s(1);
        }
}

/* ---------------- YEAR INPUT ---------------- */

// Function to read 4 digit year from keypad
u32 read4digits()
{
        u8 key;
        u8 d[4];        // Array to store digits
        u8 count=0;
        u32 val;

        while(1)
        {
                key = kpm_scan();   // Scan keypad

                if(key==0)
                        continue;       // Ignore if no key pressed

                // Accept only numeric digits
                if(key>='0' && key<='9')
                {
                        if(count<4)
                        {
                                d[count]=key;    // Store digit
                                CharLCD(key);    // Display digit on LCD
                                count++;
                        }
                }

                // '=' key used as enter key
                else if(key=='=')
                {
                        if(count==4)
                                break;
                }
        }

        // Convert ASCII digits to integer value
        val=(d[0]-'0')*1000+
                (d[1]-'0')*100+
                (d[2]-'0')*10+
                (d[3]-'0');

        return val;
}

// Function to read and validate year (1500 - 2999)
u32 read_year()
{
        u32 y;

        while(1)
        {
                y=read4digits();     // Read 4-digit year

                if(y>=1500 && y<=2999)
                        return y;

                // Display error if invalid year
                CmdLCD(0x01);
                CmdLCD(0x80);
                StrLCD("INVALID YEAR");
                CmdLCD(0xC0);
                StrLCD("1500-2999");
                delay_s(1);
        }
}

// Function to read day of week (0-6)
u32 read_day()
{
        u8 key;
        u8 val;

        while(1)
        {
                key = kpm_scan();   // Scan keypad

                // Accept digits between 0 and 6
                if(key>='0' && key<='6')
                {
                        CharLCD(key);     // Display on LCD
                        val = key - '0';  // Convert ASCII to number
                }

                // '=' key confirms entry
                else if(key=='=')
                {
                        return val;
                }
        }
}

/* ---------------- MENU INPUT ---------------- */

// Function to read single digit menu choice
u8 get_menu_input()
{
        u8 key=0;

        while(key==0)
                key=kpm_scan();   // Wait until key is pressed

        return read1digits(key);   // Convert key to numeric value
}

/* ---------------- TIME INPUT ---------------- */

// Function to get hour, minute and second input
void get_time_input(u32 *h,u32 *m,u32 *s)
{
        // Get Hour
        CmdLCD(0x01);
        CmdLCD(0x80);
        StrLCD("ENTER HOUR");
        CmdLCD(0xC0);
        StrLCD("(0-23)");
        *h = hourtime();

        // Get Minute
        CmdLCD(0x01);
        CmdLCD(0x80);
        StrLCD("ENTER MIN");
        CmdLCD(0xC0);
        StrLCD("(0-59)");
        *m = mintime();

        // Get Seconds
        CmdLCD(0x01);
        CmdLCD(0x80);
        StrLCD("ENTER SEC");
        CmdLCD(0xC0);
        StrLCD("(0-59)");
        *s = sectime();
}

/* ---------------- SET MEDICINE ---------------- */

// Function to set medicine time for MED1, MED2, or MED3
void set_medicine(u8 med)
{
        u32 h,m,s;

        // Get time input from user
        get_time_input(&h,&m,&s);

        // Store time depending on medicine number
        if(med==1)
                setmedtime1(h,m,s);

        else if(med==2)
                setmedtime2(h,m,s);

        else if(med==3)
                setmedtime3(h,m,s);

        // Display confirmation message
        CmdLCD(0x01);
        StrLCD("MED TIME SET");
        delay_s(1);
        CmdLCD(0x01);
}

/* ---------------- MAIN MENU ---------------- */

// Main menu function
void menu1()
{
        u8 choice;

        while(1)
        {
                // Display main menu
                CmdLCD(0x01);
                CmdLCD(0x80);
                StrLCD("1.EditRTC 2.Med");
                CmdLCD(0xC0);
                StrLCD("3.View 4.Exit");

                choice=get_menu_input();

                switch(choice)
                {

/* ---------- EDIT RTC ---------- */

                case 1:
                {
                        u8 ch;

                        while(1)
                        {
                                // Submenu for RTC editing
                                CmdLCD(0x01);
                                CmdLCD(0x80);
                                StrLCD("1.Time 2.Date");
                                CmdLCD(0xC0);
                                StrLCD("3.Back");

                                ch=get_menu_input();

                                if(ch==1)
                                {
                                        u32 h,m,s;

                                        // Update RTC time
                                        get_time_input(&h,&m,&s);
                                        SetRTCTimeInfo(h,m,s);

                                        CmdLCD(0x01);
                                        StrLCD("TIME UPDATED");
                                        delay_s(1);
                                        CmdLCD(0x01);
                                }

                                else if(ch==2)
                                {
                                        u32 d,m,y,day;

                                        // Enter Date
                                        CmdLCD(0x01);
                                        CmdLCD(0x80);
                                        StrLCD("ENTER DATE");
                                        CmdLCD(0xC0);
                                        d=read_date();

                                        // Enter Month
                                        CmdLCD(0x01);
                                        CmdLCD(0x80);
                                        StrLCD("ENTER MONTH");
                                        CmdLCD(0xC0);
                                        m=read_month();

                                        // Enter Year
                                        CmdLCD(0x01);
                                        CmdLCD(0x80);
                                        StrLCD("ENTER YEAR");
                                        CmdLCD(0xC0);
                                        y=read_year();

                                        // Enter Day of Week
                                        CmdLCD(0x01);
                                        CmdLCD(0x80);
                                        StrLCD("ENTER DAY");
                                        CmdLCD(0xC0);
                                        StrLCD("(0-6)=");
                                        day=read_day();

                                        // Update RTC date and day
                                        SetRTCDateInfo(d,m,y);
                                        SetRTCDay(day);

                                        CmdLCD(0x01);
                                        StrLCD("DATE UPDATED");
                                        delay_s(1);
                                }

                                else if(ch==3)
                                        break;
                        }
                }
                break;

/* ---------- CONFIG MED TIME ---------- */

                case 2:
                {
                        u8 med;

                        while(1)
                        {
                                // Medicine configuration menu
                                CmdLCD(0x01);
                                CmdLCD(0x80);
                                StrLCD("1.MED1 2.MED2");
                                CmdLCD(0xC0);
                                StrLCD("3.MED3 4.Back");

                                med=get_menu_input();

                                if(med>=1 && med<=3)
                                        set_medicine(med);

                                else if(med==4)
                                        break;
                        }
                }
                break;

/* ---------- VIEW MED TIME ---------- */

                case 3:

                        // Check if medicine times are not set
                        if((m1hour==0 && m1min==0 && m1sec==0) &&
                           (m2hour==0 && m2min==0 && m2sec==0) &&
                           (m3hour==0 && m3min==0 && m3sec==0))
                        {
                                CmdLCD(0x01);
                                CmdLCD(0x80);
                                StrLCD("SET MED TIME");
                                CmdLCD(0xC0);
                                StrLCD("FIRST");
                                delay_s(2);
                        }
                        else
                        {
                                // Display medicine times
                                view_med_times();
                        }
                        CmdLCD(0x01);

                break;

/* ---------- EXIT ---------- */

                case 4:
                        CmdLCD(0x01);
                        return;

                }
        }
}

