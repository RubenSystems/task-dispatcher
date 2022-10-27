# Task Dispatcher
Asynchronous execution of tasks and thread reuse for minimal runtime overhead. 


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

##### Clear queue
This should only be done when program execution is complete. This can cause nasty issues if it is not called before the program halts. 

```c
join(&queue);
```
