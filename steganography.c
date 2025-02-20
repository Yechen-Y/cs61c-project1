/************************************************************************
**
** NAME:        steganography.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**				Justin Yokota - Starter Code
**				YOUR NAME HERE
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This should not affect Image, and should allocate space for a new Color.
// Todo: count a index with the given row/col.
// 0 1 2
// 3 4 5  index4 = (row - 1) * cols + col - 1   and it should be row 2 col 2
Color *evaluateOnePixel(Image *image, int row, int col)
{
	//YOUR CODE HERE
	Color *newColor = malloc(sizeof(Color));
	int index = (row - 1) * image->cols + col - 1;
	int color;

	color = (**(image->image + index)).B & 1; // B的最后一项为1则为1 vice versa
	if (color == 1) {
		newColor->R = 255;
		newColor->G = 255;
		newColor->B = 255;
	} else {
		newColor->R = 0;
		newColor->G = 0;
		newColor->B = 0;
	}
	return newColor;
}

//Given an image, creates a new image extracting the LSB of the B channel.
Image *steganography(Image *image)
{
	//YOUR CODE HERE
	int pixelSum = image->cols * image->rows;
	Image *newImage = malloc(sizeof(Image));
	newImage->cols = image->cols;
	newImage->rows = image->rows;
	newImage->image = malloc(pixelSum*sizeof(Color*));

	for (int countRow = 1; countRow < newImage->rows + 1; countRow++) {
		for (int countCol = 1; countCol < newImage->cols + 1; countCol++) {
			int index = (countRow - 1) * newImage->cols + countCol - 1;
			*(newImage->image + index) = evaluateOnePixel(image, countRow, countCol);
		}
	}

	return newImage;
}

/*
Loads a file of ppm P3 format from a file, and prints to stdout (e.g. with printf) a new image, 
where each pixel is black if the LSB of the B channel is 0, 
and white if the LSB of the B channel is 1.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a file of ppm P3 format (not necessarily with .ppm file extension).
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!
*/
int main(int argc, char **argv)
{
	//YOUR CODE HERE
	Image *aImage = readData(argv[1]);
	Image *newImage = steganography(aImage);

	writeData(newImage);
	free(aImage);
	free(newImage);
	return 0;
}
