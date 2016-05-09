#ifndef GLOBAL_H_
#define GLOBAL_H_

#define NAMEMAX 256 
extern char filenames[][NAMEMAX];
extern int datalen[];

//#define TABLESIZE 87719
//#define ZONENUM 256
#define TABLESIZE 11
#define ZONENUM 4

typedef struct
{
	int zone_id;
	int begin;
	int end;
	int *keydata;
	int *hashtable;
	int *status_hashtable;
}thread_param_t;

#endif