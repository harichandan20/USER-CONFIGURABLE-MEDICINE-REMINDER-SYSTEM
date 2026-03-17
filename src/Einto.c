#include <lpc21xx.h>        // LPC2148 register definitions
#include "myLCDdefines.h"   // LCD function definitions
#include "myKPMdefines.h"   // Keypad definitions
#include "delay.h"          // Delay functions
#include "Einto.h"          // External interrupt function declarations
#include "types.h"          // User defined data types
#include "menu.h"           // Menu related functions

// External flags used to indicate interrupt occurrence
extern volatile u8 f1;   // Flag for EINT0
extern volatile u8 f2;   // Flag for EINT1


// Function to configure External Interrupt 0 (EINT0)
void Eint0()
{
	PINSEL0 &= ~(3<<2);   // Clear bits for P0.1 pin function
	PINSEL0 |= 0X0C;      // Configure P0.1 as EINT0
	
	VICIntEnable = 1<<14;         // Enable interrupt source 14 (EINT0) in VIC
	VICVectCntl0 = (1<<5)|14;     // Enable vector slot 0 and assign EINT0 interrupt
	VICVectAddr0 = (unsigned int)EINT0_ISR; // Set ISR address for EINT0
}


// Function to configure External Interrupt 1 (EINT1)
void Eint1()
{
	PINSEL0 &= ~(3<<6);   // Clear bits for P0.3 pin function
	PINSEL0 |= 0XC0;      // Configure P0.3 as EINT1
	
	VICIntEnable = 1<<15;        // Enable interrupt source 15 (EINT1)
	VICVectCntl1 = (1<<5)|15;    // Enable vector slot 1 and assign EINT1 interrupt
	VICVectAddr1 = (unsigned int)EINT1_ISR; // Set ISR address for EINT1
}


// Interrupt Service Routine for EINT0
void EINT0_ISR(void)__irq
{
	f1 = 1;                // Set flag indicating EINT0 occurred
	
	EXTINT = 1<<0;         // Clear EINT0 interrupt flag
	VICVectAddr = 0;       // Acknowledge interrupt completion to VIC
}


// Interrupt Service Routine for EINT1
void EINT1_ISR(void)__irq
{
	f2 = 1;                // Set flag indicating EINT1 occurred
	
	EXTINT = 1<<1;         // Clear EINT1 interrupt flag
	VICVectAddr = 0;       // Acknowledge interrupt completion to VIC
}
