/*
 使用pthread进行多线程处理（未加锁处理冲突）
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include "global.h"
#include "loader.h"
#include "partition.h"
#include "build.h"
#include "probe.h"

int main(int argc, char **argv)
{
	printf("running...\n");
	clock_t t1, t2;
	int i;

	int pkeydata[] = { 1,2,3,4,5,6,7,8,9,10,
	11,12,13,14,15,16,17,18,19,20 };
	int pkeylen = sizeof(pkeydata)/sizeof(int);
	int fkeydata[] = { 1,2,3,4,5,6,7,8,9,10,
		1,2,3,4,5,6,7,8,9,10,
		11,12,13,14,15,16,17,18,19,20,
		11,12,13,14,15,16,17,18,19,20 };
	int fkeylen = sizeof(fkeydata) / sizeof(int);

	//int pkeylen = datalen[0];
	//int *pkeydata = (int*)malloc(sizeof(int) * pkeylen);
	//assert(pkeydata != NULL);

	//int fkeylen = datalen[3];
	//int *fkeydata = (int*)malloc(sizeof(int) * fkeylen);
	//assert(fkeydata != NULL);

	///* load DataKey and DataUniform data */
	//loaddata(filenames, 0, pkeylen, pkeydata);
	//loaddata(filenames, 3, fkeylen, fkeydata);
	//printf("pkeylen = %d pkeydata[0] = %d \n", pkeylen, pkeydata[0]);

	int *p_keyzonenum = (int*)malloc(sizeof(int) * pkeylen);
	assert(p_keyzonenum);
	int *p_hisgram = (int*)malloc(sizeof(int) * ZONENUM);
	assert(p_hisgram);
	memset(p_hisgram, 0, sizeof(int) * ZONENUM);
	int *f_keyzonenum = (int*)malloc(sizeof(int) * fkeylen);
	assert(f_keyzonenum);
	int *f_hisgram = (int*)malloc(sizeof(int) * ZONENUM);
	assert(f_hisgram);
	memset(f_hisgram, 0, sizeof(int) * ZONENUM);

	printf("begin hash partition...\n");
	t1 = clock();
	hash_partition(pkeydata, pkeylen, p_keyzonenum, p_hisgram);
	t2 = clock();
	printf("private data partition time = %f\n", (double)(t2 - t1) / CLOCKS_PER_SEC);

	t1 = clock();
	hash_partition(fkeydata, fkeylen, f_keyzonenum, f_hisgram);
	t2 = clock();
	printf("foreign data partition time = %f\n", (double)(t2 - t1) / CLOCKS_PER_SEC);

	/* malloc for build process */
	int *hashtable[ZONENUM];
	for (i = 0; i < ZONENUM; i++)
	{
		hashtable[i] = (int*)malloc(sizeof(int) * TABLESIZE);
		assert(hashtable[i]);
		memset(hashtable[i], 0, TABLESIZE * sizeof(int));
	}

	printf("begin building hashtable...\n");
	t1 = clock();
	buildhash_with_zone(pkeydata, hashtable, p_hisgram);
	t2 = clock();
	printf("build hashtable time = %f\n", (double)(t2 - t1) / CLOCKS_PER_SEC);

	int j;
	printf("hashtable: \n");
	for (i = 0; i < ZONENUM; i++)
	{
		printf("hashtable[%d]:\n", i);
		for (j = 0; j < TABLESIZE; j++)
		{
			printf("%d ", hashtable[i][j]);
		}
		printf("\n");
	}

	printf("begin probe...\n");
	t1 = clock();
	probe_with_zone(hashtable, fkeydata, f_hisgram);
	t2 = clock();
	printf("probe time = %f\n", (double)(t2 - t1) / CLOCKS_PER_SEC);

	for (i = 0; i < ZONENUM; i++)
		free(hashtable[i]);
	free(p_keyzonenum);
	free(p_hisgram);
	free(f_keyzonenum);
	free(f_hisgram);
//	free(pkeydata);
//	free(fkeydata);

	printf("ending...\n");
	return 0;
}