#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
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

void build_process(int hb_param, int pk_param, int *pkeydata, int *hashtable, int *status_hashtable)
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

void * build_process_thread(void *p)
{
	thread_param_t data = *(thread_param_t *)p;
	int j;
	for (j = data.begin; j < data.end; j++)
		build_process(data.zone_id, j, data.keydata, data.hashtable, data.status_hashtable);

	pthread_exit(NULL);
	return NULL;
}

void buildhash_with_zone(int *pkeydata, int *hashtable[ZONENUM], int *hisgram)
{
	pthread_t threads[ZONENUM];
	pthread_attr_t attr;
	thread_param_t data[ZONENUM];
	int rc;
	void *status;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	int *status_hashtable[ZONENUM];
	int i;
	for (i = 0; i < ZONENUM; i++)
	{
		status_hashtable[i] = (int*)malloc(sizeof(int) * TABLESIZE);
		assert(status_hashtable[i]);
		memset(status_hashtable[i], 0, TABLESIZE * sizeof(int));
	}

	int begin = 0;
	int end;
	//#pragma offload target(mic) in(hisgram:length(ZONENUM))
	for (i = 0; i < ZONENUM; i++)
	{
		end = hisgram[i];

		data[i].zone_id = i;
		data[i].begin = begin;
		data[i].end = end;
		data[i].hashtable = hashtable[i];
		data[i].keydata = pkeydata;
		data[i].status_hashtable = status_hashtable[i];

		rc = pthread_create(&threads[i], &attr, build_process_thread, (void*)&data[i]);
		assert(!rc);

		begin = end;
	}

	for (i = 0; i < ZONENUM; i++)
	{
		rc = pthread_join(threads[i], &status);
		assert(!rc);
	}

	for (i = 0; i < ZONENUM; i++)
	{
		free(status_hashtable[i]);
	}
}