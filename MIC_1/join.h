#ifndef JOIN_H_
#define JOIN_H_
#include "global.h"

#pragma offload_attribute(push, target(mic))
int hash1(int key);
int hash2(int key);
int HashFunc(int key, int offset);
void HashTable_Insert(int hb_param, int pk_param, int *pkeydata, int *hashtable, int *status_hashtable);
void HashTable_Create(int *pkeydata, int *hashtable, int pkeylen);
int HashTable_Find(int hb_param, int probekey, int *hashtable, int *status_hashtable);
void Hash_Probe(int *fkeydata, int *hashtable, int fkeylen);
void Hash_Join(int *pkeydata, int *fkeydata, int *hashtable, int pkeylen, int fkeylen, long *b_time, long *p_time);
#pragma offload_attribute(pop)

#endif