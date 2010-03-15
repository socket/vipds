
#ifndef __CORE_TYPES_COMMON_INCLUDED
#define __CORE_TYPES_COMMON_INCLUDED

typedef char BOOLEAN;

typedef char BYTE;
typedef short int INT16;
typedef int INT32;
typedef long long int INT64;

typedef unsigned char UBYTE;
typedef unsigned short int UINT16;
typedef unsigned int UINT32;
typedef unsigned long long int UINT64;

#ifndef NULL
#define NULL 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

typedef void * POINTER;

#endif