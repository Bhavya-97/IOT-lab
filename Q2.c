#include<stdio.h>
#include "Driver\DrvGPIO.h"
#include "Driver\DrvUART.h"
#include "Driver\DrvSYS.h"
#include "LCD_Driver.h"

int32_t main()
{
	char TEXT[16];
	int32_t a;

	UNLOCKREG();  //present in setting up clock for pwm (last page of file)
	SYSCLK->PWRCON.XTL12M_EN = 1;
	DrvSYS_Delay(5000);  //this is not present in the file
	SYSCLK->CLKSEL0.HCLK_S = 0;
	LOCKREG();

	DrvGPIO_SetPortBits(E_GPA,15);
	a=DrvGPIO_GetPortBits(E_GPA);

	Initial_panel();
	clr_all_panel();

	sprintf(TEXT,"port :: %x",a);//for decimal --> %d
	print_lcd(0,TEXT);
}
