#include "join.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/time.h>
#include <stdio.h>

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

void HashTable_Insert(int pk_param, int *pkeydata, int *hashtable, int *status_hashtable)
{
	int i = 0;
	while (i != TABLESIZE)
	{
		int index = HashFunc(pkeydata[pk_param], i);
		if (status_hashtable[index] == 0)
		{
			hashtable[index] = pkeydata[pk_param];
			status_hashtable[index] = 1;
			break;
		}
		i++;
	}
}

void HashTable_Create(int *pkeydata, int *hashtable, int pkeylen)
{
	int i;
	int *status_hashtable = (int*)malloc(sizeof(int) * TABLESIZE);
	assert(status_hashtable);
	memset(status_hashtable, 0, sizeof(int) * TABLESIZE);

	for (i = 0; i < pkeylen; i++)
		HashTable_Insert(i, pkeydata, hashtable, status_hashtable);

	free(status_hashtable);
}

int HashTable_Find(int probekey, int *hashtable, int *status_hashtable)
{
	int i = 0;
	int index;
	while (i != TABLESIZE)
	{
		 index = HashFunc(probekey, i);
		if (status_hashtable[index] == 0)
		{
			status_hashtable[index] = 1;
			break;
		}
		else if (probekey == hashtable[index])
			break;
		else
			i++;
	}
	return index;
}

void Hash_Probe(int *fkeydata, int *hashtable, int fkeylen)
{
	int i;
	int *status_hashtable = (int*)malloc(sizeof(int) * TABLESIZE);
	assert(status_hashtable);
	memset(status_hashtable, 0, sizeof(int) * TABLESIZE);

	for (i = 0; i < fkeylen; i++)
	{
		int probekey = fkeydata[i];
		HashTable_Find(probekey, hashtable, status_hashtable);
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
