#include "global.h"

char filenames[][NAMEMAX] = {
	"DataKey16M",
	"DataUniform16M","DataUniform32M","DataUniform64M","DataUniform128M"
};

int datalen[] = {
	1 << 24,
	1 << 24, 1 << 25, 1 << 26, 1 << 27
};