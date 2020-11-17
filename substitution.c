//takes in key from cmd line & plaintext from user; outputs ciphertext
#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

//function validates key
int validate(string key);
//function enciphers plaintext
int encipher(string key, string plain);

int main(int argc, string argv[])
{
    //stores key in variable
    string key = argv[1];

    if (argc > 2 || argc == 1)
    {
        printf("Please enter one key after './substitution'. This key may not include spaces.\ni.e.: ./substitution key\n");
        return 1;
    }
    else if (validate(key) != 0)
    {
        return 1;
    }
    //gets plaintext
    string plain = get_string("plaintext: ");
    //printf("%s\n", plain);

    //outputs ciphertext
    encipher(key, plain);
    //returns 0 if successful
    return 0;
}

int validate(string key)
{
    //variable contains length of string
    int n = strlen(key);

    if (n != 26)
    {
        printf("Key must contain 26 characaters.\n");
        return 1;
    }

    for (int i = 0; i < n; i++)
    {
        if (isalpha(key[i]) == 0)
        {
            printf("Key must be only alphabetic characters.\n");
            return 1;
        }
        for (int j = i + 1; j < n; j++)
        {
            if (key[i] == key[j] || key[i] == key[j] - 32)
            {
                printf("Key must not contain repeated characters.\n");
                return 1;
            }
        }
    }
    //returns 0 if successful
    return 0;
}

//function enciphers plaintext
int encipher(string key, string plain)
{
    //cvliukjvluihglkdhgvihgidjvdmbhnjvgfugynsjxbhdybdhidgdlhdf;hid

    //string containing alphabet for key mapping
    string alpha = "abcdefghijklmnopqrstuvwxyz";

    printf("ciphertext: ");

    //converts plaintext to ciphertext
    for (int i = 0, n = strlen(plain); i < n; i++)
    {
        //checks for alphabetical letters
        if (isalpha(plain[i]) != 0)
        {
            //checks for letter in the alphabet, maps to key letter via string placement
            for (int j = 0, m = strlen(alpha); j < m; j++)
            {
                if (plain[i] == alpha[j] || plain[i] == alpha[j] - 32)
                {
                    //checks for case
                    if (isupper(plain[i]) != 0)
                    {
                        printf("%c", toupper(key[j]));
                    }
                    else if (islower(plain[i]) != 0)
                    {
                        printf("%c", tolower(key[j]));
                    }
                }
            }
        }
        else
        {
            printf("%c", plain[i]);
        }
    }
    printf("\n");
    return 0;
}