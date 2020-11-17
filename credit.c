#include <stdio.h>
#include <cs50.h>

//checks for the validity of a credit card number
//outputs the type of credit card, if valid

int main(void)
{
    //variable storing card number
    long n;
    //variable storing card number length
    int l = 0;
    //variable storing sum of multiplied digits
    int m = 0;
    //variable storing sum of remaining digits
    int r = 0;
    //variable storing first digit
    int one;
    //variable storing second digit
    int two;

    do
    {
        //request user input for card number
        n = get_long("Please enter your credit card number: \n");
    }
    //prevent user input that is outside the parameters of a credit card
    while (n < 1);

    //temp variable to hold cc # while testing user submission
    long t = n;

    //calculate length of n
    for (int i = 1; t > 1; i++)
    {
        t = t / 10;
        l = i;
    }
    
    //temp variable to hold cc # while testing user submission
    long temp = n;

    //process whole CC number
    for (int i = 1; l + 1 > i; i++)
    {
        //multiplies every second digit by two and totals the sum of their digits
        if (i % 2 == 0)
        {
            int j = temp % 10;
            j = j * 2;

            if (j > 9)
            {
                m = m + (j % 10);
                j = j / 10;
            }
            m = m + j;
        }
        //sum remaining digits
        else
        {
            int j = temp % 10;
            r = r + j;
        }

        //save second digit
        if (i == l - 1)
        {
            two = temp % 10;
        }

        //save first digit
        if (i == l)
        {
            one = temp % 10;
        }

        //reduce CC number and restart loop
        temp = temp / 10;
    }
    
    //check card for validity
    if (l == 13 || l == 15 || l == 16)
    {
        if ((m + r) % 10 == 0)
        {
            //check for AMEX
            if (l == 15)
            {
                if (one == 3)
                {
                    if (two == 4 || two == 7)
                    {
                        printf("AMEX\n");
                    }
                    else
                    {
                        printf("INVALID\n");
                    }
                }
            }

            //check for 13 digit VISA
            else if (l == 13)
            {
                if (one == 4)
                {
                    printf("VISA\n");
                }
                else
                {
                    printf("INVALID\n");
                }
            }

            //check for 16 digit VISA or Mastercard
            else if (l == 16)
            {
                if (one == 4)
                {
                    printf("VISA\n");
                }
                else if (one == 5)
                {
                    if (two == 1 || two == 2 || two == 3 || two == 4 || two == 5)
                    {
                        printf("MASTERCARD\n");
                    }
                    else
                    {
                        printf("INVALID\n");
                    }
                }

                else
                {
                    printf("INVALID\n");
                }
            }

            else
            {
                printf("INVALID\n");
            }
        }

        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}