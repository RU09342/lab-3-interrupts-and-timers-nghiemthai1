# Read Me for Button interrupt
Author: Thai Nghiem (collaborate with Ardit Pranvoku)

Implements a timer based interrupt to blink the LED at a designated frequency.
The watchdog timer must be stopped with the line 
```c
WDTCTL = WDTPW + WDTHOLD or WDTCTL = WDTPW | WDTHOLD.
```
Else, the processor will reset. <br />
The desired led pins and bits must be set to 1 to configure it to be an output.
The desired button pin and bit must be to 0 to configure it to be an input .<br />
Also,  PXREN |= BITX; must be used to enable the pullup resistor for that button. <br />     
TA0CTL is set to TASSEL_1 + MC_1. This sets up ACLK at 32kHz and enables up mode for the timer.
TA0CCTL1 is set equal to 0x10 to set TACCR1 to compare mode.
Finally, TA0CCR1 is set to 3000, so an interrupt is generated when TAR is equal to TA0CCR1.

The processor is put into LPM4 to prepare for the interrupt from the button.
In the same line, GIE is enabled so the interrupt is not masked.

The interrupt is set up using the lines
```c
#pragma vector=PORT5_VECTOR
__interrupt void PORT_5(void){
		//toggle the state of the LED
}
```
During the interrupt, a switch case is used for TACCR1. This is done because TACCR0 is the only CCR 
that can be used to generate an interrupt automatically. 
## Changes across the boards
The specific output pin number of each board for each LED and button must be changed accordingly <br />
The msp430FRxxx series (FR6989, FR2311, and FR5994 in this case) need to use the line PM5CTL0 = ~LOCKLPM5 to disable the default high impedance on the board. 
This high impedance serves to get rid of any cross currents, but is turned off later. <br />
In addtion, the FR2311 does not have a Timer_A, so Timer_B must be used instead. 
# How to implement the code
To run this code, simply import it into code composer, then clikc build. 
After you plug in your MSP430, hit debug. When you press the button, LED1 one the board should change its state