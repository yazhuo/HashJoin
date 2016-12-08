#ifndef PARTITION_H_
#define PARTITION_H_

//hash·ÖÇøËã·¨
void Hash_Partition(int *ary, int size);
void init(int *keyzonenum, int size, int *ary);
void scan1(int *ary, int *hisgram, int size);
void begin_addr(int *hisgram);
void scan2(int *keyzonenum, int *hisgram, int size, int *ary);

#endif
