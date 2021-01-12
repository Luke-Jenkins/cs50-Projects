// Implements a dictionary's functionality

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 200000;

// Hash table
node *table[N];

//variable for number of words loaded from dictionary
unsigned int w_count = 0;

//variable counting hash table collisions
unsigned int collisions = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    //obtain hash value for word
    unsigned int index = hash(word);

    for (node *tmp = table[index]; tmp != NULL; tmp = tmp->next)
    {
        if (!strcasecmp(tmp->word, word))
        {
            return true;
        }
    }

    //if word is not in linked list, returns false
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO
    //hashes word using tweaked djb2 hash function: http://www.cse.yorku.ca/~oz/hash.html
    unsigned long index = 5381;
    int c;

    while ((c = *word++))
    {
        index = ((index << 5) + index) + toupper(c); /* index * 33 + c */
    }

    while (index > N - 1)
    {
        index = index % N;
    }

    return index;
    //return 0;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO

    //Open dictionary entered as argument into CL
    FILE *d = fopen(dictionary, "r");
    if (d == NULL) //checks for file validity
    {
        printf("Could not open %s.\n", dictionary);
        return false;
    }

    //set all table[] pointers to NULL
    memset(table, 0, N);

    //create temporary word buffer
    char *w = malloc(LENGTH + 1);
    if (w == NULL) //check that memory is available
    {
        printf("Out of memory.\n");
        free(w);
        return false; //exit if no memory is available
    }
    //read contents of dictionary one word at a time into buffer
    while (fscanf(d, "%s", w) != EOF)
    {

        //allocate memory for new node
        node *n = malloc(sizeof(node));
        if (n == NULL) //check that memory is available
        {
            printf("Out of memory.\n");
            free(n);
            free(w);
            return false; //exit if no memory is available
        }

        //copy string from buffer into node
        strcpy(n->word, w);
        n->next = NULL; //assign next pointer to NULL

        //hash string to return index
        unsigned int index = hash(n->word);

        //add node to linked list at table[index] location
        if (table[index] == NULL) //if this is first node
        {
            table[index] = n; //link table[index] to new node
        }
        else
        {
            n->next = table[index]; //link new node to first node at table[index]
            table[index] = n; //link table[index] to new node

            //test
            collisions++;
        }

        //increase word count
        w_count++;

    }
    //clean up file & memory usage
    free(w);
    fclose(d);

    //test
    printf("COLLISIONS: %d\n", collisions);

    //if successful, load() returns true
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    //checks if words have been added to w_count (i.e. dictionary == loaded)
    if (w_count > 0)
    {
        return w_count;
    }
    else //if w_count = 0 (i.e. dictionary != loaded), returns 0
    {
        return 0;
    }
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    //loops through hash table
    for (unsigned int index = 0; index < N; index++)
    {
        //frees each linked list
        while (table[index] != NULL)
        {
            node *tmp = table[index]->next;
            free(table[index]);
            table[index] = tmp;
        }

        //if linked list was not freed, exits program
        if (table[index] != NULL)
        {
            printf("Error unloading table[index]: %d.\n", index);
            return false;
        }
    }

    //if whole table successfully unloads
    return true;
}