/*
 *  vds_thread.c
 *  server
 *
 *  Created by Alexey Streltsow on 3/10/10.
 *  Copyright 2010 Zila Networks LLC. All rights reserved.
 *
 */

#include "vds_common.h"
#include "vds_thread.h"

// some comment to test git

void 
TPTHREAD_Init(TPTHREAD * thread, void* (*function)(void *), UBYTE number) {
  thread->number = number;
  thread->state = 0;

  pthread_attr_t stack_size;
  pthread_attr_init(&stack_size);
  pthread_attr_setstacksize(&stack_size, 1024*1024);
  pthread_create(&thread->handle, &stack_size, function, NULL);
  pthread_attr_destroy(&stack_size);
  
  // FIXME: add condition variable
}

void 
TPTHREAD_Signal(const TPTHREAD * thread) {
	//SetEvent(thread->event);
}

void 
TPTHREAD_Wait(const TPTHREAD * thread) {
	//WaitForSingleObject(thread->event, INFINITE);
}

void 
TPTHREAD_Join(const TPTHREAD * thread) {
  void* exitcode;
	pthread_join(thread->handle, &exitcode);
}

void 
TPTHREAD_Close(TPTHREAD * thread) {
  
	//CloseHandle(thread->descriptor);
}
