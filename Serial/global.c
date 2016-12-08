#include "global.h"

char FILENAME[][NAMEMAX] = {
	"DataKey16M",
	"DataUniform16M","DataUniform32M","DataUniform64M","DataUniform128M","DataUniform256M","DataUniform512M","DataUniform1024M"
};

int DATALEN[] = {
	1 << 24,
	1 << 24, 1 << 25, 1 << 26, 1 << 27, 1 << 28, 1 << 29, 1 << 30
};
