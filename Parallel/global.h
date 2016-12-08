#ifndef GLOBAL_H_
#define GLOBAL_H_

#define NAMEMAX 256
extern char FILENAME[][NAMEMAX];
extern int DATALEN[];

#define TABLESIZE 22458671
/*
hashtable size :
17,            37,          79,        163,          331,
673,           1361,        2729,       5471,         10949,
21911,          43853,      87719,      175447,      350899,
701819,         1403641,    2807303,     5614657,     11229331,
22458671,       44917381,    89834777,    179669557,   359339171,
718678369,      1437356741,  2147483647
*/

#define THREADNUM 242424242424242424242424242424242424242424242424 

#endif
