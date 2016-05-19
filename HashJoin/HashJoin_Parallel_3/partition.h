#ifndef PARTITION_H_
#define PARTITION_H_

#include "global.h"

void * sub_hisgram_thread(void *p);
void * global_hisgram_thread(void *p);
void *sub_zonebegin_thread(void *p);
void *zone_keydata_thread(void *p);
void sub_hisgram(int *keydata, int keylen, int *subhisgram[ZONENUM], thread_subhisgram_t *zones, int *subzonebegin[ZONENUM]);
void global_hisgram(int *keydata, int *hisgram, int *subhisgram[ZONENUM], thread_subhisgram_t *zones);
void sub_zonebegin(int *zone_end, int *subzonebegin[ZONENUM], int *subhisgram[ZONENUM], thread_subhisgram_t *zones);
void zone_keydata(int *keydata, int keylen, int *subzonebegin[ZONENUM], int *zonekey);
void parallel_partition(int *keydata, int keylen, int *hisgram, int *zone_begin, int *zonekey);

#endif
