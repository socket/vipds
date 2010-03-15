
#include <windows.h>

#include "../types/common.h"

#ifndef __CORE_THREADING_THREAD_INCLUDED
#define __CORE_THREADING_THREAD_INCLUDED

typedef struct THREAD_T THREAD, * PTHREAD;

struct THREAD_T 
{
	UINT32 uid;
	UINT32 tid;
	
	UBYTE state;
	
	POINTER context;
	
	HANDLE descriptor;
	HANDLE event;
};

void THREAD_Init(PTHREAD pthread, DWORD (WINAPI * function)(LPVOID), UINT32 uid);

void THREAD_Start(const PTHREAD pthread);

void THREAD_Signal(const PTHREAD pthread);
void THREAD_Wait(const PTHREAD pthread);
void THREAD_Join(const PTHREAD pthread);
void THREAD_Sleep(const UINT32 milliseconds);

void THREAD_Free(PTHREAD pthread);

#endif