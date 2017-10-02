//******************************************************************************
//   MSP430FR2311 Demo
//      Timer A Interrupt
//
//   Description: Timer A will control the rate at which the LEDs blink
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

    P1OUT &= ~(BIT0); // Clear P1.0
    P1DIR |=BIT0; //set Port 1.0 output ---LED

    P2OUT &= ~(BIT0); //Clear P2.0
    P2DIR |=BIT0; //set Port 2.0 output ---LED


    TB0CTL= TBSSEL_1 + MC_1; //Set up Timer B0 in Up mode
    TB0CCTL0 = 0x10; //Set up Timer B0 in the compare mode
    TB0CCR0 = 12000; //set up TB0CCR0, and have Timer B0 (TB0R) to count to its contents.
                    // 32kHz/12000 is the frequency at which the LED will blink
    TB0CCTL1 = 0x10;    // Set up Timer B 1 Compare moce
    TB0CCR1 = 3000;     // Set up value for CCR1 to 3000
    //enter LPM4 mode and enable global interrupt
    _BIS_SR(LPM4_bits + GIE);

}
//Port 1 ISR
#pragma vector=TIMER0_B0_VECTOR
#pragma vector = TIMER0_B1_VECTOR
__interrupt void Timer_B(void)
{
    P1OUT ^= 0x01; //Toggle the P1.0 LED
    switch (TB0IV) //Choose the right Interrupt Vector for value of TB0CCR1
    {
    case TB0IV_TBCCR1: P2OUT ^= BIT0; //Toggle the P2.0 LED
    break;
    }
}
