
//******************************************************************************
//  MSP430FR5x9x
//      Extra Work
//
//  Description: 2 players will compete to find out who press his button faster.
//                  If player 1 lead, the LED will light up and vice versa.
//  ACLK = n/a, MCLK = SMCLK = default DCO
//
//           MSP430FR5994
//         ---------------
//     /|\|               |
//      | |               |
//      --|RST            |
//        |               |
//        |           P1.0|-->LED
//
//   Thai Nghiem (with help of Matthew and Ardti Pranvoku)
//   Rowan Universiy
//   October 2017
//   Built with IAR Embedded Workbench V6.30 & Code Composer Studio V6.1
//******************************************************************************
#include <msp430.h>

unsigned int player1, player2 = 0;

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;
    P1SEL &= ~BIT0; //GPIO option
    P1DIR |= BIT0; //set Port 1.0 output ---LED
    P4SEL &= ~BIT7; //GPIO option
    P4DIR |= BIT7; //set Port 4.7 output ---LED
    P3SEL &= ~BIT0; //GPIO option
    P3DIR |= BIT0; //set Port 3.4 output ---LED

    P1DIR &= ~(BIT1); //set Port 1.1 input --- pushbutton
    P1REN |= BIT1; //enable pull-up/pull-down resistor
    P1OUT |= BIT1; //pull-up resistor

    P1IE |= BIT1; //enable the interrupt on Port 1.1
    P1IES &= ~BIT1; //set as falling edge
    P1IFG &= ~(BIT1); //clear interrupt flag

    P2DIR &= ~(BIT1); //set Port 1.1 input --- pushbutton
    P2REN |= BIT1; //enable pull-up/pulldown resistor on
    P2OUT |= BIT1; //pull-up resistor

    P2IE |= BIT1; //enable the interrupt on Port 1.1
    P2IES &= ~BIT1; //set as falling edge
    P2IFG &= ~(BIT1); //clear interrupt flag

    TA0CTL = TASSEL_1 + MC_1; //Setting up Timer_A in A_clock and count up mode
    TA0CCTL0 = 0x10; //Compare mode
    TA0CCR0 = 500; // Set up value of CCR0 for timer A

    __enable_interrupt(); //enable interrupt
    _BIS_SR(LPM4_bits + GIE); //Enter low-power mode 4
    __no_operation(); // no operation
}

// Winning condition of the game. If player 1 is winning, 
// the LED turn on. If player 2 is winning, the LED will
// turn off. 
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void)
{
    if (player1 > player2)
    {
        P3OUT |= BIT0;
    }
    else
    {
        P3OUT &= ~BIT0;
    }
	//Reset player score to avoid overflow and rollover
    if (player1 > 60000 || player2 > 60000)
    {
        player2 = 0;
        player2 = 0;
    }
}

#pragma vector=PORT1_VECTOR
__interrupt void PORT1_IRS(void)
{
    P2IE &= ~BIT1; // Disable interrupt on P1.1
    __delay_cycles(100); //Debounce
    P2IE |= BIT1; // Enable interrupt on P1.1
    player2++; // Increment the score of player 2
    P4OUT ^= BIT7; //Toggle LED P4.7
    P1IFG &= ~(BIT1); // Clear flag
}

#pragma vector=PORT2_VECTOR
__interrupt void PORT2_IRS(void)
{
    P1IE &= ~BIT1; // Disable interrupt on P1.1
    __delay_cycles(100); //Debounce
    P1IE |= BIT1; // Enable interrupt on P1.1
    player1++; // Increment the score of player 1
    P1OUT ^= BIT0; //Toggle LED on P1.0
    P2IFG &= ~(BIT1); // Clear flag
}
