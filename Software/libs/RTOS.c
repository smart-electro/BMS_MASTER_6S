//#if !defined(BSW_TIMERS_H)          // To avoid duplicate inclusion
    #include "BSW_Timers.h"
//#endif
#include "BSW_ReadADC.h"
#include "BSW_GPIO.h"
#include "BSW_SoftUART.h"
#include "APP_Readtemperature.h"
#include "APP_SendBmsData.h"
#include "APP_ReadBmsSlave.h"
#include "APP_Settings.h"
#include "RTOS.h"
#include <p18cxxx.h>
#include <timers.h>

//extern char SlaveReceiveTimeout;

extern unsigned char ADC_Chanels[ADC_ChanalNo];
extern unsigned char ADC_ChanelCnt;
extern unsigned char ADC_SampleCnt;
extern unsigned char ADC_OK;

unsigned int timerTick;
unsigned int timer100msTick;

/*-##-----------Prototypes----------------------------------------------------*/
void Interrupt_low (void);
void Interrupt_high (void);
void Timer0_int (void);
void Timer1_int (void);

//real time operating system - scheduler
void RTOS(){
	
	//1s task
	if(Timer1s){
		Timer1s=0;
		LED1_TOGGLE
		
		
		sendCellMinVoltage_send=1;	
		sendCellMaxBalancing_send=1;
		sendCellMaxVoltage_send=1;
		
		if (ADC_OK)
		{
            CalcTemps();
			ADC_ChanelCnt = 0;                  //Zaženi ADC sekvencao
            ADC_SampleCnt = 0;
            SelChanConvADC(ADC_Chanels[ADC_ChanelCnt]);
            ADC_OK = 0;
		}
		
	}
	if(Timer100ms){
		Timer100ms=0;
		ADC_Data();
		//100ms task read slave
		sendBmsStatus_send=1;
		sendCellVoltages_send=1;
		sendCellBalancingStatus_send=1;


		SlaveReceiveTimeout++;
		
	}

 	sendCellVoltages();
	sendBmsStatus();
 	sendCellBalancingStatus();
 	sendCellMaxBalancing();
 	sendCellMaxVoltage();
 	sendCellMinVoltage();

	readBmsSlaves();
	checkForCan();  //read can for settings

}

/*********************************************************/
/*                       interrupts                      */
/*********************************************************/

// Timer 0 Interrupt Service Routine (Timer 0 ISR)
#pragma interrupt Timer0_int
void Timer0_int(void) //##/////////////////////////////////////////////////////
{
	T1CONbits.TMR1ON=0;  //stop timer1

	INTCONbits.TMR0IF=0;	// Clear Timer 0 overflow interrupt flag
	//WriteTimer0(65000);
	WriteTimer0(Timer0Value); 	
	
	interruptCAllback(); //SW uart

	T1CONbits.TMR1ON=1; //start back timer1					
}// Timer0_int() END ///////////////////////////////////////////////////////////




// Timer 1 Interrupt Service Routine (Timer 1 ISR)
#pragma interruptlow Timer1_int
void Timer1_int(void) //##/////////////////////////////////////////////////////
{

//	if(PIR1bits.TMR1IF)                 // Timer 1 INT  
   // {	
		PIR1bits.TMR1IF = 0;	// Clear Timer 1 overflow interrupt flag	
		WriteTimer1(40536);

		LED2_TOGGLE
	    timerTick++;
	
	    if(timerTick>=1){ //100ms 
		    timerTick=0;
    		timer100msTick++;
	    	Timer100ms=1;
	
	    }
	    if(timer100msTick>=10){
		    timer100msTick=0;
		    Timer1s=1;
	    }
//	}
  /*  if(PIR1bits.ADIF)                    // ADC INT
    {
        PIR1bits.ADIF = 0;               //clear interrupt flag
        ADC_Data();
    }*/

}// Timer1_int() END ///////////////////////////////////////////////////////////

// === Interrupt Vector Table START ===
#pragma code int_high=0x000008
void Interrupt_high(void) //##/////////////////////////////////////////////////
{
	_asm GOTO Timer0_int _endasm
}// Interrupt_high() END ///////////////////////////////////////////////////////

#pragma code int_low=0x000018
void Interrupt_low(void) //##//////////////////////////////////////////////////
{
	_asm GOTO Timer1_int _endasm
}// Interrupt_low() END ////////////////////////////////////////////////////////

