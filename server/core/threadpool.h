/*
 *  threadpool.h
 *  server
 *
 *  Created by Alexey Streltsow on 3/10/10.
 *  Copyright 2010 Zila Networks LLC. All rights reserved.
 *
 */

#include <stdio.h>
#include <windows.h>

#define N 10
#define Q 100000

typedef unsigned char UBYTE;
typedef unsigned short int UINT16;
typedef unsigned int UINT32;
typedef unsigned long long int UINT64;

struct TASK_T;
typedef struct TASK_T TASK;

struct QUEUE_T;
typedef struct QUEUE_T QUEUE;

struct THREAD_T;
typedef struct THREAD_T THREAD;

struct THREADPOOL_T;
typedef struct THREADPOOL_T THREADPOOL;

struct TASK_T
{
	void (*process)(const THREAD *, void *);
	void * arg;
	
	UBYTE state;
};

struct QUEUE_T
{
	TASK * tasks;
	
	UINT64 length;
	
	UINT64 left;
	UINT64 right;
};

struct THREAD_T
{
	HANDLE descriptor;
	HANDLE event;
	
	UBYTE number;
	UBYTE state;
};

struct THREADPOOL_T
{
	THREAD * threads;
	THREAD * controller;
	QUEUE * queue;
	
	UBYTE threadcount;
	UINT64 queuelength;
};

// thread operations
void THREAD_Init(THREAD *, DWORD (WINAPI *)(LPVOID), UBYTE);
void THREAD_Signal(const THREAD *);
void THREAD_Wait(const THREAD *);
void THREAD_Join(const THREAD *);
void THREAD_Close(THREAD *);

// queue operations
void QUEUE_Init(QUEUE *, UINT64);
TASK * QUEUE_Push(const QUEUE *, void (*)(const THREAD *, void *), void *);
TASK * QUEUE_Pop(const QUEUE *);

// threadpool operations
void THREADPOOL_Init(THREADPOOL*, UBYTE, UINT64);
TASK * THREADPOOL_Do(const THREADPOOL*, void (*)(const THREAD *, void *), void *);
void THREADPOOL_Wait(THREADPOOL *);

DWORD WINAPI THREADPOOL_Loop(LPVOID arg);

//=====================================================================================================================

void THREAD_Init(THREAD * thread, DWORD (WINAPI * function)(LPVOID), UBYTE number)
{
	thread->number = number;
	thread->state = 0;
	thread->event = CreateEvent(NULL, FALSE, FALSE, NULL);
	thread->descriptor = CreateThread(NULL, 0, function, thread, 0, NULL);
}

void THREAD_Signal(const THREAD * thread)
{
	SetEvent(thread->event);
}

void THREAD_Wait(const THREAD * thread)
{
	WaitForSingleObject(thread->event, INFINITE);
}

void THREAD_Join(const THREAD * thread)
{
	WaitForSingleObject(thread->descriptor, INFINITE);
}

void THREAD_Close(THREAD * thread)
{
	CloseHandle(thread->descriptor);
}

//=====================================================================================================================

void QUEUE_Init(QUEUE * queue, UINT64 length)
{
	queue->tasks = calloc(length, sizeof(TASK));
	queue->length = length;
	queue->left = 0;
	queue->right = 0;
}

// if this "interface" used from single thread only, no synchronization needed
TASK * QUEUE_Push(QUEUE * queue, void (*process)(const THREAD *, void *), void * arg)
{
	TASK * task;
	UINT64 right;
	
	right = (++(queue->right)) % queue->length;
	
	if (queue->tasks[right].state == 1) // queue is full, return NULL, no task created?
	{
		--(queue->right);
		return NULL;
	}
	
	task = queue->tasks + right;
	task->process = process;
	task->arg = arg;
	task->state = 1;
	
	return task;
}

// if this "interface" used from single thread only, no synchronization needed
TASK * QUEUE_Pop(QUEUE * queue)
{
	UINT64 left;
	
	left = (++(queue->left)) % queue->length;
	
	if (queue->tasks[left].state == 0) // queue is empty, return NULL
	{
		--(queue->left);
		return NULL;
	}
	
	return queue->tasks + left;
}

//=====================================================================================================================


void THREADPOOL_Init(THREADPOOL* pool, UBYTE threadcount, UINT64 queuelength)
{
	UBYTE i;
	
	pool->threadcount = threadcount;
	pool->queuelength = queuelength;
	
	pool->queue = calloc(1, sizeof(QUEUE));
	QUEUE_Init(pool->queue, queuelength);
	
	pool->controller = calloc(1, sizeof(THREAD));
	THREAD_INIT(pool->controller, THREADPOOL_Control);
	
	pool->threads = calloc(threadcount, sizeof(THREAD));
	for (i = 0; i < threadcount; i++)
		THREAD_Init(pool->threads + i, THREADPOOL_Loop, i);
}

TASK * THREADPOOL_Do(THREADPOOL* pool, void (*process)(const THREAD *, void *), void * arg)
{
}

void THREADPOOL_Wait(THREADPOOL * pool)
{
	UBYTE i;
	
	for (i = 0; i < pool->threadcount; i++)
	{
		THREAD_Join(pool->threads + i);
		THREAD_Close(pool->threads + i);
	}
}

DWORD WINAPI THREADPOOL_Control(LPVOID arg)
{
	THREAD * controller = (THREAD *)arg;
	
	while (TRUE)
	{
		// pop task if exists
	}
}

DWORD WINAPI THREADPOOL_Loop(LPVOID arg)
{
	THREAD * thread = (THREAD *)arg;
	
	while (TRUE)
	{
		THREAD_Wait(thread); // wait for task assigned
		
		// do task
	}
}
//
//int main(int argc, char* argv[])
//{
//	THREADPOOL pool;
//	
//	THREADPOOL_Init(&pool, N, Q); // initialize threadpool
//	
//	// push tasks in loop
//	
//	THREADPOOL_Wait(&pool); // wait dummy
//	
//  
//	return 0;
//}