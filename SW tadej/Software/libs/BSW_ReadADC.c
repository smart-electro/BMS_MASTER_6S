#include "BSW_ReadADC.h"

int ADC_result[ADC_ChanalNo];
int ADC_sample[ADC_ChanalNo];
unsigned char ADC_Chanels[ADC_ChanalNo] = {ADC_CH0, ADC_CH1, ADC_CH2, ADC_CH3};
unsigned char ADC_ChanelCnt = 0;
unsigned char ADC_SampleCnt = 0;
unsigned char ADC_OK = 0;

void ADC_Data(void)
{
    unsigned char tp_cnt =0;
    ADC_sample[ADC_ChanelCnt] +=  ReadADC();
    ADC_ChanelCnt++;
    if (ADC_ChanelCnt >= ADC_ChanalNo)
    {
        ADC_ChanelCnt = 0;
        ADC_SampleCnt++;
        if (ADC_SampleCnt >= ADC_SampleNo)
        {
            ADC_SampleCnt = 0;
            for(tp_cnt=0; tp_cnt<ADC_ChanalNo; tp_cnt++)
            {
                ADC_result[tp_cnt] = ADC_sample[tp_cnt];
            } 
            ADC_ChanelCnt = 0;
            ADC_SampleCnt = 0;
            ADC_OK = 1;
        } 
    }
    if (ADC_OK ==0) SelChanConvADC(ADC_Chanels[ADC_ChanelCnt]);
}
