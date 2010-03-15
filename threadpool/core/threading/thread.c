
#include <malloc.h>
#include <windows.h>

#include "thread.h"

void THREAD_Init(PTHREAD pthread, DWORD (WINAPI * function)(LPVOID), UINT32 uid)
{
	pthread->uid = uid;
	pthread->tid = 0;
	
	pthread->state = 0;
	pthread->context = NULL;
	
	pthread->event = CreateEvent(NULL, FALSE, FALSE, NULL);
	pthread->descriptor = CreateThread(NULL, 0, function, pthread, CREATE_SUSPENDED, &(pthread->tid));
}

void THREAD_Start(const PTHREAD pthread)
{
	ResumeThread(pthread->descriptor);
}

void THREAD_Signal(const PTHREAD pthread)
{
	SetEvent(pthread->event);
}

void THREAD_Wait(const PTHREAD pthread)
{
	WaitForSingleObject(pthread->event, INFINITE);
}

void THREAD_Join(const PTHREAD pthread)
{
	WaitForSingleObject(pthread->descriptor, INFINITE);
}

void THREAD_Sleep(const UINT32 milliseconds)
{
	Sleep(milliseconds);
}

void THREAD_Free(PTHREAD pthread)
{
	CloseHandle(pthread->descriptor);
}