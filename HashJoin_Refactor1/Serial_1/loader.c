#include "loader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void loaddata(char filename[][NAMEMAX], int index, int datalen, int *arr)
{
	FILE *fpin;
	char file_dir[256] = "..\\..\\DataSet\\";
	strcat(file_dir, filename[index]);

	printf("opening file %s\n", filename[index]);
	fpin = fopen(file_dir, "rb");
	if (fpin == NULL)
	{
		printf("fopen for fpin error\n");
		exit(1);
	}

	printf("reading file %s\n", filename[index]);
	if (fread(arr, sizeof(int), datalen, fpin) != datalen)
	{
		printf("fread error\n");
		exit(1);
	}

	fclose(fpin);
}