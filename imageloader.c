/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				Owen Mahoney 
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
        Image *img = malloc(sizeof(Image));
        if(NULL == img){
                fprintf(stderr, "Out of memory.\n");
                exit(1);
        }
        FILE *fp; 
		fp = fopen(filename, "r");

		if(!fp){
                fprintf(stderr, "Cannot open file.\n");
                exit(1);
        }
		char format;
		char nump;
		fscanf(fp, "%c %c\n", &format, &nump);
		
	
		uint32_t rows = 0;
		uint32_t cols = 0;
		
        fscanf(fp, "%u %u\n", &cols, &rows);
		img->cols = cols;
		img->rows = rows;
		int rgbComp = 0;
		
		fscanf(fp, "%d", &rgbComp);
	
        uint32_t size =  img->cols * img->rows;
		
        
        Color ** ptr = malloc(size  * sizeof(Color*));

        if (NULL == ptr) {
                fprintf(stderr, "Out of memory.\n");
                exit(1);
        }
		uint8_t *R = 0;
		uint8_t *G = 0;
		uint8_t *B = 0;
        for(int i = 0; i < size; i ++){
                Color *num = malloc(sizeof(Color));
                if (NULL == num){
                        fprintf(stderr, "Out of memory.\n");
                        exit(1);
                }
                fscanf(fp, "%d %d %d", &R, &G, &B);
				num->R = R;
				num->G = G;
				num->B= B;
                (ptr)[i] = num;
        }
		
		img->image = ptr;
        fclose(fp);
        return img;
}



//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	

	printf("P3\n");
	printf("%u %u\n", image->cols, image->rows);
	printf("255\n");

	int col = 0; 

	for(int i = 0; i < image->cols * image->rows; i++) {
		
		printf("%3d %3d %3d", image->image[i]->R, image->image[i]->G, image->image[i]->B);
		

		col ++;
		if(col == image->cols){
			col = 0;
			printf("\n");
		}
		else{
			printf("   ");
		}


	}
	
}


//Frees an image
void freeImage(Image *image)
{
	if(image != NULL){
		for(int i = 0; i < image->cols * image->rows; i++){
			free(image->image[i]);
		}
		free(image->image);
		free(image);
	}
}