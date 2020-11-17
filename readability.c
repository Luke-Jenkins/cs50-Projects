//Determines reading level of given text using the Coleman-Liau formula
#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

//takes text and counts alpha chars
int count_letters(string text);
//takes text and counts words (via whitespace)
int count_words(string text);
//takes text and counts sentences (counts phrases that end with "." "?" or "!")
int count_sentences(string text);

//gets text and outputs number of alpha chars
int main(void)
{
    string text = get_string("Text: ");

    //average letters per 100 words
    float L = 100 * count_letters(text) / count_words(text);
    //average letters per 100 sentences
    float S = 100 * count_sentences(text) / count_words(text);
    //coleman-liau calculation
    int index = round(0.0588 * L - 0.296 * S - 15.8);

    //controls output
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %d\n", index);
    }

}

int count_letters(string text)
{
    //variable for placement in string
    int n = 0;
    //variable for number of non-alpha chars
    int nalpha = 0;
    while (text[n] != '\0')
    {
        if (isalpha(text[n]) == 0)
        {
            nalpha++;
        }
        n++;
    }
    //returns sum of all chars in string minus non-alpha chars
    return strlen(text) - nalpha;
}

int count_words(string text)
{
    //variable for placement in string
    int n = 0;
    //variable storing number of words
    int words = 0;
    while (text[n] != '\0')
    {
        //checks if char is whitespace and increases word count
        if (isspace(text[n]) != 0)
        {
            words++;
        }
        n++;
    }
    //adds one to word count to account for final word, not followed by a space
    return words + 1;
}

int count_sentences(string text)
{
    //variable for placement in string
    int n = 0;
    //variable storing number of sentences
    int sentence = 0;
    while (text[n] != '\0')
    {
        //checks if char is punctuation and increases sentence count
        if (text[n] == '.' || text[n] == '?' || text[n] == '!')
        {
            sentence++;
        }
        n++;
    }
    return sentence;
}