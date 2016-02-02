#Meep

**Meep**,A tiny C++ Thread Pool implementation

##原理：

![Smaller icon](https://github.com/Leviathan1995/Meep/raw/master/image/thread_pool.png)


##Basic usage:
	
	//create thread pool with 2 idle threads
	Meep t_pool(2);
	
	//add task to task list
	Task task;
	t_pool.add_task(&task);
	
	//then the thread pool is running
	
##License:
MIT
