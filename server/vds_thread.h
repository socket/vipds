/*
 *  vds_thread.h
 *  server
 *
 *  Created by Alexey Streltsow on 3/10/10.
 *  Copyright 2010 Zila Networks LLC. All rights reserved.
 *
 */

#ifndef _vds_thread_h
#define _vds_thread_h

typedef struct TPTHREAD_T TPTHREAD;

struct TPTHREAD_T {
	pthread_t       handle;
	pthread_mutex_t eventMutex;
  pthread_cond_t  event;
  
	UBYTE number;
	UBYTE state;
};

#endif //_vds_thread_h