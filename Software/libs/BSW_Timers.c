#include <p18cxxx.h>
#include "BSW_Timers.h"


int timerTick=0;
int timer100msTick=0;


void RTOS_TimerConfig(){

    // Initialize TIMER0
    INTCON2bits.TMR0IP = 0;        //Timer0 INT-LOW
    //OpenTimer0(TIMER_INT_ON & T0_16BIT & T0_SOURCE_INT & T0_PS_1_64);
    OpenTimer0(TIMER_INT_ON & T0_16BIT & T0_SOURCE_INT & T0_PS_1_1);
    //WriteTimer0(62411);            // 1/10Sec interval --  8Mhz/4/64 := 3125 -> 65536 - 3125 = 62411
	//for SW uart we need 1/3 baud = 1200/3=400
	WriteTimer0(64981);
    INTCONbits.GIE = 1;            //enable global interrupts



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


	//interruptCAllback();
}
