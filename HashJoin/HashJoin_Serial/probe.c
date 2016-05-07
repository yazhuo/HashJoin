#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "probe.h"

int probe_process(int fk_param, int *fkeydata, int *hashtable, int *status_hashtable)
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

void probe(int *hashtable, int *fkeydata, int fkeylen)
{
	int i;
	int *status_hashtable = (int *)malloc(sizeof(int) * TABLESIZE);
	memset(status_hashtable, 0, sizeof(int));
	assert(status_hashtable);

	for (i = 0; i < fkeylen; i++)
		probe_process(i, fkeydata, hashtable, status_hashtable);

	free(status_hashtable);
}
