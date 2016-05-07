#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include "global.h"
#include "loader.h"
#include "build.h"
#include "probe.h"

int main(int argc, char **argv)
{
	printf("begining...\n");
	clock_t t1, t2;

	int pkeylen = datalen[0];
	int *pkeydata = (int*)malloc(sizeof(int) * pkeylen);
	assert(pkeydata);

	int fkeylen = datalen[1];
	int *fkeydata = (int*)malloc(sizeof(int) * fkeylen);
	assert(pkeydata);

	loaddata(filenames, 0, pkeylen, pkeydata);
	loaddata(filenames, 0, fkeylen, fkeydata);

	int *hashtable = (int*)malloc(sizeof(int) * TABLESIZE);
	memset(hashtable, 0, sizeof(int) * TABLESIZE);
	assert(hashtable);

	printf("building hashtable...\n");
	t1 = clock();
	buildhash(pkeydata, hashtable, pkeylen);
	t2 = clock();
	printf("build hashtable time = %f\n", (double)(t2 - t1) / CLOCKS_PER_SEC);

	printf("probing...\n");
	t1 = clock();
	probe(hashtable, fkeydata, fkeylen);
	t2 = clock();
	printf("probing time = %f\n", (double)(t2 - t1) / CLOCKS_PER_SEC);

	free(hashtable);
	free(fkeydata);
	free(pkeydata);
}