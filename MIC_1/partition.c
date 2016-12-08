#include "partition.h"
#include "global.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

void Hash_Partition(int *ary, int size, long *time)
{
	

	int *keyzonenum = (int*)malloc(sizeof(int) * size);
	assert(keyzonenum);
	memset(keyzonenum, 0, sizeof(int) * size);

	int *sub_hisgram = (int*)malloc(sizeof(int) * ZONENUM *ZONENUM);
	assert(sub_hisgram);
	memset(sub_hisgram, 0, sizeof(int) * ZONENUM * ZONENUM);

	int *glo_hisgram = (int*)malloc(sizeof(int) * ZONENUM);
	assert(glo_hisgram);
	memset(glo_hisgram, 0, sizeof(int) * ZONENUM);

	int *zone_begin = (int*)malloc(sizeof(int) * ZONENUM * ZONENUM);
	assert(zone_begin);
	memset(zone_begin, 0, sizeof(int) * ZONENUM * ZONENUM);

	struct timeval start, end;
	long cost_time;
	gettimeofday(&start, NULL);
	
	Init(keyzonenum, size, ary);
	Sub_Hisgram(keyzonenum, sub_hisgram, size);
	Glo_Hisgram(glo_hisgram, sub_hisgram);
	Glo_Addr(glo_hisgram);
	Sub_Addr(glo_hisgram, sub_hisgram, zone_begin);
	Zone_Key(ary, size, zone_begin, keyzonenum);

	
	
	gettimeofday(&end, NULL);
	cost_time = 1000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000;
	*time = cost_time;

	free(keyzonenum);
	free(sub_hisgram);
	free(glo_hisgram);
	free(zone_begin);
}

void Init(int *keyzonenum, int size, int *ary)
{
	int i;
#pragma omp parallel for num_threads(THREADNUM)
	for (i = 0; i < size; ++i)
		keyzonenum[i] = ary[i] % ZONENUM;
}

void Sub_Hisgram(int *keyzonenum, int *sub_hisgram, int size)
{
	int i;
	int zonesize = size / ZONENUM;

#pragma omp parallel for num_threads(THREADNUM)
	for (i = 0; i < ZONENUM; i++)
	{
		int begin = i * zonesize;
		int end = (i + 1) * zonesize;
		int his_begin = i * ZONENUM;
		int j;
		for (j = begin; j < end; j++)
			sub_hisgram[his_begin + keyzonenum[j]]++;
	}
}

void Glo_Hisgram(int *glo_hisgram, int *sub_hisgram)
{
	int i;
#pragma omp parallel for num_threads(THREADNUM)
	for (i = 0; i < ZONENUM; i++)
	{
		int j;
		for (j = 0; j < ZONENUM; j++)
		{
			int his_begin = j * ZONENUM;
			glo_hisgram[i] += sub_hisgram[his_begin + i];
		}
	}
}

void Glo_Addr(int *glo_hisgram)
{
	int i;
	for (i = 1; i < ZONENUM; ++i)
		glo_hisgram[i] += glo_hisgram[i - 1];
}

void Sub_Addr(int *glo_hisgram, int *sub_hisgram, int *zone_begin)
{
	int i;
#pragma omp parallel for num_threads(THREADNUM)
	for (i = 0; i < ZONENUM; i++)
	{
		int begin = ZONENUM * (ZONENUM - 1);
		zone_begin[begin + i] = glo_hisgram[i] - sub_hisgram[begin + i];
		int j;
		for (j = ZONENUM - 2; j >= 0; j--)
		{
			begin = ZONENUM * j;
			zone_begin[begin + i] = zone_begin[ZONENUM * (j + 1) + i] - sub_hisgram[begin + i];
		}
	}
}

void Zone_Key(int *ary, int size, int *zone_begin, int *keyzonenum)
{
	int *temp = (int*)malloc(sizeof(int) * size);
	assert(temp);
	memset(temp, 0, sizeof(int) * size);

	int zonesize = size / ZONENUM;
	int i;
#pragma omp parallel for num_threads(THREADNUM)
	for (i = 0; i < ZONENUM; i++)
	{
		int begin = i * zonesize;
		int end = (i + 1) * zonesize;
		int z_begin = i * ZONENUM;
		int j;
		for (j = begin; j < end; j++)
			temp[zone_begin[z_begin + keyzonenum[j]]++] = ary[j];
	}
	for (i = 0; i < size; i++)
		ary[i] = temp[i];
	free(temp);
}


