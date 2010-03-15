
#include <malloc.h>

#include "threadpool.h"

DWORD WINAPI THREADPOOL_Loop(LPVOID arg);
DWORD WINAPI THREADPOOL_Read(LPVOID arg);

void THREADPOOL_Init(PTHREADPOOL ppool, UBYTE maxthreads, UINT32 maxtasks)
{
	UINT32 i;
	
	PTHREAD pthread;
	PTHREADPOOLCONTEXT pcontext;
	
	ppool->maxthreads = maxthreads;
	ppool->maxtasks = maxtasks;
	
	ppool->threads = calloc(maxthreads + 1, sizeof(THREAD));
	ppool->contexts = calloc(maxthreads + 1, sizeof(THREADPOOLCONTEXT));
	
	for (i = 0; i < maxthreads + 1; i++)
	{
		pthread = ppool->threads + i;
		
		if (i == 0)
			THREAD_Init(pthread, THREADPOOL_Read, 0);
		else
			THREAD_Init(pthread, THREADPOOL_Loop, i - 1);
		
		pcontext = ppool->contexts + i;
		pcontext->ppool = ppool;
		
		pthread->context = pcontext;
	}
	
	ppool->tasks = calloc(maxtasks, sizeof(TASK));
	ppool->active = calloc(maxthreads, sizeof(TASK));
	ppool->left = 0;
	ppool->right = 0;
	
	ppool->reader = ppool->threads;
}

void THREADPOOL_Start(const PTHREADPOOL ppool)
{
	UINT32 i;
	
	for (i = 0; i < ppool->maxthreads + 1; i++)
		THREAD_Start(ppool->threads + i);
}

void THREADPOOL_Stop(const PTHREADPOOL ppool)
{
}

void THREADPOOL_Join(const PTHREADPOOL ppool)
{
	THREAD_Join(ppool->reader);
}

void THREADPOOL_Restart(const PTHREADPOOL ppool);

void THREADPOOL_Free(const PTHREADPOOL ppool)
{
	UINT32 i;
	
	for (i = 0; i < ppool->maxthreads + 1; i++)
		THREAD_Free(ppool->threads + i);

	free(ppool->threads);	
	free(ppool->tasks);
	free(ppool->active);
}

// if there is a single work-production thread, no synchronization needed on the queue
BOOLEAN THREADPOOL_Queue(PTHREADPOOL ppool, WORK work, POINTER arg)
{
	PTASK ptask;
	
	UINT32 maxthreads;
	UINT32 maxtasks;
	
	UINT32 left;
	UINT32 right;
	UINT32 count;
	
	TASK * tasks = ppool->tasks;
	
	maxthreads = ppool->maxthreads;
	maxtasks = ppool->maxtasks;
	
	left = ppool->left;
	right = ppool->right;
	count = right - left;
	
	if (count == maxtasks)
		return FALSE;
	
	left = left % maxtasks;
	right = right % maxtasks;
	
	ptask = tasks + right;
	ptask->work = work;
	ptask->arg = arg;
	
	ppool->right++;
	
	THREAD_Signal(ppool->reader);
	
	return TRUE;
}

DWORD WINAPI THREADPOOL_Read(LPVOID arg)
{
	PTHREAD pthis = (PTHREAD)arg;
	PTHREADPOOLCONTEXT pcontext = (PTHREADPOOLCONTEXT)pthis->context;
	
	PTHREADPOOL ppool = pcontext->ppool;
	THREAD * threads = ppool->threads + 1;	// all threads except the first reader thread
	TASK * tasks = ppool->tasks;				// all waiting tasks in queue
	TASK * active = ppool->active;			// all active tasks
	
	PTHREAD pthread;	// pointer to worker thread
	PTASK ptask;		// pointer to task in wait queue
	PTASK pactive;		// pointer to task in active queue
	
	UINT32 i;
	BOOLEAN flag;
	
	UINT32 maxthreads = ppool->maxthreads;
	UINT32 maxtasks = ppool->maxtasks;
	
	UINT32 left;
	UINT32 right;
	UINT32 count;
	
	while (TRUE)
	{
		count = 0;
		
		// is there any tasks in a queue?
		while (count == 0)
		{
			left = ppool->left;
			right = ppool->right;
			count = right - left;
			
			if (count == 0)
				THREAD_Wait(pthis); // wait for item insertion
		}
		
		left = left % maxtasks;		// cyclic queue item counter
		right = right % maxtasks;	// cyclic queue item counter
		
		flag = TRUE;
		
		// search [and wait] for a free thread in the pool, acts as spin lock and event wait lock
		while (flag)
		{
			for (i = 0; i < maxthreads && flag; i++)
			{
				pthread = threads + i;
				if (pthread->state == 0)
					flag = FALSE; // free thread found
			}
			
			if (flag)
				THREAD_Wait(pthis);	// wait for free thread
		}
		
		ptask = tasks + left;
		
		// take a task to active array item corresponding to thread choosen and be happy
		pactive = active + pthread->uid;
		pactive->work = ptask->work;
		pactive->arg = ptask->arg;
		
		// mark task cell as free
		ptask->work = NULL;
		ptask->arg = NULL;	// potential memory leak, in future - resources should be released somewhere here
		
		// assign the task to the thread and mark thread as working
		pthread->context = pactive;
		pthread->state = 1;
		
		// signal to the thread to do the task
		THREAD_Signal(pthread);
		
		// go to next item
		ppool->left++;
	}
}

DWORD WINAPI THREADPOOL_Loop(LPVOID arg)
{
	PTHREAD pthis = (PTHREAD)arg;
	PTHREADPOOLCONTEXT pcontext = (PTHREADPOOLCONTEXT)pthis->context;
	
	PTHREADPOOL ppool = pcontext->ppool;
	
	PTASK ptask;
	
	while (TRUE)
	{
		// wait for a task
		THREAD_Wait(pthis);
		
		// get the task and do work
		ptask = (PTASK)pthis->context;
		ptask->work(pthis, ptask->arg);
		
		// mark thread as free
		pthis->context = NULL;
		pthis->state = 0;
		
		// signal reader thread about free thread
		THREAD_Signal(ppool->reader);
	}
}