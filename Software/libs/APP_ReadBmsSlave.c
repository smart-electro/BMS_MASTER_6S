//next port - cell for 32 cel master option

/*
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


    }
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


    V_BatNo = 1;  // current battery to read from
    // memset(Bat_Tx_Str,'\0',sizeof(Bat_Tx_Str));
	// SestaviTxStr();

    
	BYTE Rx_Char;


	
	softuart_init();
	//softuart_turn_rx_on(); 
	

	     // Main application loop       

	while(1)
    {    
		//sw uart read data
		if ( softuart_kbhit() ) {
			Rx_Char = softuart_getchar();
			processReceivedByte(Rx_Char);

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






       
    } // Do this forever




*/