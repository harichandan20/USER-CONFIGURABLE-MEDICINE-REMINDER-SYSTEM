#include <lpc21xx.h>        // LPC2148 register definitions
#include "myLCDdefines.h"   // LCD functions and command definitions
#include "delay.h"          // Delay functions
#include "types.h"          // User defined data types

// Define row and column pins for keypad matrix
#define rows (0x0F<<16)     // P1.16 - P1.19 used as row outputs
#define cols (0x0F<<20)     // P1.20 - P1.23 used as column inputs


// Keypad lookup table for 4x4 matrix keypad
unsigned char kpmlut[4][4]={
 {'7','8','9','/'},
 {'4','5','6','*'},
 {'1','2','3','-'},
 {'C','0','=','+'}};


// Function to initialize keypad
void InitKPM(void)
{
        IODIR1|=rows;    // Configure row pins as output
        IODIR1&=~cols;   // Configure column pins as input

        //IOSET1 = rows;  // (Optional) Set rows HIGH initially
        //IOSET1 = cols;  // (Optional) Enable pull-up on columns
}


// Function to scan keypad and detect pressed key
unsigned char kpm_scan(void)
{
        unsigned int r,c;

        // Scan each row
        for(r=0;r<4;r++)
        {
                IOSET1=rows;            // Set all rows HIGH
                IOCLR1=(1<<(r+16));     // Set current row LOW

                delay_ms(20);           // Debounce delay

                // Check each column
                for(c=0; c<4; c++){

                        // If column goes LOW ? key pressed
                        if(!(IOPIN1 & (1<<(c+20)))){

                                // Wait until key is released
                                while(!(IOPIN1 & (1<<(c+20))));

                                // Return corresponding key from lookup table
                                return kpmlut[r][c];
                        }
                }
        }

        return 0;   // No key pressed
}


// Function to read single digit from keypad
unsigned int read1digits(u8 k1)
{
        //u8 k1=0;
        u32 val=0;

        // Wait until valid key is pressed
        while(k1==0)
        {
                k1=kpm_scan();
        }

        //CharLCD(k1);  // Optional display of key

        val=(k1-'0');   // Convert ASCII digit to integer

        return val;
}


// Function to read two-digit number from keypad
unsigned int read2digits()
{
        u8 key;
        u8 d[2];       // Array to store digits
        u8 count = 0;
        u32 val;

        while(1)
        {
                key = kpm_scan();

                if(key==0)
                        continue;

                // Digit input
                if(key>='0' && key<='9')
                {
                        if(count<2)
                        {
                                d[count]=key;      // Store digit
                                CharLCD(key);      // Display on LCD
                                count++;
                        }
                }

                // Backspace using '/' key
                else if(key=='/')
                {
                        if(count>0)
                        {
                                count--;

                                // Move cursor back
                                CmdLCD(0x10);

                                // Erase previous character
                                CharLCD(' ');

                                // Move cursor back again
                                CmdLCD(0x10);
                        }
                }

                // Confirm entry using '=' key
                else if(key=='=')
                {
                        if(count>=1)   // Allow 1 or 2 digits
                                break;
                }
        }

        // Convert ASCII digits to integer value
        if(count==1)
                val=(d[0]-'0');              // Example: 9
        else
                val=(d[0]-'0')*10+(d[1]-'0'); // Example: 09 or 23

        return val;
}


// Function to read valid hour input (0-23)
unsigned int hourtime()
{
        while(1)
        {
                u32 val=read2digits();

                if(val<24)      // Valid hour check
                {
                        return val;
                }
                else{

                // Display error message
                CmdLCD(0x01);
                CmdLCD(0x80);
                StrLCD("Enter Valid");
                CmdLCD(0xc0);
                StrLCD("Number(0-23):");
                }
        }
}


// Function to read valid minute input (0-59)
unsigned int mintime()
{
        while(1)
        {
                u32 val=read2digits();

                if(val<60)
                {
                        return val;
                }
                else{

                // Display error message
                CmdLCD(0x01);
                CmdLCD(0x80);
                StrLCD("Enter Valid");
                CmdLCD(0xc0);
                StrLCD("Min(0-59):");
                }
        }
}


// Function to read valid seconds input (0-59)
unsigned int sectime()
{
        while(1)
        {
                u32 val=read2digits();

                if(val<60)
                {
                        return val;
                }
                else{

                // Display error message
                CmdLCD(0x01);
                CmdLCD(0x80);
                StrLCD("Enter Valid");
                CmdLCD(0xc0);
                StrLCD("Sec(0-59):");
                }
        }
}
