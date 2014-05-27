// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#define MAX_PATH            260

#define MAKE_LE(shY)  (((shY >> 8) & 0xFF) | ((shY << 8) & 0xFF00 ))
#define MAKE_DWORD_LE(dwY)  (((dwY >> 24) & 0x000000FF) | ((dwY >> 8) & 0x0000FF00) | ((dwY << 24) & 0xFF000000) | ((dwY << 8) & 0x00FF0000))
