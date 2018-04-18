#include <stdio.h>
#include "NUC1xx.h"
#include "Driver\DrvSYS.h"
#include "Seven_Segment.h"
void InitADC(void)
{
	/* Step 1. GPIO initial */
	//Should be 0x00010000
	GPIOA->OFFD|=0x00010000; //Disable digital input path
	SYS->GPAMFP.ADC7_SS21_AD6=1; //Set ADC function

	/* Step 2. Enable and Select ADC clock source, and then enable ADC module */
	SYSCLK->CLKSEL1.ADC_S = 2; //Select 22Mhz for ADC
	SYSCLK->CLKDIV.ADC_N = 1; //ADC clock source = 22Mhz/2 =11Mhz;
	SYSCLK->APBCLK.ADC_EN = 1; //Enable clock source
	ADC->ADCR.ADEN = 1; //Enable ADC module

	/* Step 3. Select Operation mode */
	ADC->ADCR.DIFFEN = 0; //single end input
	ADC->ADCR.ADMD = 0; //single mode
	//Should be 0x01(In Q4 0x40)

	/* Step 4. Select ADC channel 0*/
	ADC->ADCHER.CHEN = 0x01;

	/* Step 5. Enable ADC interrupt */
	ADC->ADSR.ADF =1; //clear the A/D interrupt flags for safe
	ADC->ADCR.ADIE = 1;
	// NVIC_EnableIRQ(ADC_IRQn);

	/* Step 6. Enable WDT module */
	ADC->ADCR.ADST=1;
}
void seg_display(int16_t value)
{
	int8_t digit;
	digit = value / 1000;
	close_seven_segment();
	show_seven_segment(3,digit);
	DrvSYS_Delay(5000);

	value = value - digit * 1000;
	digit = value / 100;
	close_seven_segment();
	show_seven_segment(2,digit);
	DrvSYS_Delay(5000);

	value = value - digit * 100;
	digit = value / 10;
	close_seven_segment();
	show_seven_segment(1,digit);
	DrvSYS_Delay(5000);

	value = value - digit * 10;
	digit = value;
	close_seven_segment();
	show_seven_segment(0,digit);
	DrvSYS_Delay(5000);
}
int32_t main (void)
{
	int32_t adc_value;
	UNLOCKREG();
	SYSCLK->PWRCON.XTL12M_EN = 1; //Enable 12Mhz and set HCLK->12Mhz
	SYSCLK->CLKSEL0.HCLK_S = 0;
	LOCKREG();
	InitADC();
	while(1)
	{
		while(ADC->ADSR.ADF==0); // ADC Flag, wait till 1 (A/DC conversion done)
		ADC->ADSR.ADF=1; // write 1 to ADF is to clear the flag

		adc_value=ADC->ADDR[0].RSLT; // input 12-bit ADC value
		seg_display(adc_value); // display value to 7-segment display
		ADC->ADCR.ADST=1;       //from step 6
	}
}
