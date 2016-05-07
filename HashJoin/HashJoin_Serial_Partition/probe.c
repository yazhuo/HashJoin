#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "probe.h"
#include <assert.h>

int probe_process(int hb_param, int fk_param, int *fkeydata, int *hashtable[ZONENUM], int *status_hashtable[ZONENUM])
{
	int i = 0;
	int index;
	while (i != TABLESIZE)
	{
		index = doublehash(fkeydata[fk_param], i);
		if (status_hashtable[hb_param][index] == 0)
		{
			status_hashtable[hb_param][index] = 1;
			break;
		}
		else if (fkeydata[fk_param] == hashtable[hb_param][index])
			break;
		else
			i++;
	}
	return index;
}

void probe_with_zone(int *hashtable[ZONENUM], int *fkeydata, int *hisgram)
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
			probe_process(i, j, fkeydata, hashtable, status_hashtable);
		begin = end;
	}

	for (i = 0; i < ZONENUM; i++)
		free(status_hashtable[i]);
}