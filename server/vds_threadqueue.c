/*
 *  vds_threadqueue.c
 *  server
 *
 *  Created by Alexey Streltsow on 3/10/10.
 *  Copyright 2010 Zila Networks LLC. All rights reserved.
 *
 */
#include "vds_threadpool.h"
#include "vds_threadqueue.h"

void 
TPQUEUE_Init(TPQUEUE * queue, UINT size) {
	queue->tasks = calloc(size, sizeof(TPTASK));
	queue->size = size;
	queue->left = 0;
	queue->right = 0;
}

TPTASK* 
TPQUEUE_Push(TPQUEUE * queue, void (*process)(const TPTHREAD *, void *), void * arg) {
	TPTASK *task;
	UINT right;
	
	right = (++(queue->right)) % queue->size;
	if (queue->tasks[right].state == 1) {
		--(queue->right);
    
    // FIXME
		return NULL;
	}
	
	task = queue->tasks + right;
	task->process = process;
	task->arg = arg;
	task->state = 1;
	
	return task;
}

TPTASK* 
TPQUEUE_Pop(TPQUEUE * queue) {
	UINT left;
	left = (++(queue->left)) % queue->size;
	
	if (queue->tasks[left].state == 0)
	{
		--(queue->left);
		return NULL;
	}
	
	return queue->tasks + left;
}