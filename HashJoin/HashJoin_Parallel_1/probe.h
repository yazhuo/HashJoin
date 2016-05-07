#ifndef PROBE_H_
#define PROBE_H_
#include "global.h"
#include "build.h"

void * probe_process_thread(void *p);
int probe_process(int hb_param, int fk_param, int *fkeydata, int *hashtable, int *status_hashtable);
void probe_with_zone(int *hashtable[ZONENUM], int *fkeydata, int *hisgram);

#endif