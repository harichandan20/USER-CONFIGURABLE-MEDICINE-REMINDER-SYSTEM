#include<lpc21xx.h>        // LPC2148 register definitions
#include"delay.h"          // Delay functions
#include"myLCDdefines.h"   // LCD pin definitions and command macros

// Function to send data/command to LCD
void WriteLCD(unsigned char data)
{
    IOCLR0 = 1 << RW;  // Set RW = 0 ? LCD in WRITE mode

    // Send 8-bit data to LCD data pins
    // First clear previous data bits, then write new data
    IOPIN0 = (IOPIN0 & ~(0XFF << LCD)) | (data << LCD);

    IOSET0 = 1 << EN;  // Enable HIGH ? latch data into LCD
    delay_us(1);       // Small delay for LCD timing

    IOCLR0 = 1 << EN;  // Enable LOW ? command executed
    delay_ms(2);       // Wait for LCD to process command
}


// Function to send command to LCD
void CmdLCD(unsigned char cmd)
{
    IOCLR0 = 1 << RS;  // RS = 0 ? Command mode
    WriteLCD(cmd);     // Send command to LCD
}


// Function to send a single character to LCD
void CharLCD(char ascii)
{
    IOSET0 = 1 << RS;  // RS = 1 ? Data mode
    WriteLCD(ascii);   // Send ASCII character
}


// Function to initialize LCD
void InitLCD(void)
{
    PINSEL0 = 0X00;    // Configure Port0 pins as GPIO
    PINSEL1 = 0X00;

    // Configure LCD pins as OUTPUT
    IODIR0 |= 0XFF << LCD;  // Data pins (8-bit mode)
    IODIR0 |= 1 << RS;      // Register Select pin
    IODIR0 |= 1 << RW;      // Read/Write pin
    IODIR0 |= 1 << EN;      // Enable pin

    delay_ms(20);           // Wait for LCD power stabilization

    // LCD Initialization Commands
    CmdLCD(_8BIT_LINE1);    // Set LCD in 8-bit mode, 1 line
    delay_ms(5);

    CmdLCD(_8BIT_LINE2);    // Set LCD in 8-bit mode, 2 lines
    CmdLCD(DISP_ON);        // Turn ON display
    CmdLCD(CLEAR_LCD);      // Clear LCD screen
    CmdLCD(CUR_SHIFT_RIGHT);// Cursor move to right
}


// Function to display a string on LCD
void StrLCD(char *str)
{
    while(*str)          // Loop until NULL character
    {
        CharLCD(*str++); // Print character and move to next
    }
}
