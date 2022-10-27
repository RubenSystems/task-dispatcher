# Task Dispatcher
Asynchronous execution of tasks and thread reuse for minimal runtime overhead in the creation of threads.

This is useful for:
- Generic thread management: Like the pthread API, this accepts any ```void *``` parameter function. The library manages the creation and joining of threads. 
- Web servers: This can be used to asynchronously manage multiple clients.

or anywhere else a thread pool is used. 


# Instructions 

##### Create a queue 
```c
struct DispatchQueue queue = init_dispatch_queue();
start_execution(&queue);
```

##### Add tasks to the queue
```c
int x [] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
for (int i = 0; i < sizeof(x) / sizeof(int); i ++) {
	async_task(&queue, say_hello, &x[i]);
}
```

##### Clear ("join") queue
Joining should be done when program execution is complete. This prevents nasty issues when the program halts. If the queue is operating forever, such as on a webserver, joining is not required. 

```c
join(&queue);
```
