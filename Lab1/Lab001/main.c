/*--------------------------------------------------------
 * File:main.c
 * Description: Takes an unsigned integer and outputs the values multiplied up to 10.
 * Input: None
 * Output:Multiplication sum ie. a X 1 = a
 * Author:Tyler Phillips
 * Lab Section:10
 * Date:08/23/2020
 *--------------------------------------------------------*/
#include <msp430.h>
#include <stdio.h>

int main()
{
    WDTCTL = WDTPW | WDTHOLD; //stop watchdog timer
    unsigned int a = 5;
    print_multiplication_table(a);

    return 0;
}

void print_multiplication_table(unsigned int a)
{
    int i;

    for(i = 1; i <= 10; i++)
    {
        printf("%d X %d= %d\n" , a, i, a*i);
    }
}
