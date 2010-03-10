/*
 *  vds_threadqueue.h
 *  server
 *
 *  Created by Alexey Streltsow on 3/10/10.
 *  Copyright 2010 Zila Networks LLC. All rights reserved.
 *
 */

#ifndef _vds_threadqueue_h
#define _vds_threadqueue_h

#include "vds_threadpool.h"

void      TPQUEUE_Init(TPQUEUE * queue, UINT size);
TPTASK*   TPQUEUE_Push(TPQUEUE * queue, void (*process)(const TPTHREAD *, void *), void * arg);
TPTASK*   TPQUEUE_Pop(TPQUEUE * queue);

#endif