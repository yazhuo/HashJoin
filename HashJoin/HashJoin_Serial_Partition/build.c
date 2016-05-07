#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
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

void build_process(int hb_param, int pk_param, int *pkeydata, int *hashtable[ZONENUM], int *status_hashtable[ZONENUM])
{
	int i = 0;
	while (i != TABLESIZE)
	{
		int index = doublehash(pkeydata[pk_param], i);
		if (status_hashtable[hb_param][index] == 0)
		{
			hashtable[hb_param][index] = pkeydata[pk_param];
			status_hashtable[hb_param][index] = 1;
			break;
		}
		i++;
	}
}

void buildhash_with_zone(int *pkeydata, int *hashtable[ZONENUM], int *hisgram)
{
	int *status_hashtable[ZONENUM];
	int i, j;
	for (i = 0; i < ZONENUM; i++)
	{
		status_hashtable[i] = (int*)malloc(sizeof(int) * TABLESIZE);
		assert(status_hashtable[i]);
		memset(status_hashtable[i], 0, TABLESIZE * sizeof(int));
	}

	int begin = 0;
	int end;
	for (i = 0; i < ZONENUM; i++)
	{
		end = hisgram[i];
		for (j = begin; j < end; j++)
			build_process(i, j, pkeydata, hashtable, status_hashtable);
		begin = end;
	}
}