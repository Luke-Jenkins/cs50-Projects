#include "helpers.h"
#include <math.h>

int Gx(RGBTRIPLE box[3][3], char c);
int Gy(RGBTRIPLE box[3][3], char c);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    //iterates through height
    for (int i = 0; i < height; i++)
    {
        //iterates through width
        for (int j = 0; j < width; j++)
        {
            //determine avgerage of rgbt channels
            float average = ((float) image[i][j].rgbtRed + (float) image[i][j].rgbtGreen + (float) image[i][j].rgbtBlue) / 3;

            //set each channel to average value
            image[i][j].rgbtRed = round(average);
            image[i][j].rgbtGreen = round(average);
            image[i][j].rgbtBlue  = round(average);
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    //variables for storing row data as it's reflected
    RGBTRIPLE tempRow[width]; //array temporarily holding the value of reflected row

    //iterates through height (rows)
    for (int i = 0; i < height; i++)
    {
        //resets k pointer
        int k = 0; //pointer for moving through tempRow[]

        //iterates bakwards through width (columns)
        for (int j = width - 1; j >= 0; j--)
        {
            //assigns image value to tempRow[] backward
            tempRow[k] = image[i][j];
            k++;
        }

        //resets k pointer
        k = 0;

        //iterates through width (columns)
        for (int j = 0; j < width; j++)
        {
            //assigns tempRow value to image[][] in order
            image[i][j] = tempRow[k];
            k++;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //RGBTRIPLE box[9]; //stores valid pixel values

    //stores row values while they're blurred
    RGBTRIPLE temp[height][width];

    //iterates through height
    for (int i = 0; i < height; i++)
    {
        float pixCount; //counts number of valid pixels surrounding source

        //iterates through width
        for (int j = 0; j < width; j++)
        {
            //resets pixCount
            pixCount = 0.0;

            //values for calculating box average
            float avgRed = 0;
            float avgGreen = 0;
            float avgBlue = 0;

            //iterates through 9 pixels boxing in source
            //iterates rows
            for (int k = i - 1; k <= i + 1; k++)
            {
                //iterates columns
                for (int l = j - 1; l <= j + 1; l++)
                {
                    //checks if pixel is inside image boundries
                    if (l >= 0 && l <= width - 1 && k >= 0 && k <= height - 1)
                    {
                        //sums each color channel & increases valid pixel count
                        avgRed = avgRed + (float) image[k][l].rgbtRed;
                        avgGreen = avgGreen + (float) image[k][l].rgbtGreen;
                        avgBlue = avgBlue + (float) image[k][l].rgbtBlue;
                        pixCount++;
                    }
                }
            }

            //calculates avgerage of up to 9 surrounding pixels
            avgRed = avgRed / pixCount;
            avgGreen = avgGreen / pixCount;
            avgBlue = avgBlue / pixCount;

            //assigns averages of each color channel to temp[][] row
            temp[i][j].rgbtRed = round(avgRed);
            temp[i][j].rgbtGreen = round(avgGreen);
            temp[i][j].rgbtBlue = round(avgBlue);
        }
    }

    //copies temp[][] to original image[][]
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = temp[i][j];
        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    //temporary arrays while original is filtered
    RGBTRIPLE temp[height][width]; //array for whole image
    RGBTRIPLE box[3][3]; //array for 9 pixels around source
    int h; //move through box height (rows)
    int w; //move through box width (columns)

    //iterates through height (rows)
    for (int i = 0; i < height; i++)
    {
        //iterates through width (columns)
        for (int j = 0; j < width; j++)
        {
            //resets h
            h = 0;

            //iterates through 9 pixels boxing in source
            //iterates rows
            for (int k = i - 1; k <= i + 1; k++)
            {
                //resets w
                w = 0;

                //iterates columns
                for (int l = j - 1; l <= j + 1; l++)
                {
                    //checks if pixel is inside image boundries
                    if (l < 0 || l > width - 1 || k < 0 || k > height - 1)
                    {
                        //assigns pixels outside image[][] to box as black
                        box[h][w].rgbtRed = 0;
                        box[h][w].rgbtGreen = 0;
                        box[h][w].rgbtBlue = 0;

                        w++; //next column
                    }
                    else
                    {
                        box[h][w].rgbtRed = image[k][l].rgbtRed;
                        box[h][w].rgbtGreen = image[k][l].rgbtGreen;
                        box[h][w].rgbtBlue = image[k][l].rgbtBlue;

                        w++; //next column
                    }
                }

                h++; //next row
            }

            //calculate Gx (x-axis) convulutional matrices
            int Gxr = Gx(box, 'r');
            int Gxg = Gx(box, 'g');
            int Gxb = Gx(box, 'b');

            //calculate Gy (y-axis) convolutional matrices
            int Gyr = Gy(box, 'r');
            int Gyg = Gy(box, 'g');
            int Gyb = Gy(box, 'b');

            //calculate Sobel for each channel and assign to temp[][]
            int sobelr = llround(sqrt(pow(Gxr, 2) + pow(Gyr, 2)));
            int sobelg = llround(sqrt(pow(Gxg, 2) + pow(Gyg, 2)));
            int sobelb = llround(sqrt(pow(Gxb, 2) + pow(Gyb, 2)));

            //sets temp[][] pixel to Sobel, unless Sobel > 255
            if (sobelr > 255)
            {
                temp[i][j].rgbtRed = 255;
            }
            else
            {
                temp[i][j].rgbtRed = (int) sobelr;
            }

            if (sobelg > 255)
            {
                temp[i][j].rgbtGreen = 255;
            }
            else
            {
                temp[i][j].rgbtGreen = (int) sobelg;
            }

            if (sobelb > 255)
            {
                temp[i][j].rgbtBlue = 255;
            }
            else
            {
                temp[i][j].rgbtBlue = (int) sobelb;
            }
        }
    }

    //copies temp[][] to original image[][]
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = temp[i][j];
        }
    }

    return;
}

//student function
//takes in RGBTRIPLE array of 9 pixles
//& char key indicating which color channel to calculate
int Gx(RGBTRIPLE box[3][3], char c)
{
    //MAX for convelutional matrix
    int MAX = 3;
    //Kernal: convelutional matrices
    int Kx[MAX][MAX]; //for x axis

    //assigns values to Kx
    Kx[0][0] = -1;
    Kx[0][1] = 0;
    Kx[0][2] = 1;
    Kx[1][0] = -2;
    Kx[1][1] = 0;
    Kx[1][2] = 2;
    Kx[2][0] = -1;
    Kx[2][1] = 0;
    Kx[2][2] = 1;

    //value to be returned
    int gx = 0;

    //iterates through height (rows)
    for (int i = 0; i < MAX; i++)
    {
        //iterates through width (columns)
        for (int j = 0; j < MAX; j++)
        {
            //checks for color, then calculates gx for that channel
            if (c == 'r')
            {
                gx = gx + (Kx[i][j] * box[i][j].rgbtRed);
            }
            else if (c == 'g')
            {
                gx = gx + (Kx[i][j] * box[i][j].rgbtGreen);
            }
            else if (c == 'b')
            {
                gx = gx + (Kx[i][j] * box[i][j].rgbtBlue);
            }
        }
    }

    return gx;
}

//student function
//takes in RGBTRIPLE array of 9 pixles
//& char key indicating which color channel to calculate
int Gy(RGBTRIPLE box[3][3], char c)
{
    //MAX for convelutional matrix
    int MAX = 3;
    //Kernal: convelutional matrix
    int Ky[MAX][MAX]; //for y axis

    //assigns values to Ky
    Ky[0][0] = -1;
    Ky[0][1] = -2;
    Ky[0][2] = -1;
    Ky[1][0] = 0;
    Ky[1][1] = 0;
    Ky[1][2] = 0;
    Ky[2][0] = 1;
    Ky[2][1] = 2;
    Ky[2][2] = 1;

    //value to be returned
    int gy = 0;

    //iterates through height (rows)
    for (int i = 0; i < MAX; i++)
    {
        //iterates through width (columns)
        for (int j = 0; j < MAX; j++)
        {
            //checks for color, then calculates gx for that channel
            if (c == 'r')
            {
                gy = gy + (Ky[i][j] * box[i][j].rgbtRed);
            }
            else if (c == 'g')
            {
                gy = gy + (Ky[i][j] * box[i][j].rgbtGreen);
            }
            else if (c == 'b')
            {
                gy = gy + (Ky[i][j] * box[i][j].rgbtBlue);
            }
        }
    }

    return gy;
}