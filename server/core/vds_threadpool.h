/*
 *  vds_threadpool.h
 *  server
 *
 *  Created by Alexey Streltsow on 3/10/10.
 *  Copyright 2010 Zila Networks LLC. All rights reserved.
 *
 */

#ifndef _vds_threadpool_h
#define _vds_threadpool_h

#include "vds_common.h"
#include "vds_thread.h"

typedef struct THREADPOOL_T THREADPOOL;
typedef struct TPQUEUE_T TPQUEUE;
typedef struct TPTASK_T TPTASK;

struct TPTASK_T {
  void (*process)(const TPTHREAD *, void *);
	void *arg;
  
  UBYTE state;
};

struct TPQUEUE_T {
  TPTASK* tasks;
  
  UINT size;
  UINT left;
  UINT right;
};

struct THREADPOOL_T {
  UINT leftIndex;
  UINT rightIndex;
  UINT maxThreads;
};


#endif