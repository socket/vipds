#include <stdio.h>
#include <windows.h>

#include "core/types/common.h"
#include "core/threading/thread.h"
#include "core/threading/threadpool.h"


#define N 4
#define Q 64

DWORD WINAPI f(LPVOID a)
{
	PTHREAD pthread = (PTHREAD)a;
	
	printf("From thread[%d/%d]\n", pthread->uid, pthread->tid);
	printf("press ENTER\n");
	
	getchar();
}

THREADPOOL pool;

UINT64 i = 0;
UINT64 l = 0;
UINT32 m = 0;
UINT32 n = 0;
UINT64 d = 0;

void W(PTHREAD pthread, POINTER arg)
{
	UINT64 j = 0;
	UINT64 k = InterlockedIncrement(&d);
	
//	printf("my uid is %d, %d - started\n", pthread->uid, k);
	
	for (j = 0; j < 1000000; j++)
	{
		InterlockedIncrement(&l);
	}
	
	printf("my uid is %d, %d - finished\n", pthread->uid, k);
}

int main(int argc, char* argv[])
{
	THREADPOOL_Init(&pool, N, Q);
	THREADPOOL_Start(&pool);
	
	printf("PRESS ENTER TO RUN\n");
	getchar();
	
	m = 0;
	i = 0;
	
	while (TRUE)
	{
//		printf("press ENTER to queue\n"); getchar();
		
		if (!THREADPOOL_Queue(&pool, W, NULL))
			break;
		i++;
	}
	printf("i = %d\n", i);
	getchar();

	return 0;
}

