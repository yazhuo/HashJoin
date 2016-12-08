#include "join.h"
#include "global.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/time.h>
#include <omp.h>

int hash1(int key)
{
	return key % TABLESIZE;
}

int hash2(int key)
{
	return (1 + (key % (TABLESIZE - 1)));
}

int HashFunc(int key, int offset)
{
	return (hash1(key) + offset * hash2(key)) % TABLESIZE;
}

void HashTable_Insert(int hb_param, int pk_param, int *pkeydata, int *hashtable, int *status_hashtable)
{
	int i = 0;
	int begin = hb_param * TABLESIZE;
	while (i != TABLESIZE)
	{
		int sub_index = HashFunc(pkeydata[pk_param], i);
		int glo_index = sub_index + begin;
		if (status_hashtable[glo_index] == 0)
		{
			hashtable[glo_index] = pkeydata[pk_param];
			status_hashtable[glo_index] = 1;
			break;
		}
		i++;
	}
}

void HashTable_Create(int *pkeydata, int *hashtable, int pkeylen)
{
	int i;
	int *status_hashtable = (int*)malloc(sizeof(int)*TABLESIZE*ZONENUM);
	assert(status_hashtable);
	memset(status_hashtable, 0, sizeof(int) * TABLESIZE*ZONENUM);

	int zonesize = pkeylen / ZONENUM;

#pragma omp parallel for num_threads(THREADNUM)
	for (i = 0; i < ZONENUM; i++)
	{
		int begin = i * zonesize;
		int end = (i + 1) * zonesize;
		int j;
		for (j = begin; j < end; j++)
			HashTable_Insert(i, j, pkeydata, hashtable, status_hashtable);
	}
	free(status_hashtable);
}

int HashTable_Find(int hb_param, int probekey, int *hashtable, int *status_hashtable)
{
	int i = 0;
	int sub_index, glo_index;
	int begin = hb_param * TABLESIZE;
	while (i != TABLESIZE)
	{
		sub_index = HashFunc(probekey, i);
		glo_index = begin + sub_index;
		if (status_hashtable[glo_index] == 0)
		{
			status_hashtable[glo_index] = 1;
			break;
		}
		else if (probekey == hashtable[glo_index])
			break;
		else
			i++;
	}
	return glo_index;
}

void Hash_Probe(int *fkeydata, int *hashtable, int fkeylen)
{
	int i;
	int *status_hashtable = (int*)malloc(sizeof(int) * TABLESIZE*ZONENUM);
	assert(status_hashtable);
	memset(status_hashtable, 0, sizeof(int) * TABLESIZE*ZONENUM);

	int zonesize = fkeylen / ZONENUM;

#pragma omp parallel for num_threads(THREADNUM) 
	for (i = 0; i < ZONENUM; i++)
	{
		int begin = i * zonesize;
		int end = (i + 1) * zonesize;
		int j;
		for (j = begin; j < end; j++)
		{
			int probekey = fkeydata[j];
			HashTable_Find(i, probekey, hashtable, status_hashtable);
		}
	}

	free(status_hashtable);
}

void Hash_Join(int *pkeydata, int *fkeydata, int *hashtable, int pkeylen, int fkeylen)
{
	struct timeval start, end;
	long cost_time;

	gettimeofday(&start, NULL);
	HashTable_Create(pkeydata, hashtable, pkeylen);
	gettimeofday(&end, NULL);
	cost_time = 1000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000;
	printf("buid hash time = %d\n", cost_time);

	gettimeofday(&start, NULL);
	Hash_Probe(fkeydata, hashtable, fkeylen);
	gettimeofday(&end, NULL);
	cost_time = 1000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000;
	printf("probe time = %d\n", cost_time);
}
