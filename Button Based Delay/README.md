# Read Me for Button based delay
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

TA0CTL is configured using the desired settings. We used Timer A, up mode, and a divider of 2 so our code was:
```c
TA0CTL = TASSEL_1 + MC_1 + ID_1; //Set up Timer A, Count up, and divider 2.
```
Next, the timer is put into compare mode and TA0CCR0 is set to 6000 for up mode.

The program uses the falling edge and rising edge of the Button to change TA0CCR0.
When the button is pressed, the button interrupt is triggered, and the first if condition is 
entered as buttonPressed = 0. TA1CCR0 is set to capture mode. This will copy the value of TA1R later. 
buttonPressed is set to 1.<br />
When the button is released, the button interrupt is triggered again, and TA1R is assigned to TA1CCR0.
When the timer counts up again, it will count up to the new value of TA1CCR0, causing the frequency to be
designated by the user. Timer A1 is cleared, and buttonPressed is set to 0, ready for a new button press.
## Changes across the boards
The specific output pin number of each board for each LED and button must be changed accordingly <br />
The msp430FRxxx series (FR6989, FR2311, and FR5994 in this case) need to use the line PM5CTL0 = ~LOCKLPM5 to disable the default high impedance on the board. 
This high impedance serves to get rid of any cross currents, but is turned off later. <br />
In addtion, the FR2311 does not have a Timer_A, so Timer_B must be used instead. 
# How to implement the code
To run this code, simply import it into code composer, then click build. 
After you plug in your MSP430, hit debug. When you press the button, LED1 one the board should blink according to how long you hold the button down.