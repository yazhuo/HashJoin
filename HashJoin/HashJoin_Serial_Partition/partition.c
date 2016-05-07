#include "partition.h"
#include "global.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>

void hash_partition(int *ary, int size, int *keyzonenum, int *hisgram)
{
	init(keyzonenum, size, ary);
	scan1(keyzonenum, hisgram, size);
	begin_addr(hisgram);
	scan2(keyzonenum, hisgram, size, ary);
}

void init(int *keyzonenum, int size, int *ary)
{
	for (int i = 0; i < size; ++i)
		keyzonenum[i] = ary[i] % ZONENUM;
}

void scan1(int *keyzonenum, int *hisgram, int size)
{
	for (int i = 0; i < size; ++i)
		hisgram[keyzonenum[i]]++;
}

void begin_addr(int *hisgram)
{
	int sum = 0;
	int var = 0;
	for (int i = 0; i < ZONENUM; ++i)
	{
		var = hisgram[i];
		hisgram[i] = sum;
		sum += var;
	}
}

void scan2(int *keyzonenum, int *hisgram, int size, int *ary)
{
	int i;
	int *temp = (int*)malloc(sizeof(int) * size);
	assert(temp);
	for (i = 0; i < size; ++i)
		temp[hisgram[keyzonenum[i]]++] = ary[i];
	for (i = 0; i < size; ++i)
		ary[i] = temp[i];
	free(temp);
}