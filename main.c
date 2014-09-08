#include <xc.h>
//#include "config.h"
#include <plib/usart.h>

#define _XTAL_FREQ 8000000 //The speed of your internal(or)external oscillator
#define USE_AND_MASKS

int i = 0;
unsigned char UART1Config = 0, baud = 0;
unsigned char MsgFromPIC[] = "PIC Rocks\r";

void SetupClock(void);
void Delay1Second(void);

void main(int argc, char** argv) {

    SetupClock(); // Internal Clock to 8MHz

    TRISCbits.RC6 = 0; //TX pin set as output
    TRISCbits.RC7 = 1; //RX pin set as input

    UART1Config = USART_TX_INT_OFF & USART_RX_INT_OFF & USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_BRGH_HIGH ;
    baud = 51;
    OpenUSART(UART1Config,baud);


    while(1) //infinite loop
    {
       putsUSART(MsgFromPIC);
       Delay1Second();
    }
}

void SetupClock()
{
    OSCCONbits.IRCF0 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF2 = 1;
}

void Delay1Second()
{
    for(i=0;i<100;i++)
    {
         __delay_ms(10);
    }
}