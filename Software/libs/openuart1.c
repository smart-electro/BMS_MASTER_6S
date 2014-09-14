#include <p18cxxx.h>
#include "sw_uart1.h"
#define SWTXD         PORTAbits           // Transmit pin port and pin
#define SWTXDpin      RA0
#define TRIS_SWTXD    TRISAbits           // Transmit pin tris and pin
#define TRISbit_SWTXD TRISA0
#define SWRXD         PORTAbits           // Receive pin port and pin
#define SWRXDpin      RA1
#define TRIS_SWRXD    TRISAbits           // Receive pin tris and pin
#define TRISbit_SWRXD TRISA1

/* The following extern declared symbols needs to be defined by user prior 
 * to using UART functions.
 */

extern  void DelayRXHalfBitUART(void);
extern  void DelayRXBitUART(void);
extern  void DelayTXBitUART(void);
extern  char uartdata;
extern  char BitCount;

/********************************************************************
;*      Function Name:  OpenUART                                    *
;*      Return Value:   void                                        *
;*      Parameters:     void                                        *
;*      Description:    This routine configures the I/O pins for    *
;*                      software UART.                              *
;*******************************************************************/

void OpenUART_1(void)
{
ADCON0 = 0x00; //disable A/D converter 
ADCON1 = 0x0F; //select all digital 

//TRISA = 0x00; //set only RA2 as input
 //  PORTA = 0; 

    TRIS_SWTXD.TRISbit_SWTXD = 0x0;
    SWTXD.SWTXDpin           = 0x1; 

    TRIS_SWRXD.TRISbit_SWRXD = 0x1; //changed from 0
    SWRXD.SWRXDpin           = 0x1;
PORTAbits.RA0=1;


}


