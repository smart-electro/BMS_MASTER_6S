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
#include "BSW_GPIO.h"
#include "BSW_CAN.h"
#include "BSW_ReadADC.h"
#include "APP_Settings.h"
#include "BSW_SoftUART.h"

/*********************************************************/
/*                   Main function                       */
/*********************************************************/
void main(void)
{
	WDTCONbits.SWDTEN = 1;  // Enable Watch Dog
	GpioConfig();
	RTOS_TimerConfig();

	CanConfig();
	readSettings();
	ADC_Init();
	softuart_init();

	//disable comparator!!
	CMCONbits.CM0=1;
	CMCONbits.CM1=1;
	CMCONbits.CM2=1;

	
	//cycicaly calling RTOS (Real time Operating System)	
	while(1)
    { 
		RTOS();
		ClrWdt(); //clear watch dog
	}
}
