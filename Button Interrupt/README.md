# Read Me for Button interrupt
Author: Thai Nghiem (collaborate with Ardit Pranvoku)

## Description
Implements a timer based interrupt to blink the LED at a designated frequency.
The watchdog timer must be stopped with the line 
```c
WDTCTL = WDTPW + WDTHOLD or WDTCTL = WDTPW | WDTHOLD.
```
Else, the processor will reset. <br />
The desired led pins and bits must be set to 1 to configure it to be an output.
The desired button pin and bit must be to 0 to configure it to be an input .<br />
Also,  PXREN |= BITX; must be used to enable the pullup resistor for that button. <br />    
By using the line PM5CTL0 = ~LOCKLPM5, the default high impedance on the board is disabled.
This high impedance serves to get rid of any cross currents, but is turned off later. <br />
The processor is put into LPM4 to prepare for the interrupt from the button. <br />
In the same line, GIE is enabled so the interrupt is not masked. <br />

The interrupt is set up using the lines
```c
#pragma vector=PORT5_VECTOR
__interrupt void PORT_5(void){
		//toggle the state of the LED
}
```
During the interrupt, the LED is toggled, and then a delay of 1000 cycles is activated so that
the frequency is low enough for the user to see the LED blink.<br />
After, the interrupt flag for P5.5 is cleared so the interrupt can activate again with a new button press.
## Changes across the boards

# How to implement the code
To run this code, simply import it into code composer, then clikc build. 
After you plug in your MSP430, hit debug. When you press the button, LED1 one the board should change its state