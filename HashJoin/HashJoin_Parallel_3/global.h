#ifndef GLOBAL_H_
#define GLOBAL_H_

#define NAMEMAX 256 
extern char filenames[][NAMEMAX];
extern int datalen[];

#define TABLESIZE 87719
#define ZONENUM 256
//#define TABLESIZE 11
//#define ZONENUM 2

typedef struct
{
	int zone_id;
	int begin;
	int end;
	int *keydata;
	int *hashtable;
	int *status_hashtable;
}thread_param_t;

typedef struct
{
	int *hisgram;
	int *keydata;
	int *zonebegin;
	int datalen;
	int begin;
}thread_subhisgram_t;

typedef struct
{
	thread_subhisgram_t *zones;
	int *hisgram;
	int zonenum;
}thread_global_t;

typedef struct
{
	int global_zone_end;
	int zonenum;
	thread_subhisgram_t *zones;
}thread_zonebegin_t;

typedef struct
{
	int *zonebegin;
	int *keydata;
	int *zonekeydata;
	int datalen;
	int begin;
}thread_zonedata_t;

#endif