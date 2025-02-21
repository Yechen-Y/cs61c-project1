/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				Yechen
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

int countIndex(int row, int col, int maxRow, int maxCol);
void setBit(uint8_t *num, int flag, int bitNum);
void setRGB(uint8_t *newColor, uint8_t currentBits, Image *image, int ColorFlag, int *neighborsIndex, uint32_t rule);
//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.
// Todo: 1: 计算alive的领居数量 1.1: 计算index（注意边界条件）1.2 得到指定bit的值，如果为1则
// 增加alive邻居的数量 2:判断自身为alive or dead然后使用对应的rule来迭代
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
	//YOUR CODE HERE
	int *neighborsIndex = malloc(8 * sizeof(int));
	int *temp = neighborsIndex;
	Color *newColor = malloc(sizeof(Color));

	newColor->R = 1;
	newColor->G = 1;
	newColor->B = 1;

	// 计算index 其实是有瑕疵的 因为如果场上都没有8个或者是有重复的行或列。如何避免？或者设为一种限制？
	for (int countRow = -1; countRow < 2; countRow++) {
		for (int countCol = -1; countCol < 2; countCol++) {
			if (countRow == 0 && countCol == 0) {
				continue;
			}
			*(temp) = countIndex(row + countRow, col + countCol, image->rows, image->cols);
			temp++;
		}
	}
	int currentIndex = countIndex(row, col, image->rows, image->cols);
	setRGB(&(newColor->R), (*(image->image + currentIndex))->R, image, 0, neighborsIndex, rule);
	setRGB(&(newColor->G), (*(image->image + currentIndex))->G, image, 1, neighborsIndex, rule);
	setRGB(&(newColor->B), (*(image->image + currentIndex))->B, image, 2, neighborsIndex, rule);
	free(neighborsIndex);

	return newColor;
}

// 辅助函数，用来计算index, 包括各种边界条件
// 0 1 2
// 3 4 5  index4 = (row - 1) * cols + col - 1   and it should be row 2 col 2
int countIndex(int row, int col, int maxRow, int maxCol) {
	int tempRow, tempCol;
	tempRow = row;
	tempCol = col;
	if (row == 0) {
		tempRow = maxRow; 
	} 
	if (row > maxRow) {
		tempRow = 1;
	}
	if (col == 0) {
		tempCol = maxCol;
	}
	if (col > maxCol) {
		tempCol = 1;
	}

	return (tempRow - 1) * maxCol + tempCol - 1;
}

void setBit(uint8_t *num, int flag, int bitNum) {
	uint8_t helper = 1;
	helper <<= bitNum;
	if (flag == 1) {
		*num = *num | helper;
	} else {
		helper = ~helper;
		*num = *num & helper;
	}
}
// tips: ColorFlag 0-R 1-G 2-B
void setRGB(uint8_t *color, uint8_t currentBits, Image *image, int ColorFlag, int *index, uint32_t rule) {
	int state;
	int liveNeighbors;
	for (int count1 = 0; count1 < 8; count1++) {
		// uint8_t currentBits = (*(image->image))->R;
		liveNeighbors = 0;
		state = (currentBits >> count1) & 1;
		for (int count2 = 0; count2 < 8; count2++) {
			switch (ColorFlag)
			{
			case 0:
				if ((((*(image->image + index[count2]))->R) & 1) == 1) {
					liveNeighbors += 1;
				}
				break;
			case 1:
				if ((((*(image->image + index[count2]))->G) & 1) == 1) {
					liveNeighbors += 1;
				}
				break;
			case 2:
				if ((((*(image->image + index[count2]))->B) & 1) == 1) {
					liveNeighbors += 1;
				}
				break;
			default:
				break;
			}
			}
			if (state == 1) {
				state = (rule >> (9 + liveNeighbors)) & 1;
			} else {
				state = (rule >> liveNeighbors) & 1;
			}
			setBit(color, state, count1);
		}
	}


//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
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
			*(newImage->image + index) = evaluateOneCell(image, countRow, countCol, rule);
		}
	}

	return newImage;
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv)
{
	//YOUR CODE HERE
	if (argc != 3) {
		printf("usage: ./gameOfLife filename rule\nfilename is an ASCII PPM file (type P3) with maximum value 255.\nrule is a hex number beginning with 0x; Life is 0x1808.");
		return 1;
	}
	Image *img = readData(argv[1]);
	uint32_t rule = strtol(argv[2], NULL, 16);
	Image *nextImg = life(img, rule);
	writeData(nextImg);
	freeImage(img);
	freeImage(nextImg);
	return 0;
}
