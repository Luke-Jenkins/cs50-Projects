#include <stdio.h>
#include <cs50.h>
//prints two mirrored, n by n pyramids with space between them
int main(void)
{
    //variable: pyramid size
    int n;
    //variable; spaces per line
    int s;
    //user defines size of pyramid
    do
    {
        n = get_int("Height of each pyramid: ");
    }
    //ensure user enters a positive integer between 1 & 8
    while (n < 1 || n > 8);
    
    //build the pyramid
    //build the rows
    for (int i = 1; i < n + 1; i++)
    {
        //calculate spaces
        for (s = 0; s < n - i; s++)
        {
            printf(" ");
        }
        //calculate left blocks
        for (int j = 0; j < n - s; j++)
        {
            printf("#");
        }
        
        //print gap
        printf("  ");
        
        //calculate right blocks
        for (int j = 0; j < n - s; j++)
        {
            printf("#");
        }
        //build columns
        printf("\n");
    }
}