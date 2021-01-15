//-------------------------------------------------------------------------------
// File: main.c (CPE 325 Lab 9)
// Function: Interfacing Gravity in terms of placement of the accelerometer.
// Description:This C program interfaces with an accelerometer that has
// X(HORZ on Accelerometer connected to P6.0) axis and
// Y (VERT on Accelerometer connected to P6.1) axis and
// Z (ROT on Accelerometer connected to P6.2) and outputs from 0 to 3V.
//              The values of x, y, z axis is sent as the percentage
//             of power to the UAH Serial App.
//
// Clocks:      ACLK = LFXT1 = 32768Hz, MCLK = SMCLK = DCO = default (~1MHz)
//              An external watch crystal beten XIN & XOUT is required for ACLK
//                      MSP-EXP430F5529LP
//                    --------------------
//                   /|\|              XIN|-
//                    | |                 | 32kHz
//                    --|RST          XOUT|-
//                      |                 |
 //                     |     P3.3/UCA0TXD|------------>
//           P6.0(A0)-->|                 | 115200 - 8N1
//           P6.1(A1)-->|     P3.4/UCA0RXD|<------------
//                      |                 |

// Input: Connect Accelerometer to the board
// Output:Displays  position relative to gravity and if net gravity is greater than
// 2.0 the LEDs blink alternatively at 1Hz until a switch is pressed stopping the cycle.
// Author: Tyler Phillips
// Date: 11/05/20
//-------------------------------------------------------------------------------
#include <msp430.h>
#include <math.h>
#define SW2 (P1IN&BIT1)
#define SW1 (P1IN&BIT0)

volatile long int ADCXval, ADCYval, ADCZval;
volatile float gX, gY, gZ, netG;
unsigned int hold = 0;
unsigned int blink = 0;

void TimerA_setup(void)
{
    TA0CCTL0 = CCIE;                     // Enabled interrupt

    TA0CCR0 = 3277;                      // 3277 / 32768 Hz = 0.1s
    TA0CTL = TASSEL_1 + MC_1;            // ACLK, up mode
}

void switch_setup(void)
{
    P1DIR |= BIT0;     //Setup LED1 as output
    P1OUT |= BIT0;     //LED1 is intially off
    P4DIR |= BIT7;     //LED2
    P4OUT &= ~BIT7;

    P2DIR &= ~BIT1;     //Setup SW1
    P2REN |= BIT1;
    P2OUT |= BIT1;
    P2IE |= BIT1;
    P2IES |= BIT1;

    P1DIR &= ~BIT1;     //Setup SW2
    P1REN |= BIT1;
    P1OUT |= BIT1;
    P1IE |= BIT1;
    P1IES |= BIT1;

}

void ADC_setup(void)
{
    // configure ADC converter
    P6SEL |= BIT0 + BIT1 + BIT2;                            // Enable A/D channel inputs
    ADC12CTL0 = ADC12ON+ADC12MSC+ADC12SHT0_8; // Turn on ADC12, extend sampling time
                                              // to avoid overflow of results

    ADC12CTL1 = ADC12SHP+ADC12CONSEQ_1;       // Use sampling timer, repeated sequence
    ADC12MCTL0 = ADC12INCH_0;                 // ref+=AVcc, channel = A0
    ADC12MCTL1 = ADC12INCH_1;                 // ref+=AVcc, channel = A1
    ADC12MCTL2 = ADC12INCH_2+ADC12EOS;        // ref+=AVcc, channel = A2, end seq.

    ADC12IE |= 0x02;                           // Enable ADC12IFG.1
    ADC12CTL0 |= ADC12ENC;                    // Enable conversions
}


void UART_putCharacter(char c)
{
    while (!(UCA0IFG&UCTXIFG));    // Wait for previous character to transmit
    UCA0TXBUF = c;                  // Put character into tx buffer
}


void UART_setup(void)
{
    P3SEL |= BIT3 + BIT4;   // Set USCI_A0 RXD/TXD to receive/transmit data

    UCA0CTL1 |= UCSWRST;    // Set software reset during initialization
    UCA0CTL0 = 0;           // USCI_A0 control register
    UCA0CTL1 |= UCSSEL_2;   // Clock source SMCLK

    UCA0BR0 = 0x09;         // 1048576 Hz  / 115200 lower byte
    UCA0BR1 = 0x00;         // upper byte
    UCA0MCTL |= UCBRS0;     // Modulation (UCBRS0=0x01, UCOS16=0)

    UCA0CTL1 &= ~UCSWRST;   // Clear software reset to initialize USCI state machine
}


void sendData(void)
{
    int i;
    // Calculate g values for x, y, z
    gX = ((((float)ADCXval/4095*3.0)-1.5)/0.3);
    gY = ((((float)ADCYval/4095*3.0)-1.5)/0.3);
    gZ = ((((float)ADCZval/4095*3.0)-1.5)/0.3);

    // Use character pointers to send one byte at a time
    char *xpointer=(char *)&gX;
    char *ypointer=(char *)&gY;
    char *zpointer=(char *)&gZ;

    UART_putCharacter(0x55);    // Send header
    for(i = 0; i < 4; i++)
    {   // Send x percentage - one byte at a time
        UART_putCharacter(xpointer[i]);
    }
    for(i = 0; i < 4; i++)
    {   // Send y percentage - one byte at a time
        UART_putCharacter(ypointer[i]);
    }
    for(i = 0; i < 4; i++)
    {   // Send z percentage - one byte at a time
        UART_putCharacter(zpointer[i]);
    }

}

void calculateNetG(float x, float y, float z)
{
    netG = sqrt((gX*gX)+(gY*gY)+(gZ*gZ));
}

void main(void)
{
    // Enable interrupts globally
    __enable_interrupt();

    ADC_setup();    // Setup ADC
    UART_setup();
    TimerA_setup();
    switch_setup();
    WDTCTL = WDT_MDLY_32;   //32 ms
    SFRIE1 |= WDTIE;

    while (1)
    {
        __bis_SR_register(LPM0_bits + GIE); // Enter LPM0
        sendData();
        calculateNetG(gX, gY, gZ);
        if (netG >= 2.00)
        {
            blink = 1;
            hold = 0;
        }
    }
}

#pragma vector = ADC12_VECTOR
__interrupt void ADC12ISR(void)
{
    ADCXval = ADC12MEM0;    // Move results, IFG is cleared
    ADCYval = ADC12MEM1;
    ADCZval = ADC12MEM2;
    __bic_SR_register_on_exit(LPM0_bits);   // Exit LPM0
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void timerA_isr()
{
    ADC12CTL0 |= ADC12SC;
    __bic_SR_register_on_exit(LPM0_bits);   // Exit LPM0
}

//--------------------------------------------------------
//                   Port ISR's
//--------------------------------------------------------

// Port 1 interrupt service routine (SW2 Press)
#pragma vector = PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
    //let us clear the flag
    P1IFG &= ~BIT1;
    //debounce
    __delay_cycles(2500);
    hold = 1;
    blink = 0;

}

// Port 2 interrupt service routine (SW1 Press)
#pragma vector = PORT2_VECTOR
 __interrupt void PORT2_ISR(void)
 {
     //let us clear the flag
     P2IFG &= ~BIT1;
     //debounce
     __delay_cycles(2500);
     hold = 1;
     blink = 0;
 }

#pragma vector = WDT_VECTOR
__interrupt void watchdog_timer(void)
{
    static int count = 0;
    count++;    //Increment counter

    if(count == 16)   //32*16 = 512 => ~0.5s
    {
        count = 0;  //Reset counter to 0
        if (count == 1)
        {
            return;
        }

        else if (blink == 1)
        {
            P1OUT ^= BIT0;
            P4OUT ^= BIT7;
        }
        return;
    }
}
