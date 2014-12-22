#include "BSW_SoftUART.h"
#include "APP_ReadBmsSlave.h"
#include "APP_AuxFunctions.h"
#include "BSW_GPIO.h"
#include <p18cxxx.h>
#include "BSW_CAN.h"
#include "RTOS.h"
unsigned char rx_cnt=0;

unsigned char BatChar[8];


void processReceivedByte(char Rx_Char);
void NaslednjiPort();

#define CELL_BALANCING 0x4f  // 0 character

void readBmsSlaves()
{


		//sw uart read data
		if ( softuart_kbhit() ) {
			processReceivedByte(softuart_getchar());
		}

		//receive timeout
		if(SlaveReceiveTimeout >= 2) {
			SlaveReceiveTimeout=0;
			//slaves can have +-10% baudrate. If timeout start sweping the baudrate (timer0)
			// 8Mhz/4 = 2M samples/s
			//			    1110*3 -> WriteTimer0(65535-600);64935
			//			    1130*3 -> WriteTimer0(65535-589);64946
			//default		1150*3 -> WriteTimer0(65535-580);64955
			//			    1170*3 -> WriteTimer0(65535-569);64966
			//			    1190*3 -> WriteTimer0(65535-560);64975
			//			    1250*3 -> WriteTimer0(65535-533);65002
			SlaveReceiveMasterTimeout++;
			Timer0Value+=5;
 			if(Timer0Value >= 65100) Timer0Value=64935;
 			if(Timer0Value <= 64935) Timer0Value=64935;

			if(SlaveReceiveMasterTimeout >= 35) {	
				BatteryCells[BatNo].Voltage=0xff; //clear values
				BatteryCells[BatNo].PWM=0xff;
				clear_bit(BmsStatus0, BatNo);     //clear status bit
				NaslednjiPort();
			}
		}



}
void NaslednjiPort()
{
	//unsigned long LocalcellStatuses=cellStatuses[0];
	SlaveReceiveTimeout=0;
	SlaveReceiveMasterTimeout=0;
	

    BatNo++;                      // in gremo na novo baterijo
	if (BatNo >= 6){              // If we read thru all ports
		BatNo = 0;
		
		//check if all cells are full
		unsigned char countFull=0;
		unsigned char countROK=0;
		set_bit(BmsStatus0, 7); //we set unused but so if everithing is ok status byte0 is 0xFF
		BmsStatus1=0;  
		//loop through all cells
		for(char i=0; i<=5; i++){ 
			if(BatteryCells[i].PWM != 0){
				countFull++;
				set_bit(BmsStatus1, i); 
 			}
			if(BatteryCells[i].Voltage != 0xFF) countROK++;  
		}
		if(countFull == 6 ) set_bit(BmsStatus1, 0);
		
		if(countROK  == 6 ) set_bit(BmsStatus0, 6);
		else clear_bit(BmsStatus0, 6);
		//todo low & off

		

	}	
    rx_cnt = 0;
	softuart_flush_input_buffer();
}



void processReceivedByte(char Rx_Char){
	
	
	if ((Rx_Char == '>'))   // Èakaj na zaèetni znak ali limiter za status
	{
		rx_cnt = 0; //counter of received bytes
		BatChar[rx_cnt++] = '0'; 
	}
	else
	{
		if (rx_cnt > 0)        // If we allready received the first sign
		{    
			// if (Rx_Char == '\0' && V_BatChar[rx_cnt-1]==0x0D && V_BatChar[2]==0x2D) // if we received the whole message
			if (rx_cnt==7 && Rx_Char == 0x00 && BatChar[6]==0x0D && BatChar[2]==0x2D) // if we received the whole message
			{
				rx_cnt = 0; 
                        
				unsigned int V_BatTemp;  
				V_BatTemp=(((unsigned int)(BatChar[3]))<<8) + BatChar[4];				
				V_BatTemp=V_BatTemp-2000;
				V_BatTemp=((unsigned int)V_BatTemp)/10;

				BatteryCells[BatNo].Voltage=(char)V_BatTemp;				
				BatteryCells[BatNo].PWM=BatChar[5];
				set_bit(BmsStatus0, BatNo);

				//todo
				BatteryCells[BatNo].MaxBalancing=(char)Timer0Value;
				BatteryCells[BatNo].MaxVoltage=(char)(Timer0Value>>8);
				
				NaslednjiPort();

			} else
           	{                                       // Samo spravi sprejeti znak     
       			BatChar[rx_cnt++] = Rx_Char;      // in se postavi na naslednji prostor
            }
		}
	}
}
