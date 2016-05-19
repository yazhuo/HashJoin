#include <stdio.h>
#include <stdlib.h>
#include "genuniformdata.h"
#include <string.h>

void gen_fordata(int *data, int datalen, char *filename)
{
	char file_path[256] = "/home/b8310/yazhuo/Graduation/DataSet/";
	strcat(file_path, filename);
	int i;
    for(i = 0; i < datalen; i++)
        data[i] = (int)(rand() % fboundry_b);
    
    printf("creating foreign data file \"%s\"...\n", file_path);
    FILE *fp;
    fp = fopen(file_path, "wb");
    if(fp == NULL)
    {
        printf("writing foreign data file \"%s\" error \n", file_path);
        exit(1);
    }
    
    printf("writing foreign data file \"%s\"...\n", file_path);
    
    if(fwrite(data, sizeof(int), datalen, fp) != datalen)
    {
        printf("fwrite error\n");
        exit(1);
    }
    
    fclose(fp);
    printf("genfdata done\n");
}
