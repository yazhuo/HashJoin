/*
对HashJoin_Parallel_1不同的是，加入分区部分的并行化
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/time.h>
#include "global.h"
#include "loader.h"
#include "partition.h"
#include "build.h"
#include "probe.h"

int main(int argc, char **argv)
{
	printf("running...\n");
	int i;
	struct timeval start, end;
	long cost_time = 0;

	int pkeylen = datalen[0];
	int *pkeydata = (int*)malloc(sizeof(int) * pkeylen);
	assert(pkeydata != NULL);

	int fkeylen = datalen[1];
	int *fkeydata = (int*)malloc(sizeof(int) * fkeylen);
	assert(fkeydata != NULL);

	/* load DataKey and DataUniform data */
	loaddata(filenames, 0, pkeylen, pkeydata);
	loaddata(filenames, 1, fkeylen, fkeydata);

	int *p_hisgram = (int*)malloc(sizeof(int) * ZONENUM);
	assert(p_hisgram);
	memset(p_hisgram, 0, sizeof(int) * ZONENUM);
	int *p_zone_end = (int*)malloc(sizeof(int) * ZONENUM);
	assert(p_zone_end);
	memset(p_zone_end, 0, sizeof(int) * ZONENUM);
	int *p_zonekey = malloc(sizeof(int) * pkeylen);
	assert(p_zonekey);
	memset(p_zonekey, 0, sizeof(int) * pkeylen);

	printf("begin malloc on MIC...\n");
#pragma offload_transfer target(mic) in(p_hisgram:length(ZONENUM), p_zone_end:length(ZONENUM) alloc_if(1) free_if(0) )\
inout(p_zonekey:length(pkeylen) alloc_if(1) free_if(0)) 
	{}
	printf("begin partition on MIC...\n");
#pragma offload target(mic) in(pkeydata : length(pkeylen))
	{
#ifdef __MIC__
		struct timeval t1, t2;
		long cost_time = 0;
		gettimeofday(&t1, NULL);
		parallel_partition(pkeydata, pkeylen, p_hisgram, p_zone_end, p_zonekey);
		gettimeofday(&t2, NULL);
		cost_time = (t2.tv_sec - t1.tv_sec) * 1000 + (t2.tv_usec - t1.tv_usec) / 1000;
		printf("S partition on MIC time = %d\n", cost_time);

#else
		printf("fail to run on mic \n\n");
#endif
	}

	int *f_hisgram = (int*)malloc(sizeof(int) * ZONENUM);
	assert(f_hisgram);
	memset(f_hisgram, 0, sizeof(int) * ZONENUM);
	int *f_zone_end = (int*)malloc(sizeof(int) * ZONENUM);
	assert(f_zone_end);
	memset(f_zone_end, 0, sizeof(int) * ZONENUM);
	int *f_zonekey = malloc(sizeof(int) * fkeylen);
	assert(f_zonekey);
	memset(f_zonekey, 0, sizeof(int) * fkeylen);

	printf("begin partition on CPU...\n");
	gettimeofday(&start, NULL);
	parallel_partition(fkeydata, fkeylen, f_hisgram, f_zone_end, f_zonekey);
	gettimeofday(&end, NULL);
	cost_time = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
	printf("R partition time on CPU = %d\n", cost_time);

	int *hashtable[ZONENUM];
	for (i = 0; i < ZONENUM; i++)
	{
		hashtable[i] = malloc(sizeof(int) * TABLESIZE);
		assert(hashtable[i]);
		memset(hashtable[i], 0, TABLESIZE * sizeof(int));
	}

	printf("begin building hashtable...\n");
	gettimeofday(&start, NULL);
	buildhash_with_zone(pkeydata, hashtable, p_zone_end);
	gettimeofday(&end, NULL);
	cost_time = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
	printf("build hashtable time = %d\n", cost_time);

	printf("begin probe...\n");
	gettimeofday(&start, NULL);
	probe_with_zone(hashtable, fkeydata, f_zone_end);
	gettimeofday(&end, NULL);
	cost_time = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
	printf("probe time = %d\n", cost_time);


	for (i = 0; i < ZONENUM; i++)
		free(hashtable[i]);
	free(p_hisgram);
	free(f_hisgram);
	free(p_zone_end);
	free(f_zone_end);
	free(p_zonekey);
	//free(f_zonekey);
	free(pkeydata);
	free(fkeydata);

	printf("ending...\n");
	return 0;
}