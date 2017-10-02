//******************************************************************************
//   MSP430F552x Demo
//      Button Interrupt
//
//   Description: Inturrupt a ongoing process by pushing a button
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
#include <msp430f5529.h>

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;// Stop watchdog timer

    P1SEL =0; //Select GPIO option
    P1DIR |=BIT0; //set Port 1.0 output ---LED

    P1DIR &=~(BIT1); //set Port 1.1 input --- pushbutton
    P1REN|=BIT1;//enable pull-up/pull-down resistor on
    P1OUT|=BIT1; //choose the pull-up resistor

    P1IE |=BIT1;//enable the interrupt on Port 1.1
    P1IES |=BIT1;//set as falling edge
    P1IFG &=~(BIT1);//clear interrupt flag

    //enter LPM4 mode and enable global interrupt
    _BIS_SR(LPM4_bits + GIE);
}
//Port 1 ISR
#pragma vector=PORT1_VECTOR
__interrupt void PORT_1(void)
{
    P1OUT ^=0x01; // Change state of P1.1
    //Debouncing
    volatile unsigned int dur = 1000;
    do (dur--);
    while (dur != 0);
    P1IFG &=~(BIT1); // Clear flag
}
