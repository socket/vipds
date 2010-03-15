
#include "../types/common.h"

#include "thread.h"

#ifndef __CORE_THREADING_THREADPOOL_INCLUDED
#define __CORE_THREADING_THREADPOOL_INCLUDED

typedef void (*WORK)(PTHREAD pthread, POINTER arg);

typedef struct TASK_T TASK, * PTASK;

typedef struct THREADPOOL_T THREADPOOL, * PTHREADPOOL;
typedef struct THREADPOOLCONTEXT_T THREADPOOLCONTEXT, * PTHREADPOOLCONTEXT;

struct TASK_T
{
	WORK work;
	POINTER arg;
};

struct THREADPOOL_T 
{
	UBYTE maxthreads;
	
	PTHREAD reader;		// reader thread
	
	THREAD * threads;					// first reader thread + some worker threads, the size is maxthreads + 1
	THREADPOOLCONTEXT * contexts;	// first reader thread context + some worker thread contexts, the size is maxthreads + 1
	
	UINT32 maxtasks;
	
	TASK * tasks;		// wait task queue - buffer for incoming tasks, cyclic queue, the size is maxtasks
	TASK * active;		// active task queue - buffer for currently working tasks, simple array, the size is maxthreads
	
	UINT32 left;		// left bound (first item) of the cyclic queue, incremented on activation
	UINT32 right;		// right bound (item after the last item of the queue) of the cyclic queue, incremented on insertion
};

struct THREADPOOLCONTEXT_T
{
	PTHREADPOOL ppool;
};

void THREADPOOL_Init(PTHREADPOOL ppool, UBYTE maxthreads, UINT32 maxtasks);

void THREADPOOL_Start(const PTHREADPOOL ppool);
void THREADPOOL_Stop(const PTHREADPOOL ppool);		// not implemented
void THREADPOOL_Join(const PTHREADPOOL ppool);
void THREADPOOL_Restart(const PTHREADPOOL ppool);		// not implemented

void THREADPOOL_Free(PTHREADPOOL ppool);				// for correct result THREAD_Stop should be used before

BOOLEAN THREADPOOL_Queue(PTHREADPOOL ppool, WORK work, POINTER arg);

#endif