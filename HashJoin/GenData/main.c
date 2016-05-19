#include "genuniformdata.h"
#include "global.h"
#include <stdio.h>
#include <stdlib.h>
int main(void)
{
    int datalen = F_DATALEN[6];
    char *filename = F_FILENAMES[6];
    
    int *data = (int*)malloc(sizeof(int) * datalen);
    gen_fordata(data, datalen, filename);
    
    free(data);
    
    return 0;
}
