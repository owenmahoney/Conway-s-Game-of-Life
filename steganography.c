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
Color *evaluateOnePixel(Image *image, int row, int col)
{	

	int spot = (row) * (image->cols) + (col);
	Color * ptr = malloc(sizeof(Color));
	if (NULL == ptr){
            fprintf(stderr, "Out of memory.\n");
            exit(1);
        }
	
	if((image->image[spot]->B) & 1){
		ptr->R = 255;
		ptr->G = 255;
		ptr->B = 255;
	}else{
		ptr->R = 0;
		ptr->G = 0;
		ptr->B = 0;

	}
	return ptr;
}

//Given an image, creates a new image extracting the LSB of the B channel.
Image *steganography(Image *image)
{
	Image *newImg = malloc(sizeof(Image));
	if (NULL == newImg){
            fprintf(stderr, "Out of memory.\n");
            exit(1);
        }
	
	newImg->cols = image->cols;
	newImg->rows = image->rows;
	Color ** ptr = malloc(image->cols * image->rows * sizeof(Color*));
	if (NULL == ptr){
            fprintf(stderr, "Out of memory.\n");
            exit(1);
        }
	int row = 0;
	int col = 0;
	for(int i = 0; i < image->cols * image->rows; i ++){
                (ptr)[i] = evaluateOnePixel(image, row, col);
				col ++;
				if(col == (image->cols)){
					row++;
					col = 0;
				}
        }
	newImg->image = ptr;
	return newImg;

}

/*
Loads a .ppm from a file, and prints to stdout (e.g. with printf) a new image, 
where each pixel is black if the LSB of the B channel is 0, 
and white if the LSB of the B channel is 1.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!
*/
int main(int argc, char **argv)
{
	if(argc != 2){
		exit(-1);
	}
	
	char *filename = argv[1];
	Image *temp = readData(filename);
	Image *real = steganography(temp);
	writeData(real);
	freeImage(temp);
	freeImage(real);
	return 0;



	
}
