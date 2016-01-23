#ifndef COLOR_H
#define COLOR_H

#include <stdint.h>

#define CH_RED		0
#define CH_GREEN	1
#define CH_BLUE		2

// Absolute level for different colour channels.
const uint16_t abs_level[3][5] = 
{
	{ 0,	2500,	2800,	3400,	4000 },
	{ 0,	2500,	2800,	3400,	4000 },
	{ 0,	2500,	2800,	3400,	4000 }
};

// Turn the channel off, abbreviation.
#define OFF			0

#endif
