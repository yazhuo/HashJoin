#include "gendata.h"
#include <stdio.h>
#include <stdlib.h>

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void gen_pridata(int *data)
{
    int i;
    for(i = 0; i < boundry_b - boundry_a; ++i)
        data[i] = i + boundry_a;
    for(i = 0; i < DATALEN; ++i)
        swap(&data[i], &data[rand() % (boundry_b - boundry_a - i) + i]);
    
    printf("creating private data file \"%s\"...\n", FILENAME);
    
    FILE *fp;
    fp = fopen(FILENAME, "wb");
    if(fp == NULL)
    {
        printf("writing private data file \"%s\" error \n", FILENAME);
        exit(1);
    }
    
    printf("writing private data file \"%s\"...\n", FILENAME);
    
    if(fwrite(data, sizeof(int), DATALEN, fp) != DATALEN)
    {
        printf("fwrite error\n");
        exit(1);
    }
    
    fclose(fp);
    printf("genpdata done\n");
}