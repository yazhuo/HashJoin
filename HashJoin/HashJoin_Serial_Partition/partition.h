#ifndef PARTITION_H_
#define PARTITION_H_

//hash·ÖÇøËã·¨
void hash_partition(int *ary, int size, int *keyzonenum, int *hisgram);
void init(int *keyzonenum, int size, int *ary);
void scan1(int *ary, int *hisgram, int size);
void begin_addr(int *hisgram);
void scan2(int *keyzonenum, int *hisgram, int size, int *ary);

#endif
