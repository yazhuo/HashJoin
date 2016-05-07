#ifndef PROBE_H_
#define PROBE_H_
#include "global.h"
#include "build.h"

void probe(int *hashtable, int *fkeydata, int fkeylen);
int probe_process(int fk_param, int *fkeydata, int *hashtable, int *status_hashtable);

#endif
