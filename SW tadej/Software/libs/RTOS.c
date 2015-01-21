#if !defined(BSW_TIMERS_H)          // To avoid duplicate inclusion
    #include "BSW_Timers.h"
#endif

#include "BSW_GPIO.h"
#include "APP_SendBmsData.h"
#include "APP_ReadBmsSlave.h"
#include <p18cxxx.h>

extern int Timer100ms; //
extern int Timer1s; //
extern char SlaveReceiveTimeout;
//real time operating system - scheduler
void RTOS(){
	
	//1s task
	if(Timer1s){
		Timer1s=0;
		//send CAN
		//read temp
		LED1_TOGGLE

		
		sendCellVoltages();
		sendCellBalancingStatus();	

			

		
	}
	if(Timer100ms){
		Timer100ms=0;
		//100ms task read slave
		sendBmsStatus();
		
		SlaveReceiveTimeout++;
		LED2_TOGGLE
	}

	
	readBmsSlaves();

}

