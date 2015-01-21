/**********************************************************************/
// Battery Management System (c) by WWW.SMART-ELECTRO.NET
// This source code is subject to the CreativeCommons Attribution 4.0 International (CC BY 4.0) license
//
// You can copy and redistribute the material in any medium or format.
// Remix, transform, and build upon the material for any purpose, even commercially as long as you
// don't forget to mentioned www.smart-electro.net in your work.
// More about license can be found at: http://http://creativecommons.org/licenses/by/4.0/deed.en
//
// THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
// KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LporIMITED TO THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Version: V2.0
// Author: Tadej Gortnar <tadej.gortnar@gmail.com>
// Created: Dec 2010
//
// Modified: Feb 2014 Simon.grabar <simon.grabar@gmail.com>
// -Updated header and comments
// -Updated to support 32 slaves
//
#include <p18cxxx.h>
#include "RTOS.h"
#include "BSW_Timers.h"
#include "BSW_CAN.h"
#include "BSW_GPIO.h"
#include "BSW_ReadADC.h"

//
void InterruptHandlerHigh();

extern int Timer100ms; //
extern int Timer1s; //
extern int timerTick;
extern int timer100msTick;

/*
	//TRISD = 0x00; // PortD output
    //PORTD = 0xFF;
    //LATD  = 0x00;
	OSCCON = 0b01111111;

	ADCON0 = 0x00; //disable A/D converter 
	ADCON1 = 0x0F; //select all digital 
    //TRISAbits.TRISA0 = 0x0; //changed from 0
    //PORTAbits.RA0           = 0x1;
	//PORTAbits.RA0=1;

*/
/*********************************************************/
/*                   Main function                       */
/*********************************************************/
void main(void)
{
	WDTCONbits.SWDTEN = 1;  // Enable Watch Dog
	RTOS_TimerConfig();
	CanConfig();
	GpioConfig();
	//cycicaly calling RTOS (Real time Operating System)
	while(1)
    { 
		RTOS();
		ClrWdt(); //clear watch dog
	}
}

/*********************************************************/
/*                       interrupts                      */
/*********************************************************/


// High priority interrupt vector
#pragma code InterruptVectorHigh=0x408
void InterruptVectorHigh(void)
{
      _asm
        goto InterruptHandlerHigh  //jump to interrupt routine
      _endasm
}

#pragma code
#pragma interrupt InterruptHandlerHigh

void InterruptHandlerHigh()
{
    //unsigned int t1;
    //unsigned char mdisp;

    if(INTCONbits.TMR0IF)                   //check for TMR0 overflow interrupt flag
    {
        INTCONbits.TMR0IF = 0;          //clear interrupt flag
        WriteTimer0(65035);             //Nastavi na 1/10.0 Sek
        //rx_tic++;
	    timerTick++;
	
	    if(timerTick>=360){ //100ms 
		    timerTick=0;
    		timer100msTick++;
	    	Timer100ms=1;
	
	    }
	    if(timer100msTick>=10){
		    timer100msTick=0;
		    Timer1s=1;
	    }
	} 

    if(PIR1bits.ADIF)                       //chek for ADC INT interrupt flag
    {
        PIR1bits.ADIF = 0;                  //clear interrupt flag
        ADC_Data();
    }


	//interruptCAllback();
}

