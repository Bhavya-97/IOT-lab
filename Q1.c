#include <stdio.h>
#include "NUC1xx.h"
#include "Driver\DrvGPIO.h"
#include "Driver\DrvUART.h"
#include "Driver\DrvSYS.h"

void Init_LED() // Initialize GPIO pins
{
    DrvGPIO_Open(E_GPC, 15, E_IO_OUTPUT); // GPC12 pin set to output mode
    DrvGPIO_SetBit(E_GPC, 15);            // Goutput Hi to turn off LED
}

void EINT0Led_CALLBACK(void)
{
    DrvGPIO_ClrBit(E_GPC, 15); //turns on LED
    DrvSYS_Delay(300000);
    DrvGPIO_SetBit(E_GPC, 15); //turns off LED
    DrvSYS_Delay(300000);
}

void EINT1Callback(void)
{
    DrvGPIO_ClrBit(E_GPB,11);  //turns on Buzzer
    DrvSYS_Delay(100000);
    DrvGPIO_SetBit(E_GPB,11);  //turns off Buzzer
    DrvSYS_Delay(100000);
}

int main (void)
{
	UNLOCKREG();
  DrvSYS_Open(48000000);
  LOCKREG();

  Init_LED();
  DrvGPIO_Open(E_GPB, 14, E_IO_INPUT); //for LED
  DrvGPIO_EnableEINT0(E_IO_RISING, E_MODE_EDGE, EINT0Led_CALLBACK);     //GPIO port E_GPB, pin 14

  DrvGPIO_Open(E_GPB, 11, E_IO_OUTPUT); //for buzzer
  DrvGPIO_Open(E_GPB, 15, E_IO_INPUT);  // configure external interrupt pin GPB15

  DrvGPIO_EnableEINT1(E_IO_BOTH_EDGE, E_MODE_EDGE, EINT1Callback);

  while(1)
    {
    }
}
