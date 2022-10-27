//
//  dispatcher.c
//
//  Created by Ruben Ticehurst-James on 22/10/2022.
//

#include "dispatcher.h"
#include <stdlib.h>



struct DispatchQueue init_dispatch_queue() {
	struct DispatchQueue queue = {
		.begin = NULL,
		.end = NULL,
		.mutex = PTHREAD_MUTEX_INITIALIZER,
		.add_contition = PTHREAD_COND_INITIALIZER,
		.running = 1
	};
	
	return queue;
}

static struct QueueNode * _init_queue_node(dispatcher_task task, void * arg) {
	struct QueueNode * node = malloc(sizeof(struct QueueNode));
	node->task = task;
	node->arg = arg;
	node->next = NULL;
	return node;
}

// Non-thread safe helper functions;
static void _enqueue(struct DispatchQueue * queue, dispatcher_task task, void * arg) {
	struct QueueNode * node = _init_queue_node(task, arg);
	if (queue->begin == NULL) {
		queue->begin = node;
		queue->end = queue->begin;
	} else {
		queue->end->next = node;
		queue->end = queue->end->next;
	}
}

static struct QueueNode * _dequeue(struct DispatchQueue * queue) {
	if (queue->begin != NULL) {
		struct QueueNode * begin = queue->begin;
		queue->begin = queue->begin->next;
		return begin;
	}
	queue->end = NULL;
	return NULL;
}

static enum DequeueTaskStatus _dequeue_task(struct DispatchQueue * queue) {
	struct QueueNode * back = NULL;
	pthread_mutex_lock(&queue->mutex);
	if ((back = _dequeue(queue)) == NULL && queue->running == 1) {
		pthread_cond_wait(&queue->add_contition, &queue->mutex);
		back = _dequeue(queue);
	} else if (back == NULL && queue->running == 0) {
		pthread_mutex_unlock(&queue->mutex);
		return DEQUEUE_NOITEM;
	}
	pthread_mutex_unlock(&queue->mutex);
	if (back != NULL) {
		back->task(back->arg);
		free(back);
		return DEQUEUE_EXECUTED;
	} else {
		return DEQUEUE_NOITEM;
	}
}

int counter = 0;
static void * _thread_exe_function(void * data){
	struct DispatchQueue * queue = (struct DispatchQueue *)data;
	while (queue->running) {
		_dequeue_task(queue);
	}
	
	while (_dequeue_task(queue) == DEQUEUE_EXECUTED) {
		pthread_cond_signal(&queue->add_contition);
	}
		
	return NULL;
}

void async_task(struct DispatchQueue * queue, dispatcher_task task, void * arg) {
	pthread_mutex_lock(&queue->mutex);
	_enqueue(queue, task, arg);
	pthread_mutex_unlock(&queue->mutex);
}

void start_execution(struct DispatchQueue * queue) {
	
	for (int i = 0; i < DISPATCHER_TASK_SIZE; i ++) {
		pthread_create(&queue->threads[i], NULL, _thread_exe_function, queue);
	}
}

void join(struct DispatchQueue * queue) {
	queue->running = 0;
	for (int i = 0; i < DISPATCHER_TASK_SIZE; i ++) {
		pthread_join(queue->threads[i], NULL);
	}
}
