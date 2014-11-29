#include <p18cxxx.h>
#include "BSW_GPIO.h"

void GpioConfig(){
	TRISBbits.TRISB0= 0x0;  //led1
	TRISBbits.TRISB1= 0x0;	//led2

}


