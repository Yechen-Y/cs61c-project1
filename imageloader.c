/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"

//Opens a .ppm P3 image file, and constructs an Image object. 
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename) 
{
	//YOUR CODE HERE
	FILE *fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("There is no a file");
		return NULL;
	}

	Image *aImage = (Image *) malloc(sizeof(Image));
	char format[3];
	int maxrange;
	fscanf(fp, "%s", format);
	if (format[0] != 'P' || format[1] != '3') {
		printf("the type is wrong");
		return NULL;
	}
	fscanf(fp, "%d %d", &aImage->rows, &aImage->cols);
	fscanf(fp, "%d", &maxrange);
	if (maxrange > 255 || maxrange < 0) {
		printf("The range is wrong");
		return NULL;
	}
	// Todo: *aImage应该malloc一个地址 总共要malloc rows * cols个地址，malloc后记得free！
	// Question: **aImage又该怎么找到下一个 *aImage的地址呢？类似数组？？ok！
	// tips: scanf 和 fscanf 都必须取地址
	int pixelSum = aImage->cols * aImage->rows;
	aImage->image = (Color **) malloc(pixelSum * sizeof(Color *));
	int count = 0;
	while (count < pixelSum) {
		*(aImage->image + count) = (Color *) malloc(sizeof(Color));
		Color *currentPixel = *(aImage->image + count);
		fscanf(fp, "%hhu %hhu %hhu", &currentPixel->R, &currentPixel->G, &currentPixel->B);
		count += 1;
	}
	fclose(fp);
	return aImage;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
// 0 1 2 3 row0 image->cols = 4
// 4 5 6 7 row1
void writeData(Image *image)
{
	//YOUR CODE HERE
	printf("P3\n%d %d\n255\n", image->rows, image->cols);
	int countRow, countCol;
	for (countRow = 0; countRow < image->rows; countRow++) {
		for (countCol = 0; countCol < image->cols; countCol ++) {
			Color *currentPixel = *(image->image + countRow * image->cols + countCol);
			if (countCol == image->cols - 1) {
				printf("%3hhu %3hhu %3hhu", currentPixel->R, currentPixel->G, currentPixel->B);
			} else {
				printf("%3hhu %3hhu %3hhu   ", currentPixel->R, currentPixel->G, currentPixel->B);
			}
		}
		printf("\n");
	}
}

//Frees an image
// 需要free的空间有三部份 Color Color** 和Image
void freeImage(Image *image)
{
	//YOUR CODE HERE
	int pixelSum = image->cols * image->rows;
	int count;
	for (count = 0; count < pixelSum; count++) {
		free(*(image->image + count));
	}
	free(image->image);
	free(image);
}