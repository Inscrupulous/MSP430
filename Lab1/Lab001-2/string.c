/*--------------------------------------------------------
 * File:string.c
 * Description:
 * Input:
 * Output:
 * Author:Tyler Phillips
 * Lab Section:10
 * Date:08/24/2020
 *--------------------------------------------------------*/
#include <msp430.h>
#include <stdio.h>

int main()
{
    WDTCTL = WDTPW | WDTHOLD; //stop watchdog timer
    char s[]= "Hello! Welcome to CPE325. It's a great day, isn't it?";
    int i, upper = 0, lower = 0, digit = 0;

    for(i = 0; s[i] != 0; i++)
    {
        if (s[i] >= 'A' && s[i] <= 'Z')
        {
            upper++;
        }
        else if (s[i] >= 'a' && s[i] <= 'z')
        {
            lower++;
        }
        else if (s[i] >= '0' && s[i] <= '9')
        {
            digit++;
        }
    }
    printf("String %s Contains: The string contains %d upper-case and %d lower-case characters and %d digits.",s ,upper, lower, digit);

    return 0;

}
