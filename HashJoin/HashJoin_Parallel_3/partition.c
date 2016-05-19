#include "partition.h"
#include "global.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void * sub_hisgram_thread(void *p)
{
	thread_subhisgram_t zone = *(thread_subhisgram_t *)p;
	int *keyzonenum = (int*)malloc(sizeof(int) * zone.datalen);
	int i;
	int begin = zone.begin;

	for (i = 0; i < zone.datalen; ++i)
		keyzonenum[i] = zone.keydata[i + begin] % ZONENUM;

	for (i = 0; i < zone.datalen; i++)
		zone.hisgram[keyzonenum[i]]++;

	free(keyzonenum);
	pthread_exit(NULL);
	return NULL;
}

void * global_hisgram_thread(void *p)
{
	thread_global_t global = *(thread_global_t *)p;
	int i;
	int zonenum = global.zonenum;
	for (i = 0; i < ZONENUM; i++)
		global.hisgram[zonenum] += (global.zones[i]).hisgram[zonenum];

	pthread_exit(NULL);
	return NULL;
}

void *sub_zonebegin_thread(void *p)
{
	thread_zonebegin_t zonesbegin = *(thread_zonebegin_t *)p;
	int i;
	int zonenum = zonesbegin.zonenum;
	(zonesbegin.zones[ZONENUM - 1]).zonebegin[zonenum] = zonesbegin.global_zone_end - (zonesbegin.zones[ZONENUM - 1]).hisgram[ZONENUM - 1];
	for (i = ZONENUM - 2; i >= 0; i--)
	{
		(zonesbegin.zones[i]).zonebegin[zonenum] = (zonesbegin.zones[i + 1]).zonebegin[zonenum] - (zonesbegin.zones[i]).hisgram[zonenum];
	}

	pthread_exit(NULL);
	return NULL;
}

void *zone_keydata_thread(void *p)
{
	thread_zonedata_t zonedata = *(thread_zonedata_t *)p;
	int *keyzonenum = (int*)malloc(sizeof(int) * zonedata.datalen);
	int i;
	int begin = zonedata.begin;

	for (i = 0; i < zonedata.datalen; i++)
		keyzonenum[i] = zonedata.keydata[i + begin] % ZONENUM;

	int *zonekey = zonedata.zonekeydata;
	int *zonebegin = zonedata.zonebegin;

	for (i = 0; i < zonedata.datalen; i++)
		zonekey[zonebegin[keyzonenum[i]]++] = zonedata.keydata[i + begin];

	free(keyzonenum);
	pthread_exit(NULL);
	return NULL;
}

void sub_hisgram(int *keydata, int keylen, int *subhisgram[ZONENUM], thread_subhisgram_t *zones, int *subzonebegin[ZONENUM])
{
	pthread_t threads[ZONENUM];
	pthread_attr_t attr;
	int rc;
	void *status;
	int i;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	int zonesize = keylen / ZONENUM;
	for (i = 0; i < ZONENUM; i++)
	{
		zones[i].hisgram = subhisgram[i];
		zones[i].zonebegin = subzonebegin[i];
		zones[i].datalen = zonesize;
		zones[i].keydata = keydata;
		zones[i].begin = i * zonesize;

		rc = pthread_create(&threads[i], &attr, sub_hisgram_thread, (void*)&zones[i]);
		assert(!rc);
	}

	for (i = 0; i < ZONENUM; i++)
	{
		rc = pthread_join(threads[i], &status);
		assert(!rc);
	}
}

void global_hisgram(int *keydata, int *hisgram, int *subhisgram[ZONENUM], thread_subhisgram_t *zones)
{
	pthread_t threads[ZONENUM];
	pthread_attr_t attr;
	thread_global_t global[ZONENUM];
	void *status;
	int rc, i;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	for (i = 0; i < ZONENUM; i++)
	{
		global[i].hisgram = hisgram;
		global[i].zones = zones;
		global[i].zonenum = i;
		rc = pthread_create(&threads[i], &attr, global_hisgram_thread, (void*)&global[i]);
		assert(!rc);
	}

	for (i = 0; i < ZONENUM; i++)
	{
		rc = pthread_join(threads[i], &status);
		assert(!rc);
	}
}

void sub_zonebegin(int *zone_end, int *subzonebegin[ZONENUM], int *subhisgram[ZONENUM], thread_subhisgram_t *zones)
{
	pthread_t threads[ZONENUM];
	pthread_attr_t attr;
	thread_zonebegin_t zonesbegin[ZONENUM];
	void *status;
	int rc, i;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	for (i = 0; i < ZONENUM; i++)
	{
		zonesbegin[i].global_zone_end = zone_end[i];
		zonesbegin[i].zones = zones;
		(zonesbegin[i].zones[i]).zonebegin = subzonebegin[i];
		zonesbegin[i].zonenum = i;

		rc = pthread_create(&threads[i], &attr, sub_zonebegin_thread, (void*)&zonesbegin[i]);
		assert(!rc);
	}

	for (i = 0; i < ZONENUM; i++)
	{
		rc = pthread_join(threads[i], &status);
		assert(!rc);
	}
}

void zone_keydata(int *keydata, int keylen, int *subzonebegin[ZONENUM], int *zonekey)
{
	pthread_t threads[ZONENUM];
	pthread_attr_t attr;
	thread_zonedata_t zonedata[ZONENUM];
	void *status;
	int rc, i;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);


	int zonesize = keylen / ZONENUM;
	for (i = 0; i < ZONENUM; i++)
	{
		zonedata[i].keydata = keydata;
		zonedata[i].zonekeydata = zonekey;
		zonedata[i].datalen = zonesize;
		zonedata[i].zonebegin = subzonebegin[i];
		zonedata[i].begin = i * zonesize;
		rc = pthread_create(&threads[i], &attr, zone_keydata_thread, (void*)&zonedata[i]);
		assert(!rc);
	}

	for (i = 0; i < ZONENUM; i++)
	{
		rc = pthread_join(threads[i], &status);
		assert(!rc);
	}
}

void parallel_partition(int *keydata, int keylen, int *hisgram, int *zone_end, int *zonekey)
{
	int i, j;
	thread_subhisgram_t zones[ZONENUM];

	int *subhisgram[ZONENUM];
	for (i = 0; i < ZONENUM; i++)
	{
		subhisgram[i] = (int*)malloc(sizeof(int) * ZONENUM);
		assert(subhisgram[i]);
		memset(subhisgram[i], 0, sizeof(int) * ZONENUM);
	}
	int *subzonebegin[ZONENUM];
	for (i = 0; i < ZONENUM; i++)
	{
		subzonebegin[i] = (int*)malloc(sizeof(int) * ZONENUM);
		assert(subzonebegin[i]);
		memset(subzonebegin[i], 0, sizeof(int) * ZONENUM);
	}
	/****************************************************************************************/
	sub_hisgram(keydata, keylen, subhisgram, zones, subzonebegin);

	//测试线程执行情况
	for (i = 0; i < ZONENUM; i++)
		for (j = 0; j < ZONENUM; j++)
			printf("zones[%d].hisgram[%d] = %d\n", i, j, zones[i].hisgram[j]);

	/****************************************************************************************/
	global_hisgram(keydata, hisgram, subhisgram, zones);

	//测试全局直方图
	for (i = 0; i < ZONENUM; i++)
		printf("hisgram[%d] = %d\n", i, hisgram[i]);

	/****************************************************************************************/
	//全局编址
	int sum = 0;
	for (i = 0; i < ZONENUM; i++)
	{
		sum += hisgram[i];
		zone_end[i] = sum;
	}
	for (i = 0; i < ZONENUM; i++)
		printf("zone_end[%d] = %d\n", i, zone_end[i]);



	/****************************************************************************************/
	sub_zonebegin(zone_end, subzonebegin, subhisgram, zones);
	for (i = 0; i < ZONENUM; i++)
		for (j = 0; j < ZONENUM; j++)
			printf("zones[%d].begin[%d] = %d\n", i, j, subzonebegin[i][j]);

	/****************************************************************************************/
	zone_keydata(keydata, keylen, subzonebegin, zonekey);

	for (i = 0; i < ZONENUM; i++)
	{
		free(subhisgram[i]);
		free(subzonebegin[i]);
	}

}