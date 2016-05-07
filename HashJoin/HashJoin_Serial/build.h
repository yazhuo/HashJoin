#ifndef BUILD_H_
#define BUILD_H_
#include "global.h"

int hash1(int key);
int hash2(int key);
int doublehash(int key, int offset);
void buildhash(int *pkeydata, int *hashtable, int pkeylen);
void build_process(int pk_param, int *pkeydata, int *hashtable, int *status_hashtable);

#endif