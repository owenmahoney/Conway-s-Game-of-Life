/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.
int checkbits(uint8_t neighbors[], int bitnum)
{

	int alive = 0;
	for (int i =0; i < 8; i++){
		if(neighbors[i] & (1 << bitnum)){
			alive++;
		}

	}
	return alive;
}
uint8_t pixLoop(uint8_t pixel, uint8_t neighbors[], uint32_t rule){
	for (int i =0; i < 8; i++){
		//printf("pix number %d and i number %d\n", pixel, i);
		int curr =  (pixel & (1 << i)) != 0;
		//printf("curr bit dead or alive: %d\n", curr);
		int alive = checkbits(neighbors, i);
		//printf("num alive %d\n", alive);
		if(curr){
			//int r = (rule & (1 << (alive + 9))) != 0;
			//printf("curr bit of rule: %d\n", r);
			if(!(rule & (1 << (alive + 9)))){
				//printf("prev pixel num %d\n", pixel);
				pixel = pixel ^ (1 << i);
				//printf("new pixel num %d\n", pixel);
			}
		}else{
			if(rule & (1 << (alive))){
				//printf("prev pixel num %d\n", pixel);
				pixel = pixel ^ (1 << i);
				//printf("new pixel num %d\n", pixel);
		
		}
	}
}
return pixel;
}


Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
	int spot = (row) * (image->cols) + (col);
	
	int neighbors[8];
	
	neighbors[0] =  (row) * (image->cols) + ((col-1 + image->cols) % image->cols);
	neighbors[1] = 	(row) * (image->cols) + ((col+1 + image->cols) % image->cols);
	neighbors[2] = (((row-1 + image->rows) % image->rows) * (image->cols) + col);
	neighbors[3] = (((row+1 + image->rows) % image->rows) * (image->cols) + col);
	neighbors[4] = (((row+1 + image->rows) % image->rows) * (image->cols) + ((col-1 + image->cols) % image->cols));
	neighbors[5] = (((row+1 + image->rows) % image->rows) * (image->cols) + ((col+1 + image->cols) % image->cols));
	neighbors[6] = (((row-1 + image->rows) % image->rows) * (image->cols) + ((col-1 + image->cols) % image->cols));
	neighbors[7] = (((row-1 + image->rows) % image->rows) * (image->cols) + ((col+1 + image->cols) % image->cols)); 

	Color * ptr = malloc(sizeof(Color));
	if (NULL == ptr){
            fprintf(stderr, "Out of memory.\n");
            exit(1);
        }
	
	uint8_t R = image->image[spot]->R;
	uint8_t G = image->image[spot]->G;
	uint8_t B = image->image[spot]->B;
	uint8_t Rarr[8];
	for (int i =0; i < 8; i++){ 
		int spot = neighbors[i];
		Rarr[i]= image->image[spot]->R;
	}
	uint8_t Garr[8];
	for (int i =0; i < 8; i++){ 
		int spot = neighbors[i];
		Garr[i]= image->image[spot]->G;
	}v
	uint8_t Barr[8];
	for (int i =0; i < 8; i++){ 
		int spot = neighbors[i];
		Barr[i]= image->image[spot]->B;
	}
	R = pixLoop(R, Rarr, rule);
	G = pixLoop(G, Garr, rule);
	B = pixLoop(B, Barr, rule);
	ptr->R = R;
	ptr->G = G;
	ptr->B = B;
	
	
		
	return ptr;
}



//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
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
                (ptr)[i] = evaluateOneCell(image, row, col, rule);
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
		if(argc != 3){
		printf("usage: ./gameOfLife filename rule\n");
		printf("filename is an ASCII PPM file (type P3) with maximum value 255.\n");
		printf("rule is a hex number beginning with 0x; Life is 0x1808.\n");
		exit(-1);
	}
	char *filename = argv[1];
	char *ptr;
	uint32_t rule = strtol(argv[2], &ptr, 16);
	Image *temp = readData(filename);
	Image *real = life(temp, rule);
	writeData(real);
	freeImage(temp);
	freeImage(real);
	return 0;

}
