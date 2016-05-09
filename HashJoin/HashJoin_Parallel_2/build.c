#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <omp.h>
#include "build.h"

int hash1(int key)
{
	return key % TABLESIZE;
}

int hash2(int key)
{
	return (1 + (key % (TABLESIZE - 1)));
}

int doublehash(int key, int offset)
{
	return (hash1(key) + offset * hash2(key)) % TABLESIZE;
}

//void build_process(int hb_param, int pk_param, int *pkeydata, int *hashtable[ZONENUM], int *status_hashtable[ZONENUM])
//{
//	int i = 0;
//	while (i != TABLESIZE)
//	{
//		int index = doublehash(pkeydata[pk_param], i);
//		if (status_hashtable[hb_param][index] == 0)
//		{
//			hashtable[hb_param][index] = pkeydata[pk_param];
//			status_hashtable[hb_param][index] = 1;
//			break;
//		}
//		i++;
//	}
//}

void build_process(thread_param_t* data, int pk_param)
{
	int i = 0;
	while (i != TABLESIZE)
	{
		int index = doublehash(data->keydata[pk_param], i);
		if (data->status_hashtable[index] == 0)
		{
			data->hashtable[index] = data->keydata[pk_param];
			data->status_hashtable[index] = 1;
			break;
		}
		i++;
	}
}

void buildhash_with_zone(int *pkeydata, int *hashtable[ZONENUM], int *hisgram)
{
	thread_param_t data[ZONENUM];
	int *status_hashtable[ZONENUM];
	int i;
	for (i = 0; i < ZONENUM; i++)
	{
		status_hashtable[i] = (int*)malloc(sizeof(int) * TABLESIZE);
		assert(status_hashtable[i]);
		memset(status_hashtable[i], 0, TABLESIZE * sizeof(int));
	}

#pragma omp parallel for num_threads(8)
	for (i = 0; i < ZONENUM; i++)
	{
		printf("%d\n", i);

		int begin, end, j;
		begin = (i == 0) ? 0 : hisgram[i - 1];
		end = hisgram[i];
		data[i].zone_id = i;
		data[i].begin = begin;
		data[i].end = end;
		data[i].hashtable = hashtable[i];
		data[i].keydata = pkeydata;
		data[i].status_hashtable = status_hashtable[i];
		for (j = begin; j < end; j++)
		{
			build_process(&data[i], j);
		}
	}

	for (i = 0; i < ZONENUM; i++)
	{
		free(status_hashtable[i]);
	}
}