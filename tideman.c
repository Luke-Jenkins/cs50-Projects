#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

//Helper function prototypes
void merge_sort(pair array[], pair temp[], int start, int end);
bool cycle(pair array[], bool visited[], int w);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);
        printf("\n");
    }
    //test prefernces
    /*for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            printf("%s, %s preferences: %d\n", candidates[i], candidates[j], preferences[i][j]);
        }

        printf("\n");
    }*/


    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // TODO
    //Search for candidate using linear search
    for (int i = 0; i < candidate_count; i++)
    {
        if (!strcmp(candidates[i], name))
        {
            //assigns voter's rank to candidate via ranks[] array
            ranks[rank] = i;
            //test: printf("%s: %d\n", name, rank);
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO
    //loops through ranked candidate pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            //ensures no prefernce value is assigned to a pair of the same candidate (e.g. preferences[0][0])
            if (ranks[i] != ranks[j])
            {
                //increases preference[i][j] indicating voter preference of candidate [i] over candidate [j]
                preferences[ranks[i]][ranks[j]]++;
            }
            //test printf("%s preferences: %d\n", candidates[i], preferences[i][j]);
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
    //loops through candidate pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            //increases number of pairs with a winner/loser (excludes ties)
            if (preferences[i][j] > preferences[j][i])
            {
                //adds candidate pair to pairs[] array where [i] is .winner and [j] is .loser
                pairs[pair_count].winner = i; // test printf("winner: %s\n", candidates[pairs[pair_count].winner]);
                pairs[pair_count].loser = j; // test printf("loser: %s\n", candidates[pairs[pair_count].loser]);
                pair_count++; //increases count of pairs
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO
    //test
    /*printf("Original Pairs array: \n");
    for (int i = 0; i < pair_count; i++)
    {
        printf("Winner: %s | Loser: %s\n", candidates[pairs[i].winner], candidates[pairs[i].loser]);
    }*/

    //establishes temp array for sorting pairs
    pair temp_pairs[pair_count];

    //calls merge sorting function
    merge_sort(pairs, temp_pairs, 0, pair_count - 1);

    //test
    /*printf("Sorted Pairs array: \n");
    for (int i = 0; i < pair_count; i++)
    {
        printf("Winner: %s | Loser: %s\n", candidates[pairs[i].winner], candidates[pairs[i].loser]);
    }*/

    return;
}

//performs a merge sort
//found a supplemental explanation of merge-sorting at https://hackr.io/blog/merge-sort-in-c
void merge_sort(pair array[], pair temp[], int start, int end)
{
    //ends function when 1 or fewer items are left in array
    if (end <= start)
    {
        return;
    }

    else
    {
        //determines middle item of array to be sorted
        int mid = (start + end) / 2;

        //recursively sorts left half
        merge_sort(array, temp, start, mid);
        //recursively sorts right half
        merge_sort(array, temp, mid + 1, end);

        //pointers for comparing items in the left and right halves
        int l = start; //left pointer
        int r = mid + 1; //right pointer
        int k; //pointer for looking through new array

        //merge halves
        for (k = start; k <= end; k++)
        {
            if (l == mid + 1) // left pointer has reached the right
            {
                temp[k] = array[r];
                r++;
            }
            else if (r == end + 1) //right pointer has reached the end
            {
                temp[k] = array[l];
                l++;
            }

            //assigns array item with higher prefernce value to new array
            else if (preferences[array[l].winner][array[l].loser] - preferences[array[l].loser][array[l].winner] >
                     preferences[array[r].winner][array[r].loser] - preferences[array[r].loser][array[r].winner])
            {
                temp[k] = array[l];
                l++;
            }
            else
            {
                temp[k] = array[r];
                r++;
            }
        }

        //copies sorted temp[] to original array, pairs[]
        for (k = start; k <= end; k++)
        {
            array[k] = temp[k];
        }
    }

}
// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    //test
    /*printf("Pair preferences strength:\n");
    for (int i = 0 ; i < pair_count; i++)
    {
        printf("%s, %s: %d\n", candidates[pairs[i].winner], candidates[pairs[i].loser], preferences[pairs[i].winner][pairs[i].loser] - preferences[pairs[i].loser][pairs[i].winner]);
    }*/

    //tracks whether or not a candidate was visited in cycle() function
    bool visited[candidate_count];

    for (int i = 0; i < pair_count; i++)
    {
        //resets all visited[] values
        for (int j = 0; j < candidate_count; j++)
        {
            visited[j] = false;
        }
        //checks if locking pair will create a cycle
        //true == no || false == yes
        if (cycle(pairs, visited, i) == true)
        {
            //locks pair into graph
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }

    //test
    /*printf("Locked pairs:\n");
    for (int i = 0; i < pair_count; i++)
    {
        if (locked[pairs[i].winner][pairs[i].loser] == true)
        {
            printf("%s, %s\n", candidates[pairs[i].winner], candidates[pairs[i].loser]);
        }
    }*/

}

//helper function for lockpairs() that accepts arguments
//takes in pairs, a bool array, and a starting pointer (w)
//outputs false if locking this pair would create a cycle; otherwise outputs true
bool cycle(pair array[], bool visited[], int w)
{
    //stops recursion if winner has been visited before (i.e. creating a cycle)
    if (visited[array[w].loser] == true)
    {
        return false;
    }

    else
    {
        for (int i = 0; i < pair_count; i++)
        {
            //checks if loser of current pair is a winner in another pair
            if (array[w].loser == array[i].winner)
            {
                //if above is true, checks if other pair is locked[][]
                if (locked[array[i].winner][array[i].loser] == true)
                {
                    //if true checks if the other pair would cause a cycle
                    visited[array[w].winner] = true; //marks the starting winner as visited
                    if (cycle(array, visited, i) == false) //recursively calls function and checks for cycle
                    {
                        //exits function if cycle is created
                        return false;
                    }
                }
            }
        }
        //exits function if no cycle is created
        return true;
    }
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    //holds the number of locked losses a candidate has
    int losers[candidate_count];
    //sets loser[] values to zero
    for (int i = 0; i < candidate_count; i++)
    {
        losers[i] = 0;
    }

    //checks candidates against locked[][] & increases loser[]
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[i][j] == true)
            {
                losers[j]++;
            }
        }
    }

    //prints name of candidate with loser[] value of 0 (source of graph)
    for (int i = 0; i < candidate_count; i++)
    {
        if (losers[i] == 0)
        {
            printf("%s\n", candidates[i]);
        }
    }
    return;
}

