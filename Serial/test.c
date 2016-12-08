#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **atgv)
{
	int datalen = 1 << 24;
	int *arr = (int*)malloc(sizeof(int) * datalen);
	FILE *fpin;
	char file_dir[256] = "../../DataSet/";
	char *filename = "DataKey16M";
	strcat(file_dir, filename);
	fpin = fopen(file_dir, "rb");
	if(fpin == NULL)
	{
		printf("fopen for fpin error\n");
		exit(1);
	}

	printf("reading files %s\n", filename);
	if(fread(arr, sizeof(int), datalen, fpin) != datalen)
	{
		printf("fread error\n");
		exit(1);
	}

	fclose(fpin);
	return 0;
}
