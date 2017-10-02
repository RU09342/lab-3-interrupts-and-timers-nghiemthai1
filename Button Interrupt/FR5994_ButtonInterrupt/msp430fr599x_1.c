//******************************************************************************
//   MSP430FR5994 Demo
//      Button Interrupt
//
//   Description: Inturrupt a ongoing process by pushing a button
//   ACLK = 32.768kHz, MCLK = SMCLK = default DCO~1MHz
//
//                MSP430
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

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;// Stop watchdog timer

    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings

    P1DIR |=BIT0; //set Port 1.0 output ---LED

    P5DIR &=~(BIT5); //set Port 5.5 input --- pushbutton
    P5REN|=BIT5;//enable pull-up/pull-down resistor on
    P1OUT|=BIT1; //choose the pull-up resistor

    P5IE |=BIT5;//enable the interrupt on Port 1.1
    P5IES |=BIT5;//set as falling edge
    P5IFG &=~(BIT5);//clear interrupt flag

    //enter LPM4 mode and enable global interrupt
    _BIS_SR(LPM4_bits + GIE);
}
//Port 1 ISR
#pragma vector=PORT5_VECTOR
__interrupt void PORT_5(void)
{
    P1OUT ^=0x01; // Change state of P1.1
    volatile unsigned int dur = 1000;
    do (dur--);
    while (dur != 0);
    P5IFG &=~(BIT5); // Clear flag
}
