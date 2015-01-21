#include <p18cxxx.h>
#include "BSW_Timers.h"

int Timer100ms; //
int Timer1s; //
int timerTick;
int timer100msTick;


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

