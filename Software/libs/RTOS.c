#include "BSW_Timers.h"
#include "BSW_GPIO.h"
 #include <p18cxxx.h>

//real time operating system - scheduler
void RTOS(){
	
	//1s task
	if(Timer1s){
		Timer1s=0;
		//send CAN
		//read temp
		LED1_TOGGLE

		
	}
	if(Timer100ms){
		Timer100ms=0;
		//100ms task read slave
		//LED1_OFF
			LED2_TOGGLE
	}


}

