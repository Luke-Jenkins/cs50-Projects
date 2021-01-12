#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    //test for number of commande line arguments (should be exactly 2)
    if (argc != 2)
    {
        //if too few arguments
        if (argc < 2)
        {
            printf("Too few arguments.\nUsage: ./recover [filename]\n");
            return 1;
        }
        //if too many arguments
        else if (argc > 2)
        {
            printf("Too many command-line arguments.\nUsage: ./recover [filename.raw]\n");
            return 1;
        }
    }

    //new type to store a single byte of data
    typedef uint8_t byte;

    //array of first 3 digits in jpeg header file
    byte jh[3];
    jh[0] = 0xff;
    jh[1] = 0xd8;
    jh[2] = 0xff;

    //data size variables
    byte BYTE = 1; //1 byte (B); 8 bits
    int BLOCK = 512; //512B; 4,096 bits

    //variables to create file names for each jpeg
    char *filename = malloc(BYTE * 8); //string to be printed as name for each jpeg
    int filecount = 0; //int for increasing digits in filename
    FILE *img = 0;//malloc(BLOCK); //creates pointer to empty memory for new jpegs

    //open memory card file
    FILE *f = fopen(argv[1], "r"); //pointer to memory card's data
    //if imaged can't be opened, exit program
    if (!f)
    {
        free(filename); //free malloced memory
        printf("Cannot open file.\nPlease check file address.\n");
        return 1;
    }

    //buffer to store BLOCK of data from memory card
    byte buffer[BLOCK];

    //read BLOCK of original file into buffer; temporary array to store the data
    while (fread(buffer, BYTE, BLOCK, f) != 0) //while fread() has not reached the end of memory card
    {
        //look through buffer file for jpeg header
        if (buffer[0] == jh[0] && buffer[1] == jh[1] && buffer[2] == jh[2] && (buffer[3] & 0xf0) == 0xe0)
        {
            //open new jpeg file to write
            if (filecount == 0) //the first time a jpeg is created
            {
                sprintf(filename, "%03i.jpg", filecount); //update filename
                img = fopen(filename, "w");//open new jpeg file
                filecount++; //increase filecount
                fwrite(buffer, BYTE, BLOCK, img); //write data to new jpeg file
            }
            else //every other time a jpeg is created
            {
                fclose(img); //close current image
                sprintf(filename, "%03i.jpg", filecount); //update filename
                filecount++; //increase filecount
                img = fopen(filename, "w"); //open new jpeg
                fwrite(buffer, BYTE, BLOCK, img); //write data to new jpeg
            }
        }
        else //if no header is found
        {
            if (img != 0) //a jpeg has already been found
            {
                //realloc(img, sizeof(*img) + BLOCK);
                fwrite(buffer, BYTE, BLOCK, img);
            }
        }
    }

    //close remaining files & exit
    free(filename);
    fclose(img);
    fclose(f);
    return 0;
}

/*open memory card
repeat until end of card:
    read 512 bytes into buffer
    if start of new jpeg:
        if first jpeg
            ...
        else
            ...
    else
        if already found jpeg
        ...
close any remaining files*/