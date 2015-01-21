#include "BSW_SoftUART.h"
#include "APP_ReadBmsSlave.h"
#include "APP_AuxFunctions.h"
#include "BSW_GPIO.h"
#include <p18cxxx.h>
#include "BSW_CAN.h"
#include "BSW_EEPROM.h"
#include "RTOS.h"
unsigned char rx_cnt=0;
unsigned char crc8=0;
unsigned char BatChar[8];
				unsigned int ADC_value, ADC_Ref;  
				float V_BatTemp;  

void processReceivedByte(char Rx_Char);
void NaslednjiPort();
unsigned char crc_ibutton_update(unsigned char crc8,unsigned char datain);

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
			if(BatteryCells[i].PWM != 0 && BatteryCells[i].PWM != 0xFF){ // todo look for status
				countFull++;
				set_bit(BmsStatus1, 1); //one full flag 
 			}
			if(BatteryCells[i].Voltage != 0xFF) countROK++;  
		}
		if(countFull == 6 ) set_bit(BmsStatus1, 0); //all full flag
		
		if(countROK  == 6 ) set_bit(BmsStatus0, 6); 
		else clear_bit(BmsStatus0, 6);
		//todo low & off

		

	}	
    rx_cnt = 0;
	softuart_flush_input_buffer();
}

unsigned char crc_ibutton_update(unsigned char crc8,unsigned char datain)
{
  unsigned char i;

  crc8 = crc8 ^ datain;
  for (i = 0; i < 8; i++)
    {
      if (crc8 & 0x01)
    	crc8 = (crc8 >> 1) ^ 0x8C;
      else
    	crc8 >>= 1;
    }
  return crc8;
}

void processReceivedByte(char Rx_Char){
	
	
	if ((Rx_Char == '>'))   // Èakaj na zaèetni znak ali limiter za status
	{
		rx_cnt = 0; //counter of received bytes
//		BatChar[rx_cnt++] = '0'; 
		BatChar[rx_cnt++] = Rx_Char; 
	}
	else
	{
		if (rx_cnt > 0)        // If we allready received the first sign
		{    
			if (rx_cnt==7 && BatChar[7]==0xEE) // if we received the whole message
			{
				rx_cnt = 0; 
				
				//crc carculation
				crc8=0;
				//BatChar[0]= '>';
				for (unsigned char i=0; i<=5; i++)
			  	{
			    	crc8 = crc_ibutton_update(crc8, BatChar[i]);
			  	}
				if(crc8==BatChar[6]){

					/*	
					softuart_putchar('>');
					softuart_putchar((ADC_result & 0xFF00L) >> 8);   // Apply the lower middle byte of the data 
					softuart_putchar((ADC_result & 0x00FFL));      // Apply the low byte of the data 
					softuart_putchar((RefVolt & 0xFF00L) >> 8);   // Apply the lower middle byte of the data
					softuart_putchar((RefVolt & 0x00FFL));      // Apply the low byte of the data
					softuart_putchar(PWM);
					softuart_putchar(crc8);*/
	
					ADC_value=(((unsigned int)(BatChar[1]))<<8) + BatChar[2];
					ADC_Ref=(((unsigned int)(BatChar[3]))<<8) + BatChar[4];
					V_BatTemp = (float)ADC_value * (float)420;
					V_BatTemp /= ADC_Ref;	
					V_BatTemp -= (float)200;
					BatteryCells[BatNo].Voltage=(char)V_BatTemp;
	                
					//Cell voltage calcualtion        
					/*unsigned int V_BatTemp;  
					V_BatTemp=(((unsigned int)(BatChar[3]))<<8) + BatChar[4];			
					V_BatTemp=V_BatTemp-2000;
					V_BatTemp=((unsigned int)V_BatTemp)/10;
					BatteryCells[BatNo].Voltage=(char)V_BatTemp;	*/				
					
					//pwm
					BatteryCells[BatNo].PWM=BatChar[5];
					set_bit(BmsStatus0, BatNo);
	
					//Cell max voltage checking
					if((BatteryCells[BatNo].Voltage > BatteryCells[BatNo].MaxVoltage) || (BatteryCells[BatNo].MaxVoltage==0xFF)){
						BatteryCells[BatNo].MaxVoltage=BatteryCells[BatNo].Voltage;
						EepromWrite(EEPROM_MAX_VOLTAGE+BatNo, BatteryCells[BatNo].MaxVoltage);
					}
					//Cell min voltage checking
					if((BatteryCells[BatNo].Voltage < BatteryCells[BatNo].MinVoltage) || (BatteryCells[BatNo].MinVoltage==0xFF)){
						BatteryCells[BatNo].MinVoltage=BatteryCells[BatNo].Voltage;
						EepromWrite(EEPROM_MIN_VOLTAGE+BatNo, BatteryCells[BatNo].MinVoltage);
					}				
					//Cell max Balancing checking
					if((BatteryCells[BatNo].PWM > BatteryCells[BatNo].MaxBalancing) || (BatteryCells[BatNo].MaxBalancing==0xFF)){
						BatteryCells[BatNo].MaxBalancing=BatteryCells[BatNo].PWM;
						EepromWrite(EEPROM_MAX_PWM+BatNo, BatteryCells[BatNo].MaxBalancing);
					}
	
		
				
				
					NaslednjiPort();
				}
			} else
           	{                                       // Samo spravi sprejeti znak     
       			BatChar[rx_cnt++] = Rx_Char;      // in se postavi na naslednji prostor
            }
		}
	}
}
