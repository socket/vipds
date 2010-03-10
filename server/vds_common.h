/*
 *  vds_common.h
 *  server
 *
 *  Created by Alexey Streltsow on 3/10/10.
 *  Copyright 2010 Zila Networks LLC. All rights reserved.
 *
 */

#ifndef _vds_common_h
#define _vds_common_h
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef unsigned char UBYTE;
typedef unsigned short int UINT16;
typedef unsigned int UINT32;
typedef unsigned long long int UINT64;

typedef UINT32 UINT;

void die(const char* error, ...);
void die_trace(const char* filename, int line, const char* error, ...);

#define DIE(text) ( die_trace(__FILE__, __LINE__, text) )


#endif