#include "glo_malloc.h"
#include "global.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

void basic_malloc(int *pkeydata, int *fkeydata, int *hashtable, int pkeylen, int fkeylen)
{
	pkeydata = (int*)malloc(sizeof(int) * pkeylen);
	assert(pkeydata);
	
	fkeydata = (int*)malloc(sizeof(int) * fkeylen);
	assert(fkeydata);

	hashtable = (int*)malloc(sizeof(int) * TABLESIZE);
	assert(hashtable);
	memset(hashtable, 0, sizeof(int) * TABLESIZE);
}

void basic_free(int *pkeydata, int *fkeydata, int *hashtable)
{
	free(pkeydata);
	free(fkeydata);
	free(hashtable);
}