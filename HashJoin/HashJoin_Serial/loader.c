#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "loader.h"

void loaddata(char filenames[][NAMEMAX], int index, int datalen, int *arr)
{
	FILE *fpin;
	char file_dir[256] = "..\\DataSet\\";
	strcat(file_dir, filenames[index]);
	//open data files
	printf("opening files %s\n", filenames[index]);
	fpin = fopen(file_dir, "rb");
	if (fpin == NULL)
	{
		printf("fopen for fpin error\n");
		exit(1);
	}

	//read data from data files to array
	printf("reading files %s\n", filenames[index]);
	if (fread(arr, sizeof(int), datalen, fpin) != datalen)
	{
		printf("fread error\n");
		exit(1);
	}

	fclose(fpin);
}
