#ifndef PROBE_H_
#define PROBE_H_
#include "global.h"
#include "build.h"

int probe_process(int hb_param, int fk_param, int *fkeydata, int *hashtable[ZONENUM], int *status_hashtable[ZONENUM]);
void probe_with_zone(int *hashtable[ZONENUM], int *fkeydata, int *hisgram);

#endif
