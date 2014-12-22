#include "BSW_ReadADC.h"


int ADC_sample[ADC_ChanalNo];
unsigned char ADC_Chanels[ADC_ChanalNo] = {ADC_CH0, ADC_CH1, ADC_CH2, ADC_CH3};
unsigned char ADC_ChanelCnt = 0;
unsigned char ADC_SampleCnt = 0;
unsigned char ADC_OK = 0;

void ADC_Init(void)
{
// Initialize ADC module
    ADC_ChanelCnt = 0;
    ADC_SampleCnt = 0;
    OpenADC(ADC_FOSC_32 & ADC_RIGHT_JUST & ADC_8_TAD, ADC_CH0 & ADC_INT_OFF & ADC_REF_VDD_VSS, ADC_4ANA);
//    ConvertADC();    
}


void ADC_Data(void)
{
    unsigned char cnt =0;
    ADC_sample[ADC_ChanelCnt] +=  ReadADC();
    ADC_ChanelCnt++;
    if (ADC_ChanelCnt >= ADC_ChanalNo)
    {
        ADC_ChanelCnt = 0;
        ADC_SampleCnt++;
        if (ADC_SampleCnt >= ADC_SampleNo)
        {
            ADC_SampleCnt = 0;
            for(cnt=0; cnt<ADC_ChanalNo; cnt++)
            {
                ADC_result[cnt] = ADC_sample[cnt];
            } 
            ADC_ChanelCnt = 0;
            ADC_SampleCnt = 0;
            ADC_OK = 1;
        } 
    }
    if (ADC_OK ==0) SelChanConvADC(ADC_Chanels[ADC_ChanelCnt]);
}
