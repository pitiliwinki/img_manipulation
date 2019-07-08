/**
* @file image_copy.c
* @brief This programs opens a BMP image, reads the header, colortable, and image data
	and stores it in a new file. Makes a copy of the image.
* @author Iñaki Garcia
* @version v0.01
* @date 2019-07-08
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Header.h"

int main(int argc, char* argv[])
{
	clock_t start, stop;

	start = clock(); // Note the start time for profiling purposes.

	FILE* fo = fopen("images/lena_copy.bmp", "wb"); // Output File name

	int i;

	FILE* streamIn;
	streamIn = fopen("images/lena512.bmp", "rb"); // Input file name

	if (streamIn == (FILE*)0) // check if the input file has not been opened succesfully.
	{
		printf("File opening error ocurred. Exiting program.\n");
		exit(0);
	}

	unsigned char header[54]; // to store the image header
	unsigned char colorTable[1024]; // to store the colorTable, if it exists.

	int count = 0;
	for (i = 0; i < 54; i++)
	{
		header[i] = getc(streamIn);  // strip out BMP header

	}
	int width = *(int*)& header[18]; // read the width from the image header
	int height = *(int*)& header[22]; // read the height from the image header
	int bitDepth = *(int*)& header[28]; // read the bitDepth from the image header

	if (bitDepth <= 8)
		fread(colorTable, sizeof(unsigned char), 1024, streamIn);


	printf("width: %d\n", width);
	printf("height: %d\n", height);

	fwrite(header, sizeof(unsigned char), 54, fo); // write the image header to output file

	// unsigned char buf[height * width]; // to store the image data

	char* buf = NULL;

	buf = (char*)malloc(width * height);

	if (buf == NULL)
	{
		printf("Failed to allocate memory \n");
	}


	fread(buf, sizeof(unsigned char), (height * width), streamIn);

	if (bitDepth <= 8)
		fwrite(colorTable, sizeof(unsigned char), 1024, fo);

	fwrite(buf, sizeof(unsigned char), (height * width), fo);

	fclose(fo);
	fclose(streamIn);

	free(buf);

	stop = clock();

	printf("Time: %lf ms\n", ((double)(stop - start) * 1000.0) / CLOCKS_PER_SEC);

	getchar();

	negative_image();
}

void negative_image()
{
	clock_t start, stop;
	start = clock();
	FILE* fp = fopen("images/lena512.bmp", "rb");   //read the file//

	unsigned char* imageData; // to store the image information
	unsigned char* newimageData; // to store the new image information, the negative image
	unsigned char imageHeader[54]; // to get the image header
	unsigned char colorTable[1024]; // to get the colortable

	int i, j; // loop counter variables

	fread(imageHeader, sizeof(unsigned char), 54, fp); // read the 54-byte from fp to imageHeader


	// extract image height and width from imageHeader      
	int width = *(int*)& imageHeader[18];
	int height = *(int*)& imageHeader[22];
	int bitDepth = *(int*)& imageHeader[28];

	int imgDataSize = width * height; // calculate image size

	imageData = (unsigned char*)malloc(imgDataSize * sizeof(unsigned char)); // allocate the block of memory as big as the image size
	newimageData = (unsigned char*)malloc(imgDataSize * sizeof(unsigned char));

	if (bitDepth <= 8)	// COLOR TABLE Present
		fread(colorTable, sizeof(unsigned char), 1024, fp); // read the 1024-byte from fp to colorTable


	fread(imageData, sizeof(unsigned char), imgDataSize, fp);


	//Calculate the mean of the image
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			newimageData[i * width + j] = 255 - imageData[i * width + j];
		}
	}

	FILE* fo = fopen("images/lena_negative.bmp", "wb");

	fwrite(imageHeader, sizeof(unsigned char), 54, fo); // write the header back.

	if (bitDepth <= 8)	// COLOR TABLE Present
		fwrite(colorTable, sizeof(unsigned char), 1024, fo); // write the color table back

	fwrite(newimageData, sizeof(unsigned char), imgDataSize, fo); // write the values of the negative image.

	fclose(fo);
	fclose(fp);
	free(imageData);
	free(newimageData);
	stop = clock();
	double d = (double)(stop - start) * 1000.0 / CLOCKS_PER_SEC;
	printf("%lf\n", d);

	getchar();
}