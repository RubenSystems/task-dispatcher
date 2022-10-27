//
//  dispatcher.h
//
//  Created by Ruben Ticehurst-James on 22/10/2022.
//

#ifndef dispatcher_h
#define dispatcher_h

#include <stdio.h>
#include <pthread.h>


#define DISPATCHER_TASK_SIZE 10

typedef void * (*dispatcher_task)(void *);


struct DispatchQueue init_dispatch_queue(void);

void async_task(struct DispatchQueue *, dispatcher_task, void *);

void start_execution(struct DispatchQueue *);

void join(struct DispatchQueue *);

struct QueueNode {
	dispatcher_task task;
	void * arg;
	struct QueueNode * next;
	
};

struct DispatchQueue {
	struct QueueNode * begin;
	struct QueueNode * end;
	char running; // shutdown the queue
	pthread_mutex_t mutex;
	pthread_cond_t add_contition; // condition for waiting on add
	pthread_t threads [DISPATCHER_TASK_SIZE];
};


/*
 Should be called only by dispatch queue.
 */
enum DequeueTaskStatus {
	DEQUEUE_NOITEM,
	DEQUEUE_EXECUTED
};




#endif /* dispatcher_h */
