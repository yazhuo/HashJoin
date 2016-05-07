#ifndef BUILD_H_
#define BUILD_H_
#include "global.h"


int hash1(int key);
int hash2(int key);
int doublehash(int key, int offset);
void build_process(int hb_param, int pk_param, int *pkeydata, int *hashtable[ZONENUM], int *status_hashtable[ZONENUM]);
void buildhash_with_zone(int *pkeydata, int *hashtable[ZONENUM], int *hisgram);

#endif