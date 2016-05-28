#ifndef GLO_MALLOC_H_
#define GLO_MALLOC_H_

void basic_malloc(int *pkeydata, int *fkeydata, int *hashtable, int pkeylen, int fkeylen);
void basic_free(int *pkeydata, int *fkeydata, int *hashtable);
#endif