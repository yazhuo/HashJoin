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

	long s_time = 0;
	Hash_Partition(pkeydata, pkeylen, &s_time);
	printf("S partition time = %d\n", s_time);

	long r_time = 0;
	Hash_Partition(fkeydata, fkeylen, &r_time);
	printf("R partition time = %d\n", r_time);

	gettimeofday(&end, NULL);
	cost_time = 1000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000;
	printf("partition time = %d\n", cost_time);

	printf("hash join...\n");
	
	long b_time = 0;
	long p_time = 0;
#pragma offload target(mic)\
	inout(pkeydata : length(pkeylen))\
	inout(fkeydata : length(fkeylen))\
	inout(hashtable : length(TABLESIZE *ZONENUM))
	Hash_Join(pkeydata, fkeydata, hashtable, pkeylen, fkeylen, &b_time, &p_time);
	printf("buid hash time = %d\n", b_time);
	printf("probe time = %d\n", p_time);
	

	printf("free space...\n");
	free(pkeydata);
	free(fkeydata);
	free(hashtable);

	printf("ending...\n");
	return 0;
}