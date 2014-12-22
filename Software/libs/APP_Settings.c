#include <p18cxxx.h>
#include "APP_Settings.h"
#include "BSW_CAN.h"
#include "BSW_EEPROM.h"

#define CAN_SETTINGS_ID 0x12345678

enum CAN_RX_MSG_FLAGS CAN_RecFlags;
unsigned char CAN_Data[8];
unsigned long CAN_Identifier;
unsigned char CAN_Length;

void checkForCan(){
	// Check for CAN message
	if(CANIsRxReady()){
	
		CANReceiveMessage(&CAN_Identifier, &CAN_Data[0], &CAN_Length, &CAN_RecFlags);
	    if ( CAN_RecFlags & CAN_RX_OVERFLOW )
	    {
			// Rx overflow occurred; handle it
	    }
	    if ( CAN_RecFlags & CAN_RX_INVALID_MSG )
	    {
			// Invalid message received; handle it
	    }
	    if ( CAN_RecFlags & CAN_RX_XTD_FRAME )
	    {
			// Extended Identifier received; handle it
	    }
	    else
	    {
			// Standard Identifier received.
	    }
	    if ( CAN_RecFlags & CAN_RX_RTR_FRAME )
	    {
			// RTR frame received
	    }
	    else
	    {
			// Regular frame received.
			// V MasterA nekaj naredi z tem.
	    }
	
			// Extract receiver filter match, if it is to be used
	 		if (CAN_Identifier == CAN_SETTINGS_ID)
	 		{
				EepromWrite(EEPROM_MBS_ID, CAN_Data[6]);	    	
	     		// Reset();

	 		}
	}
}


void readSettings(){
	
	bms_id=EepromRead(EEPROM_MBS_ID);
	


}