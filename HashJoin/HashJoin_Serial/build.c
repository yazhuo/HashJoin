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

void build_process(int pk_param, int *pkeydata, int *hashtable, int *status_hashtable)
{
	int i = 0;
	while (i != TABLESIZE)
	{
		int index = doublehash(pkeydata[pk_param], i);
		if (status_hashtable[index] == 0)
		{
			hashtable[index] = pkeydata[pk_param];
			status_hashtable[index] = 1;
			break;
		}
		i++;
	}
}

void buildhash(int *pkeydata, int *hashtable, int pkeylen)
{
	int i;
	int *status_hashtable = (int *)malloc(sizeof(int) * TABLESIZE);
	memset(status_hashtable, 0, sizeof(int)* TABLESIZE);
	assert(status_hashtable);

	for (i = 0; i < pkeylen; i++)
		build_process(i, pkeydata, hashtable, status_hashtable);

	free(status_hashtable);
}