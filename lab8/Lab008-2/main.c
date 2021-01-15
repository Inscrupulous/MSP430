#include <msp430.h>

char TestUserName[50];
char TestPassWord[50];
char OneUserName[50] = "LabStudent1                                       ";
char OnePassWord[50] = "password1                                         ";
char TwoUserName[50] = "LabStudent2                                       ";
char TwoPassWord[50] = "password2                                         ";
char EU[] = "Enter a Username:";
char EP[] = "Enter a Password:";
char CP[] = "Welcome to CPE 325!!!";
char IP[] = "Incorrect username or password!!!";
char temp;
char temp2;
int i = 0;
int k = 0;
int sec = 0;
int tsec = 0;
int noPassword = 0;

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

//void TimerA_setup(void)
//{
//    TA0CTL = TASSEL_2 + MC_1 + ID_3;    // Select SMCLK/8 and up mode
//    TA0CCR0 = 13107;                    // 100ms interval
//    TA0CCTL0 = CCIE;                    // Capture/compare interrupt enable
//}

void SetTime(void)
{
    tsec++;
    if (tsec == 10)
    {
        tsec = 0;
        noPassword = 1;
        WDTCTL = WDTPW + WDTHOLD;   //Stop watchdog timer
        SFRIE1 &= ~WDTIFG;
        return;
    }
}

void testUserOne()
{
    int j = 0;
    int userIncorrect = 0;
    int passIncorrect = 0;
    for(j=0; j<sizeof(OneUserName); j++){
        while(!(UCA0IFG&UCTXIFG));
        if(TestUserName[j] != OneUserName[j]){
            userIncorrect = 1;
            break;
        }
    }
    if (userIncorrect == 0){
        for(j=0; j<sizeof(OnePassWord); j++){
            if(TestPassWord[j] != OnePassWord[j]){
                passIncorrect = 1;
                break;
            }
        }
    }
    if (userIncorrect == 0 && passIncorrect == 0){
        for (i=0; i<sizeof(CP); i++) {
            while(!(UCA0IFG&UCTXIFG));  // Wait until TXBUF is free
            UCA0TXBUF = CP[i];
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
    int m = 0;
    int n = 0;
    int userIncorrect2 = 0;
    int passIncorrect2 = 0;
    for(m=0; m<sizeof(TwoUserName); m++){
        if(TestUserName[m] != TwoUserName[m]){
            for (n=0; n<sizeof(IP); n++) {
                while(!(UCA0IFG&UCTXIFG));  // Wait until TXBUF is free
                UCA0TXBUF = IP[n];
            }
            UCA0TXBUF = '\n';
            while(!(UCA0IFG&UCTXIFG));
            UCA0TXBUF = '\r';
            while(!(UCA0IFG&UCTXIFG));
            userIncorrect2 = 1;
            break;
            }
    }
    if (userIncorrect2 == 0){
        for(m=0; m<sizeof(TwoPassWord); m++){
            if(TestPassWord[m] != TwoPassWord[m]){
                for (n=0; n<sizeof(IP); n++) {
                    while(!(UCA0IFG&UCTXIFG));  // Wait until TXBUF is free
                    UCA0TXBUF = IP[n];
                }
                UCA0TXBUF = '\n';
                while(!(UCA0IFG&UCTXIFG));
                UCA0TXBUF = '\r';
                while(!(UCA0IFG&UCTXIFG));
                passIncorrect2 = 1;
                break;
            }
        }
    }
    if (userIncorrect2 == 0 && passIncorrect2 == 0){
        for (m=0; m<sizeof(CP); m++) {
            while(!(UCA0IFG&UCTXIFG));  // Wait until TXBUF is free
            UCA0TXBUF = CP[m];
        }
        UCA0TXBUF = '\n';
        while(!(UCA0IFG&UCTXIFG));
        UCA0TXBUF = '\r';
        while(!(UCA0IFG&UCTXIFG));
    }
}

void main(void)
{
    //WDTCTL = WDT_ADLY_1000;
   // SFRIE1 |= WDTIE;

    UART_setup();               // Initialize UART

    int enteredPassword = 0;
    while (1)
    {
       WDTCTL = WDT_ADLY_1000;
       SFRIE1 |= WDTIE;
       for (i=0; i<sizeof(TestUserName); i++) {
            TestUserName[i] = ' ';
            TestPassWord[i] = ' ';
       }
        for (i=0; i<sizeof(EU); i++) {
            while(!(UCA0IFG&UCTXIFG));  // Wait until TXBUF is free

            UCA0TXBUF = EU[i];
        }
        for (i=0; i<sizeof(TestUserName); i++){
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
        for (i=0; i<sizeof(EP); i++) {
            while(!(UCA0IFG&UCTXIFG));  // Wait until TXBUF is free

            UCA0TXBUF = EP[i];
        }
        enteredPassword = 0;
        noPassword = 0;
        //_BIS_SR(LPM0_bits + GIE);   // Enter LPM0 w/ interrupts
        while(noPassword == 0 && enteredPassword == 0){
            _BIS_SR(LPM0_bits + GIE);   // Enter LPM0 w/ interrupts
        for (i=0; i<sizeof(TestPassWord); i++){
            while(!(UCA0IFG&UCRXIFG));  // Wait for a new character
            temp2 = UCA0RXBUF;
            if(temp2 == '\r'){
                UCA0TXBUF = '\n';
                while(!(UCA0IFG&UCTXIFG));
                UCA0TXBUF = '\r';
                while(!(UCA0IFG&UCTXIFG));
                enteredPassword = 1;
                break;
            }
            else{
                TestPassWord[i] = temp2;
                UCA0TXBUF = TestPassWord[i];
                while(!(UCA0IFG&UCTXIFG));
            }
        }
        }
        if(enteredPassword == 1){
        testUserOne();
        enteredPassword = 0;
        }
    }
}

#pragma vector = WDT_VECTOR
__interrupt void watchdog_timer(void)
{
    SetTime();  // Update time
    _BIC_SR_IRQ(LPM0_bits);
}
