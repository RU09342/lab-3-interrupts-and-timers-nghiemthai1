//******************************************************************************
//   MSP430F Demo
//      Button Based Interrupt
//
//   Description: Interrupt a ongoing process by pushing a button. The delay time
//              is based upon how long the button is pressed.
//   ACLK = 32.768kHz, MCLK = SMCLK = default DCO~1MHz
//
//                MSP430F552x
//             -----------------
//         /|\|                 |
//          | |                 |
//          --|RST              |
//            |                 |
//            |             P1.0|-->LED
//
//   Thai Nghiem
//   Rowan University
//   September 2017
//   Built with CCSv4 and IAR Embedded Workbench Version: 4.21
//******************************************************************************
#include <msp430.h>

int buttonPressed;

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD; //Stop watchdog timer

    P1SEL &= ~BIT0; //select GPIO
    P1DIR |= BIT0; //set Port 1.0 output ---LED
    P1OUT &= ~(BIT0); // Clear P1.0


    P1DIR &= ~(BIT3); //set Port 1.3 input --- pushbutton
    P1REN |= BIT3; //enable pull-up resistor on
    P1OUT |= BIT3;


    P1IE |= BIT3; //enable the interrupt on Port 1.3
    P1IES |= BIT3; //set as falling edge
    P1IFG &= ~(BIT3); //clear interrupt flag

    TA0CTL = TASSEL_1 + MC_1 + ID_2; //Set up Timer A, Count up, and divider 4.
    TA0CCTL0 = 0x10; //Set up compare mode for CCTL
    TA0CCR0 = 1600; // LED will blink at 32kHZ*2/1600 = 10.6 Hz

    //enter LPM0 mode and enable global interrupt
           _BIS_SR(LPM0_bits + GIE);
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0(void)
{

    P1OUT ^= 0x01; //Toggle LED

}

#pragma vector=PORT1_VECTOR
__interrupt void PORT_1(void)
{
    //Debouncing
    P1IE &= ~BIT3;
    __delay_cycles(1);

    if (buttonPressed == 0) //Falling-edge of a button
    {
        TA1CTL = TASSEL_1+ MC_3; // Selecting Timer A and Count Continuous
        TA1CCR0 = 0xFFFF; //Initialize value of TA1CCR0
        TA1CCTL0 = CAP; //Capture mode
        buttonPressed = 1;
        TA0CCR0 = 1; //Reset CCR0

    }
    else if (buttonPressed == 1) //Rising-edge of a button
    {
        TA1CTL = MC_0; //Stop Counting
        TA0CCR0 = TA1R; //Assign new value for CCR0
        if (TA0CCR0 > 65500) //Fastest
            TA0CCR0 = 0xFFFF;
        if (TA0CCR0 < 2000) // Slowest
            TA0CCR0 = 2000;
        TA1CTL = TACLR; //Clear Timer A1
        buttonPressed = 0;
    }

    P1IES ^= BIT3; //toggle to set as rising edge
    P1IE |= BIT3; // Enable interrupt
    P1IFG &= ~(BIT3); // Clear flag
}

