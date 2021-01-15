#include <msp430.h>
#include <stdio.h>
#include <stdint.h>

char EnterUser[] = "Enter a Username:";
char EnterPass[] = "Enter a Password:";
char TestUserName[50];
char TestPassWord[50];
char OneUserName[50] = "tjp0018                                           ";
char OnePassWord[50] = "ILoveCPE325                                       ";
char TwoUserName[50] = "nae0005                                           ";
char TwoPassWord[50] = "MSP430IsCool                                      ";
char CorrectPass[] = "Welcome to CPE 325!!!";
char IncorrectPass[] = "Incorrect username or password!!!";
char temp;
char temp1;
int i = 0;
int j = 0;
int k = 0;
int m = 0;
int n = 0;
int sec = 0;
int tsec = 0;
int noPassWord = 0;
int userIncorrect = 0;
int passIncorrect = 0;
int userIncorrect1 = 0;
int passIncorrect1 = 0;
int enteredPassWord = 0;

void UART_setup(void)
{

    P3SEL |= BIT3 + BIT4;       // Set USCI_A0 RXD/TXD to receive/transmit data
    UCA0CTL1 |= UCSWRST;        // Set software reset during initialization
    UCA0CTL0 = 0;               // USCI_A0 control register
    UCA0CTL1 |= UCSSEL_2;       // Clock source SMCLK

    UCA0BR0 = 0x09;             // 1048576 Hz  / 115200 lower byte
    UCA0BR1 = 0x00;             // upper byte
    UCA0MCTL |= UCBRS0;         // Modulation (UCBRS0=0x01, UCOS16=0)
    // Clear software reset to initialize USCI state machine
    UCA0CTL1 &= ~UCSWRST;
}

void SetTime(void)
{
    tsec++;
    if (tsec == 10)
    {
        tsec = 0;
        noPassWord = 1;
        WDTCTL = WDTPW + WDTHOLD;   //Stop watchdog timer
        SFRIE1 &= ~WDTIFG;
        return;
    }
}


void testUserOne()
{
    for(j = 0; j < sizeof(OneUserName); j++){
        while(!(UCA0IFG&UCTXIFG));
        if(TestUserName[j] != OneUserName[j]){
            userIncorrect = 1;
            break;
        }
    }
    if (userIncorrect == 0){
        for(j = 0; j < sizeof(OnePassWord); j++){
            if(TestPassWord[j] != OnePassWord[j]){
                passIncorrect = 1;
                break;
            }
        }
    }
    if (userIncorrect == 0 && passIncorrect == 0){
        for (i = 0; i < sizeof(CorrectPass); i++) {
            while(!(UCA0IFG&UCTXIFG));  // Wait until TXBUF is free
            UCA0TXBUF = CorrectPass[i];
        }
        UCA0TXBUF = '\n';
        while(!(UCA0IFG&UCTXIFG));
        UCA0TXBUF = '\r';
        while(!(UCA0IFG&UCTXIFG));
    }
    if (userIncorrect == 1 || passIncorrect == 1){
        testUserTwo();
    }

}

void testUserTwo()
{
    for(m = 0; m < sizeof(TwoUserName); m++){
        if(TestUserName[m] != TwoUserName[m]){
            for (n = 0; n < sizeof(IncorrectPass); n++) {
                while(!(UCA0IFG&UCTXIFG));  // Wait until TXBUF is free
                UCA0TXBUF = IncorrectPass[n];
            }
            UCA0TXBUF = '\n';
            while(!(UCA0IFG&UCTXIFG));
            UCA0TXBUF = '\r';
            while(!(UCA0IFG&UCTXIFG));
            userIncorrect1 = 1;
            break;
            }
    }
    if (userIncorrect1 == 0){
        for(m = 0; m < sizeof(TwoPassWord); m++){
            if(TestPassWord[m] != TwoPassWord[m]){
                for (n = 0; n < sizeof(IncorrectPass); n++) {
                    while(!(UCA0IFG&UCTXIFG));  // Wait until TXBUF is free
                    UCA0TXBUF = IncorrectPass[n];
                }
                UCA0TXBUF = '\n';
                while(!(UCA0IFG&UCTXIFG));
                UCA0TXBUF = '\r';
                while(!(UCA0IFG&UCTXIFG));
                passIncorrect1 = 1;
                break;
            }
        }
    }
    if (userIncorrect1 == 0 && passIncorrect1 == 0){
        for (m = 0; m < sizeof(CorrectPass); m++) {
            while(!(UCA0IFG&UCTXIFG));  // Wait until TXBUF is free
            UCA0TXBUF = CorrectPass[m];
        }
        UCA0TXBUF = '\n';
        while(!(UCA0IFG&UCTXIFG));
        UCA0TXBUF = '\r';
        while(!(UCA0IFG&UCTXIFG));
    }
}

void main(void)
{
    UART_setup();               // Initialize UART

    while (1)
    {
       WDTCTL = WDT_ADLY_1000;
       SFRIE1 |= WDTIE;
       for (i = 0; i < sizeof(TestUserName); i++) {
            TestUserName[i] = ' ';
            TestPassWord[i] = ' ';
       }
        for (i = 0; i < sizeof(EnterUser); i++) {
            while(!(UCA0IFG&UCTXIFG));  // Wait until TXBUF is free

            UCA0TXBUF = EnterUser[i];
        }
        for (i = 0; i < sizeof(TestUserName); i++){
            while(!(UCA0IFG&UCRXIFG));  // Wait for a new character
            temp = UCA0RXBUF;
            if(temp == '\r'){
                UCA0TXBUF = '\n';
                while(!(UCA0IFG&UCTXIFG));
                UCA0TXBUF = '\r';
                while(!(UCA0IFG&UCTXIFG));
                break;
            }
            else{
                TestUserName[i] = temp;
                UCA0TXBUF = TestUserName[i];
                while(!(UCA0IFG&UCTXIFG));
            }
        }
        for (i = 0; i < sizeof(EnterPass); i++) {
            while(!(UCA0IFG&UCTXIFG));  // Wait until TXBUF is free

            UCA0TXBUF = EnterPass[i];
        }
        enteredPassWord = 0;
        noPassWord = 0;
        //_BIS_SR(LPM0_bits + GIE);   // Enter LPM0 w/ interrupts
        while(noPassWord == 0 && enteredPassWord == 0){
            _BIS_SR(LPM0_bits + GIE);   // Enter LPM0 w/ interrupts
        for (i = 0; i < sizeof(TestPassWord); i++){
            while(!(UCA0IFG&UCRXIFG));  // Wait for a new character
            temp1 = UCA0RXBUF;
            if(temp1 == '\r'){
                UCA0TXBUF = '\n';
                while(!(UCA0IFG&UCTXIFG));
                UCA0TXBUF = '\r';
                while(!(UCA0IFG&UCTXIFG));
                enteredPassWord = 1;
                break;
            }
            else{
                TestPassWord[i] = temp1;
                UCA0TXBUF = TestPassWord[i];
                while(!(UCA0IFG&UCTXIFG));
            }
        }
        }
        if(enteredPassWord == 1){
        testUserOne();
        enteredPassWord = 0;
        }
    }
}

//call SetTime function
#pragma vector = WDT_VECTOR
__interrupt void watchdog_timer(void)
{
    SetTime();
    _BIC_SR_IRQ(LPM0_bits);
}
