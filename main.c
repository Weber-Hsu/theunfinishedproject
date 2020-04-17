/*
 * main.c
 *
 *  Created on: Apr 6, 2020
 *      Author: shangweih
 */

/* Included file */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
//#include <file.h>

#include "DSP28x_Project.h"

/* Defines */

#define CONV_WAIT 1L // micro-seconds to wait for ADC conversion.
extern void DSP28x_usDelay(Uint32 Count);

static unsigned short indexX=0;
static unsigned short indexY=0;

//const unsigned char pucTempStr[] = 'Current Temperature:';

int16_t refTemp;
int16_t curTemp;
int16_t tempDelta;


/* cpu_timer0_isr */
void cpu_timer0_isr(void)
{
    //
    // Acknowledge this interrupt to receive more interrupts from group 1
    //
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

/* Main */
void main(void)
{
	volatile int status = 0;
	uint16_t i;
    //
    // If running from flash copy RAM only functions to RAM
    //
	#ifdef _FLASH
		memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (size_t)&RamfuncsLoadSize);
	#endif

	/* Initialize System Control:
	 * PLL/ WatchDog/ Enable Peripheral Clocks */
	InitSysCtrl();

	/* Initialize GPIO: */
//	InitGpio();
	InitSciaGpio();

	/* Clear all interrupts and initialize PIE vector table */
	DINT; // Disable CPU interrupts

	/* Initialize PIE control registers to their default state. */
	InitPieCtrl();

	/* Disable CPU interrupts and clear all interrupt flags */
	IER = 0x0000;
	IFR = 0x0000;

	/* Initialize PIE vector table to the shell ISR (Interrupt Service Routine) */
	InitPieVectTable();

	EALLOW;
	PieVectTable.TINT0 = &cpu_timer0_isr;
	EDIS;

	/* Initialize SCIA */
//	scia_init();

	/* Initialize GPIOs for the LEDs and turn them off */
	EALLOW;
    GpioCtrlRegs.GPBDIR.bit.GPIO34 = 1;
    GpioCtrlRegs.GPBDIR.bit.GPIO39 = 1;
    GpioDataRegs.GPBDAT.bit.GPIO34 = 1;
    GpioDataRegs.GPBDAT.bit.GPIO39 = 1;
    EDIS;

    InitCpuTimers(); // for this example
    ConfigCpuTimer(&CpuTimer0,90,1000);
    CpuTimer0Regs.TCR.all = 0x4010; // Use write-only instruction to set TSS bit = 1
    IER |= M_INT1;
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;

    /* Enable global interrputs and higher priority real-time debug events */
    EINT;// Enable Global interrupt INTM
    ERTM;// Enable Global realtime interrupt DBGM

    /* Initialize the ADC */
    InitAdc();

    EALLOW;
    AdcRegs.ADCCTL2.bit.ADCNONOVERLAP = 1;	//Enable non-overlap mode

    // Connect channel A5 internally to the temperature sensor
    AdcRegs.ADCCTL1.bit.TEMPCONV  = 1;
    AdcRegs.ADCSOC0CTL.bit.CHSEL  = 5;    //Set SOC0 channel select to ADCINA5

    // Set SOC0 acquisition period to 26 ADCCLK
    AdcRegs.ADCSOC0CTL.bit.ACQPS  = 25;
    AdcRegs.INTSEL1N2.bit.INT1SEL = 0; 	  //Connect ADCINT1 to EOC0
    AdcRegs.INTSEL1N2.bit.INT1E  =  1; 	  //Enable ADCINT1

    // Twiddle LEDs
    GpioDataRegs.GPBDAT.bit.GPIO34 = 0;
    GpioDataRegs.GPBDAT.bit.GPIO39 = 1;

    for(i = 0; i < 500; i++)
    {
        GpioDataRegs.GPBTOGGLE.bit.GPIO34 = 1;
        GpioDataRegs.GPBTOGGLE.bit.GPIO39 = 1;
        DELAY_US(50000);
    }

    // Turn LEDs off
    GpioDataRegs.GPBDAT.bit.GPIO34 = 1;
    GpioDataRegs.GPBDAT.bit.GPIO39 = 1;
    DELAY_US(50000);

    /* Enable Timer */
    CpuTimer0Regs.TCR.all = 0x4000;

    /* Start main loop */
    for(;;)
    {
//    	curTemp = ;

    }

} /* End of File */




