#include "APP_ReadBmsSlave.h"
#include "APP_AuxFunctions.h"
#include "APP_ReadTemperature.h"
#include "BSW_Can.h"

unsigned char CAN_Data[8];
unsigned long CAN_Identifier;


/*******************************************/
/*******************************************/
void sendBmsStatus()
{
	// CAN data
	Cells_status.CELL1_ROK=1;
	Cells_status.CELL2_ROK=0;
	Cells_status.CELL3_ROK=1;
	Cells_status.CELL4_ROK=0;
	Cells_status.CELL5_ROK=1;
	Cells_status.CELL6_ROK=0;	
	Cells_status.ALL_ROK=1;

	#define CAN_lenghth1 5
	CAN_Data[0]=(unsigned char)&Cells_status;
	CAN_Data[1]=(unsigned char)&Cells_status; //todo
	CAN_Data[2]=TEMP1;
	CAN_Data[3]=TEMP2;
	CAN_Data[4]=TEMP3;
	CAN_Data[5]=TEMP4;

	
	// CAN adresses
	//todo read from eeprom
	CAN_Identifier=0x040050F3;
	
	//send message
	CANSendMessage(CAN_Identifier, &CAN_Data[0], CAN_lenghth1, CAN_TX_PRIORITY_0 & CAN_TX_XTD_FRAME & CAN_TX_NO_RTR_FRAME);
}


/*******************************************/
/*******************************************/
void sendCellVoltages()
{
	// CAN data
	#define CAN_lenghth2 6
	CAN_Data[0]=BatteryCells[0].Voltage;
	CAN_Data[1]=BatteryCells[1].Voltage;
	CAN_Data[2]=BatteryCells[2].Voltage;
	CAN_Data[3]=BatteryCells[3].Voltage;
	CAN_Data[4]=BatteryCells[4].Voltage;
	CAN_Data[5]=BatteryCells[5].Voltage;
	
	// CAN adresses
	//todo read from eeprom
	CAN_Identifier=0x040050F4;
	
	//send message
	CANSendMessage(CAN_Identifier, &CAN_Data[0], CAN_lenghth2, CAN_TX_PRIORITY_0 & CAN_TX_XTD_FRAME & CAN_TX_NO_RTR_FRAME);
}



/*******************************************/
/*******************************************/
void sendCellBalancingStatus()
{
	#define CAN_lenghth3 6
	CAN_Data[0]=BatteryCells[0].PWM;
	CAN_Data[1]=BatteryCells[1].PWM;
	CAN_Data[2]=BatteryCells[2].PWM;
	CAN_Data[3]=BatteryCells[3].PWM;
	CAN_Data[4]=BatteryCells[4].PWM;
	CAN_Data[5]=BatteryCells[5].PWM;
	
	// CAN adresses
	//todo read from eeprom
	CAN_Identifier=0x040050F5;
	
	//send message
	CANSendMessage(CAN_Identifier, &CAN_Data[0], CAN_lenghth3, CAN_TX_PRIORITY_0 & CAN_TX_XTD_FRAME & CAN_TX_NO_RTR_FRAME);
}



/*******************************************/
/*******************************************/
void sendCellMaxBalancing()
{
	// CAN data
	#define CAN_lenghth4 6
	CAN_Data[0]=BatteryCells[0].MaxBalancing;
	CAN_Data[1]=BatteryCells[1].MaxBalancing;
	CAN_Data[2]=BatteryCells[2].MaxBalancing;
	CAN_Data[3]=BatteryCells[3].MaxBalancing;
	CAN_Data[4]=BatteryCells[4].MaxBalancing;
	CAN_Data[5]=BatteryCells[5].MaxBalancing;
	
	// CAN adresses
	//todo read from eeprom
	CAN_Identifier=0x040050F6;
	
	//send message
	CANSendMessage(CAN_Identifier, &CAN_Data[0], CAN_lenghth4, CAN_TX_PRIORITY_0 & CAN_TX_XTD_FRAME & CAN_TX_NO_RTR_FRAME);
}



/*******************************************/
/*******************************************/
void sendCellMaxVoltage()
{
	// CAN data
	#define CAN_lenghth5 6
	CAN_Data[0]=BatteryCells[0].MaxVoltage;
	CAN_Data[1]=BatteryCells[1].MaxVoltage;
	CAN_Data[2]=BatteryCells[2].MaxVoltage;
	CAN_Data[3]=BatteryCells[3].MaxVoltage;
	CAN_Data[4]=BatteryCells[4].MaxVoltage;
	CAN_Data[5]=BatteryCells[5].MaxVoltage;
	
	// CAN adresses
	//todo read from eeprom
	CAN_Identifier=0x040050F3;
	
	//send message
	CANSendMessage(CAN_Identifier, &CAN_Data[0], CAN_lenghth5, CAN_TX_PRIORITY_0 & CAN_TX_XTD_FRAME & CAN_TX_NO_RTR_FRAME);
}




/*******************************************/
/*******************************************/
void sendCellMinVoltage()
{
	// CAN data
	#define CAN_lenghth6 6
	CAN_Data[0]=BatteryCells[0].MinVoltage;
	CAN_Data[1]=BatteryCells[1].MinVoltage;
	CAN_Data[2]=BatteryCells[2].MinVoltage;
	CAN_Data[3]=BatteryCells[3].MinVoltage;
	CAN_Data[4]=BatteryCells[4].MinVoltage;
	CAN_Data[5]=BatteryCells[5].MinVoltage;
	
	
	// CAN adresses
	//todo read from eeprom
	CAN_Identifier=0x040050F7;
	
	//send message
	CANSendMessage(CAN_Identifier, &CAN_Data[0], CAN_lenghth6, CAN_TX_PRIORITY_0 & CAN_TX_XTD_FRAME & CAN_TX_NO_RTR_FRAME);
}


/*
			    Can_addrLO.Bytes[2] = j;
				Can_addrLO.Bytes[3] = MASTER_NUMBER;
           		CANSendMessage(Can_addrLO.SE_ID, &tx_CAN_msg.Data[0], 8, CAN_TX_PRIORITY_0 & CAN_TX_XTD_FRAME & CAN_TX_NO_RTR_FRAME);
*/