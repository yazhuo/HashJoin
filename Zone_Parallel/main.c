#include "loader.h"
#include "global.h"
#include "join.h"
#include "partition.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>
#include <string.h>

int main(int argc, char **argv)
{
	printf("begining...\n");
	struct timeval start, end;
	long cost_time;

	int param1 = atoi(argv[1]);
	int param2 = atoi(argv[2]);

	int pkeylen = DATALEN[param1];
	int fkeylen = DATALEN[param2];

	printf("malloc space...\n");
	gettimeofday(&start, NULL);
	int *pkeydata = (int*)malloc(sizeof(int) * pkeylen);
	assert(pkeydata);
	int *fkeydata = (int*)malloc(sizeof(int) * fkeylen);
	assert(fkeydata);
	int *hashtable = (int*)malloc(sizeof(int) * TABLESIZE * ZONENUM);
	assert(hashtable);
	memset(hashtable, 0, sizeof(int) * TABLESIZE *ZONENUM);
	/*int *p_hisgram = (int*)malloc(sizeof(int) * ZONENUM);
	assert(p_hisgram);
	memset(p_hisgram, 0, sizeof(int) * ZONENUM);
	int *f_hisgram = (int*)malloc(sizeof(int) * ZONENUM);
	assert(f_hisgram);
	memset(f_hisgram, 0, sizeof(int) * ZONENUM);*/
	gettimeofday(&end, NULL);
	cost_time = 1000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000;
	printf("malloc time = %d\n", cost_time);

	printf("loading data...\n");
	gettimeofday(&start, NULL);
	loaddata(FILENAME, param1, pkeylen, pkeydata);
	loaddata(FILENAME, param2, fkeylen, fkeydata);
	gettimeofday(&end, NULL);
	cost_time = 1000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000;
	printf("loaddata time = %d\n", cost_time);

	printf("hash partition...\n");
	gettimeofday(&start, NULL);
	Hash_Partition(pkeydata, pkeylen);
	gettimeofday(&end, NULL);
	cost_time = 1000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000;
	printf("S partition time = %d\n", cost_time);

	gettimeofday(&start, NULL);
	Hash_Partition(fkeydata, fkeylen);
	gettimeofday(&end, NULL);
	cost_time = 1000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000;
	printf("R partition time = %d\n", cost_time);
	
	printf("hash join...\n");
	gettimeofday(&start, NULL);
	Hash_Join(pkeydata, fkeydata, hashtable, pkeylen, fkeylen);
	gettimeofday(&end, NULL);
	cost_time = 1000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000;
	printf("join time = %d\n", cost_time);

	printf("free space...\n");
	free(pkeydata);
	free(fkeydata);
	free(hashtable);

	printf("ending...\n");
	return 0;
}
