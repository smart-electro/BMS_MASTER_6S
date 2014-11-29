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
#include <stdio.h>
#include <string.h>
#include <timers.h> 
#include <usart.h>
#include <eep.h>
#include <spi.h>
#include "softuart.h"


#include "BMS_6s.h"


int Timer100ms=360; //1200*3/10

//next port - cell for 32 cel master option
void NaslednjiPort()
{
    char j;
	unsigned long LocalcellStatuses=cellStatuses[0];

	V_BatNo++;                      // in gremo na novo baterijo
    rx_cnt = 0;
	softuart_flush_input_buffer();

	

//tx_CAN_msg.Data[0]=V_BatNo;
//CANSendMessage(tx_CAN_msg.Address.SE_ID, &tx_CAN_msg.Data[0], 8, CAN_TX_PRIORITY_0 & CAN_TX_XTD_FRAME & CAN_TX_NO_RTR_FRAME);


	if (V_BatNo == 6){              // If we read thru all ports
		j=0;

		for(char i=0;i<=4; i+=4){       //four cells per message
			if ( CANIsTxReady() )      // Sestavi CAN telegram 1 za MasterA.
        	{			
		        //memset(Bat_Tx_Str,'\0',sizeof(Bat_Tx_Str)); // Bruši USART buffer
		        //SestaviTxStr();            // Saestavi USART telegram za vse baterije
		        //tx_send = 1;               // FLAG za pošiljanje

    		/*	tx_CAN_msg.Data[0] = (V_Bat[i] >> 8);
    			tx_CAN_msg.Data[1] = (V_Bat[i] & 0xFF);
			    tx_CAN_msg.Data[2] = (V_Bat[i+1] >> 8);
			    tx_CAN_msg.Data[3] = (V_Bat[i+1] & 0xFF);
			    tx_CAN_msg.Data[4] = (V_Bat[i+2] >> 8);
			    tx_CAN_msg.Data[5] = (V_Bat[i+2] & 0xFF);
			    tx_CAN_msg.Data[6] = (V_Bat[i+3] >> 8);
			    tx_CAN_msg.Data[7] = (V_Bat[i+3] & 0xFF);
			    */

				tx_CAN_msg.Data[0] = V_BatL[i];
    			tx_CAN_msg.Data[1] = V_BatH[i];
			    tx_CAN_msg.Data[2] = V_BatL[i+1];
			    tx_CAN_msg.Data[3] = V_BatH[i+1];
			    tx_CAN_msg.Data[4] = V_BatL[i+2];
			    tx_CAN_msg.Data[5] = V_BatH[i+2];
			    tx_CAN_msg.Data[6] = V_BatL[i+3];
			    tx_CAN_msg.Data[7] = V_BatH[i+3];

				// CAN adresses
			    //node_no = (Read_b_eep(0) << 2);
			    //Can_addrLO.SE_ID = 0x040050F3;
			    //Can_addrLO.Bytes[2] = Can_addrLO.Bytes[2] | node_no;
			    //Can_addrHI.SE_ID = 0x040150F3;
			    //Can_addrHI.Bytes[2] = Can_addrHI.Bytes[2] | node_no;
			    Can_addrLO.SE_ID = 0x040050F3;
			    Can_addrLO.Bytes[2] = j;
				Can_addrLO.Bytes[3] = MASTER_NUMBER;
			    tx_CAN_msg.Length.Len = 8;
           		CANSendMessage(Can_addrLO.SE_ID, &tx_CAN_msg.Data[0], 8, CAN_TX_PRIORITY_0 & CAN_TX_XTD_FRAME & CAN_TX_NO_RTR_FRAME);
 
		  		j++;
		        V_BatNo = 0;               // Postavi se na prvo baterijo
				LATD=0;
				_delay(1000);
			}
		}      
   		//status message
		//long is dysplaied in reverse order
		//Example: FF FF FF FE - the first battery is not ballancing all others are
	/*	Can_addrLO.Bytes[1] = 1;
		tx_CAN_msg.Data[0] = ((LocalcellStatuses & 0xFF000000L) >> 24);
    	tx_CAN_msg.Data[1] = ((LocalcellStatuses & 0x00FF0000L) >> 16);
		tx_CAN_msg.Data[2] = ((LocalcellStatuses & 0x0000FF00L) >> 8);
	    tx_CAN_msg.Data[3] = (LocalcellStatuses & 0x000000FFL);
		//configuration bits 
		tx_CAN_msg.Data[4] = ((connectedCells & 0xFF000000L) >> 24);
    	tx_CAN_msg.Data[5] = ((connectedCells & 0x00FF0000L) >> 16);
		tx_CAN_msg.Data[6] = ((connectedCells & 0x0000FF00L) >> 8);
	    tx_CAN_msg.Data[7] = (connectedCells & 0x000000FFL);		
		CANSendMessage(Can_addrLO.SE_ID, &tx_CAN_msg.Data[0], 8, CAN_TX_PRIORITY_0 & CAN_TX_XTD_FRAME & CAN_TX_NO_RTR_FRAME);
 			   
*/

    }
}
/*********************************************************/
/*              auxiliary functions                      */
/*********************************************************/
int hexCharToInt(BYTE digit)
{
	if('0'<=digit && digit<='9')
		return digit-'0';
	if('A'<=digit && digit<='F')
		return digit-'A'+10;
	if('a'<=digit && digit<='f')
		return digit-'a'+10;
	return -1;
}

int hexToInt(BYTE *h)
{
	int i, result=0;
	int tmp;
	
	for(i=0;h[i] && i<8;i++)
    {
		tmp = hexCharToInt(h[i]);
		if(tmp == -1) return result;
		result = result*0x10+tmp;
	}
	return result;
} 
/*********************************************************/
/*                   Main function                       */
/*********************************************************/
void main(void)
{

	//TRISD = 0x00; // PortD output
    //PORTD = 0xFF;
    //LATD  = 0x00;
	OSCCON = 0b01111111;

	ADCON0 = 0x00; //disable A/D converter 
	ADCON1 = 0x0F; //select all digital 
    //TRISAbits.TRISA0 = 0x0; //changed from 0
    //PORTAbits.RA0           = 0x1;
	//PORTAbits.RA0=1;

    // Initialize TIMER0
    INTCON2bits.TMR0IP = 0;        //Timer0 INT-LOW
    //OpenTimer0(TIMER_INT_ON & T0_16BIT & T0_SOURCE_INT & T0_PS_1_64);
    OpenTimer0(TIMER_INT_ON & T0_16BIT & T0_SOURCE_INT & T0_PS_1_1);
    //WriteTimer0(62411);            // 1/10Sec interval --  8Mhz/4/64 := 3125 -> 65536 - 3125 = 62411
	//for SW uart we need 1/3 baud = 1200/3=400
	WriteTimer0(64981);
    INTCONbits.GIE = 1;            //enable global interrupts
	//Timers


    // Initialize CAN module with no message filtering
    // CANInitialize(SJW, BRP, PHSEG1, PHSEG2, PROPSEG, config);
    // 8MHz Fosc 250Kb/s
    // 8MHz -> 2MHz @ 250Khz = 8Tq (1+2+3+2)
    CANInitialize(1 ,0x02 ,2 ,3 ,2 , CAN_CONFIG_VALID_XTD_MSG); //256kb at 8Mhz crystal
    // CANInitialize(1 ,0x06 ,2 ,3 ,2 , CAN_CONFIG_VALID_XTD_MSG); USB-CAN
    CANSetOperationMode(CAN_OP_MODE_CONFIG);
    CANSetFilter(CAN_FILTER_B1_F1, Can_bootF.SE_ID, CAN_CONFIG_XTD_MSG);
    CANSetFilter(CAN_FILTER_B1_F2, Can_nodeF.SE_ID, CAN_CONFIG_XTD_MSG);
    CANSetMask(CAN_MASK_B1, Can_Mask1.SE_ID, CAN_CONFIG_XTD_MSG);
    CANSetFilter(CAN_FILTER_B2_F1, Can_addrLO.SE_ID, CAN_CONFIG_XTD_MSG);
    CANSetMask(CAN_MASK_B2, Can_Mask2.SE_ID, CAN_CONFIG_XTD_MSG);
    CANSetOperationMode(CAN_OP_MODE_NORMAL);



/*	// Initialize USART
	// (8MHz / 16 / 9600) -1 = 9615,3846153846153846153846153846Boud
	// (8MHz / 16 / 1200) -1 = 1199,0407673860911270983213429257Boud
    OpenUSART( USART_TX_INT_OFF & USART_RX_INT_OFF & USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_CONT_RX & USART_BRGH_LOW, 104);
	// OpenUSART( USART_TX_INT_OFF & USART_RX_INT_OFF & USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_CONT_RX & USART_BRGH_HIGH, 52);
    tx_cnt = 0;
    tx_send = 0;
    rx_cnt = 0;
*/

    V_BatNo = 1;  // current battery to read from
    // memset(Bat_Tx_Str,'\0',sizeof(Bat_Tx_Str));
	// SestaviTxStr();

    WDTCONbits.SWDTEN = 1;  // Enable Watch Dog
	BYTE Rx_Char;

	///DEBUG SECTION
	//detect reset - SEND CAN msg
	Can_addrLO.Bytes[3] = MASTER_NUMBER;
	Can_addrLO.Bytes[2] = RCON;
   	CANSendMessage(Can_addrLO.SE_ID, &tx_CAN_msg.Data[0], 8, CAN_TX_PRIORITY_0 & CAN_TX_XTD_FRAME & CAN_TX_NO_RTR_FRAME);
	char MSG_SAUD[] = {"Ola\n\rPIC:\\>"};
	unsigned char i = 0;
	softuart_puts_p( "12345678" );    // "implicit" PSTR
	
	softuart_init();
	//softuart_turn_rx_on(); /* redundant - on by default */
	
	/*************************************/
	/*      Main application loop        */
    /*************************************/
	while(1)
    {    
		//sw uart read data
		if ( softuart_kbhit() ) {
			Rx_Char = softuart_getchar();
			processReceivedByte(Rx_Char);
			/*tx_CAN_msg.Data[0] =softuart_getchar(); 
			tx_CAN_msg.Data[1] =softuart_getchar(); 
			tx_CAN_msg.Data[2] =softuart_getchar(); 
			tx_CAN_msg.Data[3] =softuart_getchar(); 
			tx_CAN_msg.Data[4] =softuart_getchar(); 
			tx_CAN_msg.Data[5] =softuart_getchar(); 
			tx_CAN_msg.Data[6] =softuart_getchar(); 
			tx_CAN_msg.Data[7] =softuart_getchar(); 
			Can_addrLO.SE_ID = 0x040050A2;
	   		CANSendMessage(Can_addrLO.SE_ID, &tx_CAN_msg.Data[0], 8, CAN_TX_PRIORITY_0 & CAN_TX_XTD_FRAME & CAN_TX_NO_RTR_FRAME);
*/
		}
		//1sec timer for sending out the CAN massages
		if (Timer100ms==3600){ //360==100ms
			Timer100ms=0;

			Can_addrLO.SE_ID = 0x040050A0;
	   		CANSendMessage(Can_addrLO.SE_ID, &tx_CAN_msg.Data[0], 8, CAN_TX_PRIORITY_0 & CAN_TX_XTD_FRAME & CAN_TX_NO_RTR_FRAME);
		}
		//receive timeout
		if(rx_tic >= 3600) {
			rx_tic=0;
			//NaslednjiPort();
		}

/*			//FRAME RECEIVED ON UART
			tx_data[0] = '>';  3E
			tx_data[1] = Bat_status; 
			tx_data[2] = '-'; 2D
			tx_data[3] = to_hex(ADC_result >> 8);
			tx_data[4] = to_heex(ADC_rsult >> 4);
			tx_data[5] = to_hex(ADC_result & 0x000F);
			tx_data[6] = '\r';  A
			tx_data[7] = '\0';  D
			softuart_puts(tx_data);
*/





        ClrWdt();
    } // Do this forever
}
void processReceivedByte(char Rx_Char){
			rx_tic = 0;                // Resetiraj TIME-OUT števec

					
            //if ((Rx_Char == '>') || (Rx_Char == '-'))   // Èakaj na zaèetni znak ali limiter za status
            if ((Rx_Char == '>'))   // Èakaj na zaèetni znak ali limiter za status
            {
                 rx_cnt = 0; //counter of received bytes
                 V_BatChar[rx_cnt++] = '0'; 
            }
            else
            {
 				if (rx_cnt > 0)        // If we allready received the first sign
                {    
                   // if (Rx_Char == '\0' && V_BatChar[rx_cnt-1]==0x0D && V_BatChar[2]==0x2D) // if we received the whole message
                    if (Rx_Char == 0x0D && V_BatChar[2]==0x2D) // if we received the whole message
                    {
  						rx_cnt = 0; 
						//V_BatChar[rx_cnt++] = '\0';
                        
						V_BatTemp = hexToInt(&V_BatChar[0]); // Pretvori v vrednost
                        //if (V_BatTemp > 0)                   // Ali je vrednost smiselna
                        //{
                            //V_Bat[V_BatNo] = V_BatTemp;      // Zato jo zapiši
                            //V_Bat[V_BatNo] =0xFF;
							
							//batt statuses
							if(V_BatChar[1]==0x4F) set_bit(cellStatuses[0], V_BatNo); //ballencing 0x4F
							else clear_bit(cellStatuses[0], V_BatNo);
							
							V_BatL[V_BatNo] = V_BatChar[3];	
							V_BatH[V_BatNo] = V_BatChar[4];

tx_CAN_msg.Data[0] =V_BatChar[0];
tx_CAN_msg.Data[1] =V_BatChar[1];
tx_CAN_msg.Data[2] =V_BatChar[2];
tx_CAN_msg.Data[3] =V_BatChar[3];
tx_CAN_msg.Data[4] =V_BatChar[4];
tx_CAN_msg.Data[5] =V_BatChar[5];
tx_CAN_msg.Data[6] =V_BatChar[6];
tx_CAN_msg.Data[7] =V_BatChar[7];
Can_addrLO.SE_ID = 0x040050A1;
CANSendMessage(Can_addrLO.SE_ID, &tx_CAN_msg.Data[0], 8, CAN_TX_PRIORITY_0 & CAN_TX_XTD_FRAME & CAN_TX_NO_RTR_FRAME);


						//	NaslednjiPort();

                        //} 
                        //memset(V_BatChar,'\0',sizeof(V_BatChar)); //discard if anything left in UART
                    } else
                    {                                       // Samo spravi sprejeti znak     
                        V_BatChar[rx_cnt++] = Rx_Char;      // in se postavi na naslednji prostor
                    }
                }
            }
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
    unsigned int t1;
    unsigned char mdisp;
    //if(PIR1bits.TMR1IF)
    //{
   //     PIR1bits.TMR1IF=0;                    //clear interrupt flag
    //}
    //if(INTCONbits.TMR0IF)                   //check for TMR0 overflow interrupt flag
    //{
       // unsigned int t0;
        INTCONbits.TMR0IF = 0;              //clear interrupt flag
        WriteTimer0(65035);             //Nastavi na 1/10.0 Sek
        rx_tic++;
		Timer100ms++;
		interruptCAllback();
    //}
}

// Low priority interrupt vector
/*#pragma code InterruptVectorLow=0x418
void InterruptVectorLow (void)
{
      _asm
          GOTO InterruptHandlerLow  //jump to interrupt routine
      _endasm
}

#pragma code
#pragma interrupt InterruptHandlerLow //save=disp0,disp1,disp2,disp3,TBLPTRU

void InterruptHandlerLow (void)
{
    if(INTCONbits.TMR0IF)                   //check for TMR0 overflow interrupt flag
    {
        unsigned int t0;
        INTCONbits.TMR0IF = 0;              //clear interrupt flag
        t0=ReadTimer0();
        WriteTimer0(64981);             //Nastavi na 1/10.0 Sek
        rx_tic++;
		Timer100ms++;
		PORTAbits.RA0 = PORTAbits.RA0 ^ 1;
		
    }
}*/
// End of program

