/*--------------------------------------------------------
 * File:main.c
 * Description: print the sizes and ranges of common data types
 * Input: None
 * Output: Table that shows size(bytes), and minimum/maximum ranges for the data types
 * Author:Tyler Phillips
 * Lab Section:10
 * Date:08/30/2020
 *--------------------------------------------------------*/
#include<limits.h>
#include<stdio.h>
#include<float.h>
#include<msp430.h>



void printChar(char c, int count)
{
    int i;

    for(i = 0; i < count; i++)
    {

        printf("%c",c);

    }

        printf("\n");

}

int main()
{
    WDTCTL = WDTPW | WDTHOLD; //stop watchdog timer

    //printing format

    printChar('-',93);


        printf("| %-20s | %-20s | %-20s | %-20s |\n","Data Type","Size (in bytes)","Minimum","Maximum");

        printChar('-',93);



            //char signed

            printf("| %-20s | %-20d | %-20d | %-20d |\n","char",sizeof(char),SCHAR_MIN,SCHAR_MAX);



            //short signed

            printf("| %-20s | %-20d | %-20d | %-20d |\n","short int",sizeof(short),SHRT_MIN,SHRT_MAX);



            //int signed

            printf("| %-20s | %-20d | %-20d | %-20d |\n","int",sizeof(int),INT_MIN,INT_MAX);



            //long signed

            printf("| %-20s | %-20d | %-20ld | %-20ld |\n","long int",sizeof(long),LONG_MIN,LONG_MAX);



            //long long signed

            printf("| %-20s | %-20d | %-20lld | %-20lld |\n","long long int",sizeof(long long),LLONG_MIN,LLONG_MAX);



            //char unsigned

            printf("| %-20s | %-20d | %-20u | %-20u |\n","unsigned char",sizeof(unsigned char),0,UCHAR_MAX);



            //short unsigned

            printf("| %-20s | %-20d | %-20u | %-20u |\n","unsigned short int",sizeof(unsigned short),0,USHRT_MAX);



            //int unsigned

            printf("| %-20s | %-20d | %-20u | %-20u |\n","unsigned int",sizeof(unsigned int),0,UINT_MAX);



            //long unsigned

            printf("| %-20s | %-20d | %-20d | %-20lu |\n","unsigned long int",sizeof(unsigned long),0,ULONG_MAX);



            //long long unsigned

            printf("| %-20s | %-20d | %-20d | %-20llu |\n","unsigned long long",sizeof(unsigned long long),0,ULLONG_MAX);



            //float

            printf("| %-20s | %-20d | %-20e | %-20e |\n","float",sizeof(float),FLT_MIN,FLT_MAX);



            //double

            printf("| %-20s | %-20d | %-20e | %-20e |\n","double",sizeof(double),DBL_MIN ,DBL_MAX);

            //more formatting
            printChar('-',93);


              return 0;

}
