#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <stdio.h>
#include <assert.h>
#include "log.h" 

#define Log(fmt, ...) \
	_Log(fmt, ## __VA_ARGS__)


#define Assert(cond, ...) \
 do { \
   if (!(cond)) { \
     fflush(stdout); \
	 fprintf(stderr, __VA_ARGS__); \
	 assert(cond); \
   } \
 } while (0)

#endif
