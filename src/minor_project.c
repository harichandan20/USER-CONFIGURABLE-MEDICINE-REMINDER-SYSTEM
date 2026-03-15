#include <lpc21xx.h>
#include "delay.h"
#include "myLCDdefines.h"
#include "myKPMdefines.h"
#include "myRTCdefines.h"
#include "Einto.h"
#include "menu.h"
#include "medicene.h"

/* -------------------- Switch Definitions -------------------- */
#define SW1 (1<<30)
#define SW2 (1<<31)

/* -------------------- Day Definitions -------------------- */
#define SUN 0
#define MON 1
#define TUE 2
#define WED 3
#define THU 4
#define FRI 5
#define SAT 6

#define LED (1<<24)

/* -------------------- Global Variables -------------------- */
s32 hour, min, sec;
s32 date, month, year;
s32 day, i;

volatile u8 f1 = 0;
volatile u8 f2 = 0;

u8 buzz = 0;


/* ============================================================
                       MAIN FUNCTION
============================================================ */
int main()
{
    /* Configure buzzer pin as output */
    IODIR0 |= (1<<4);

    /* Initialize LCD */
    InitLCD();

    /* Initialize Keypad */
    InitKPM();

                RTC_Init();

    /* Set Initial RTC Time (HH,MM,SS) */
    SetRTCTimeInfo(11, 42, 45);

    /* Set Initial RTC Date (DD,MM,YYYY) */
    SetRTCDateInfo(10, 3, 2026);

    /* Set Day (SUN–SAT) */
    SetRTCDay(2);

    /* Enable External Interrupts */
    Eint0();   // Switch 1
    Eint1();   // Switch 2

                CmdLCD(0x80);
    StrLCD("    Medicene   ");

    CmdLCD(0xC0);
    StrLCD("Reminder System");

    delay_s(2);
                CmdLCD(0x01);
    /* -------------------- Main Loop -------------------- */
    while(1)
    {


        /* ----------- Enter Menu if SW1 pressed ----------- */
        if(f1 == 1)
        {
            menu1();
            f1 = 0;
        }

        /* ----------- Stop Buzzer if SW2 pressed ----------- */
        else if(f2 == 1)
        {
            IOCLR0 = (1<<4);
            f2 = 0;
        }

        else
        {
            /* ----------- Display Time ----------- */
            GetRTCTimeInfo(&hour, &min, &sec);
            DisplayRTCTime(hour, min, sec);

            /* ----------- Display Date ----------- */
            GetRTCDateInfo(&date, &month, &year);
            DisplayRTCDate(date, month, year);

            /* ----------- Display Day ----------- */
            GetRTCDay(&day);
            DisplayRTCDay(day);

            /* ----------- Check Medicine Schedule ----------- */
            buzz = checkmedicene();

            if(buzz == 1)
            {
                CmdLCD(0x01);

                CmdLCD(0x80);
                StrLCD("TAKE MEDICINE");

                CmdLCD(0xC0);
                StrLCD("PRESS SW2");

                IOSET0 = (1<<4);

                /* Wait for user acknowledgement */
                while(1)
                {
                    delay_ms(100);

                    if(f2 == 1)
                    {
                        IOCLR0 = (1<<4);
                        f2 = 0;
                        break;
                    }
                }

                /* Confirmation Messages */

                CmdLCD(0x01);
                CmdLCD(0x80);
                StrLCD("MEDICINE");

                CmdLCD(0xC0);
                StrLCD("TAKEN");

                delay_s(1);

                CmdLCD(0x01);
                CmdLCD(0x80);
                StrLCD("HAVE A NICE");

                CmdLCD(0xC0);
                StrLCD("DAY");

                delay_s(1);
                CmdLCD(0x01);
            }
        }
    }
}
