#include "global.h"

char F_FILENAMES[][256] = {
    "DataUniform16M",
    "DataUniform32M",
    "DataUniform64M",
    "DataUniform128M",
    "DataUniform256M",
    "DataUniform512M",
    "DataUniform1024M"
};

int F_DATALEN[] ={ 1<<24, 1<<25, 1<<26, 1<<27,1<<28,1<<29,1<<30 };
