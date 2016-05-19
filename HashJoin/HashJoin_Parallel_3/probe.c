#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include "probe.h"
#include <assert.h>

void * probe_process_thread(void *p)
{
	thread_param_t data = *(thread_param_t *)p;
	int j;
	for (j = data.begin; j < data.end; j++)
		probe_process(data.zone_id, j, data.keydata, data.hashtable, data.status_hashtable);

	pthread_exit(NULL);
	return NULL;
}

int probe_process(int hb_param, int fk_param, int *fkeydata, int *hashtable, int *status_hashtable)
{
	int i = 0;
	int index;
	while (i != TABLESIZE)
	{
		index = doublehash(fkeydata[fk_param], i);
		if (status_hashtable[index] == 0)
		{
			status_hashtable[index] = 1;
			break;
		}
		else if (fkeydata[fk_param] == hashtable[index])
			break;
		else
			i++;
	}
	return index;
}

void probe_with_zone(int *hashtable[ZONENUM], int *fkeydata, int *hisgram)
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
	for (i = 0; i < ZONENUM; i++)
	{
		end = hisgram[i];

		data[i].begin = begin;
		data[i].end = end;
		data[i].hashtable = hashtable[i];
		data[i].keydata = fkeydata;
		data[i].status_hashtable = status_hashtable[i];
		data[i].zone_id = i;

		rc = pthread_create(&threads[i], &attr, probe_process_thread, (void*)&data[i]);
		assert(!rc);

		begin = end;
	}

	for (i = 0; i < ZONENUM; i++)
	{
		rc = pthread_join(threads[i], &status);
		assert(!rc);
	}

	for (i = 0; i < ZONENUM; i++)
		free(status_hashtable[i]);
}