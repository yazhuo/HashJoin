#ifndef PARTITION_H_
#define PARTITION_H_

//hash·ÖÇøËã·¨
void Hash_Partition(int *ary, int size);
void Init(int *keyzonenum, int size, int *ary);
void Sub_Hisgram(int *keyzonenum, int *sub_hisgram, int size);
void Glo_Hisgram(int *glo_hisgram, int *sub_hisgram);
void Glo_Addr(int *glo_hisgram);
void Sub_Addr(int *glo_hisgram, int *sub_hisgram, int *zone_begin);
void Zone_Key(int *ary, int size, int *zone_begin, int *keyzonenum);
#endif
