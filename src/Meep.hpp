//
//  Meep.hpp
//  Meep
//
//  Created by leviathan on 16/1/31.
//  Copyright © 2016年 leviathan. All rights reserved.
//

#ifndef MEEP_HPP_
#define MEEP_HPP_

#include <vector>
#include <string>
#include <thread>
#include <queue>
#include <functional>


//Task class
class Task
{
public:
    Task(){};
    Task(std::string name):
        task_name(name),task_data(nullptr){}
    void set_data(void *data);
private:
    std::string task_name;
    void * task_data;
};

//Thead pool class
class Meep
{
public:
    Meep(int num); //init thread_num
    void create_pool();
    void *thread_call(void * data);
    int move2pool(pthread_t pid);
    int move2busy(pthread_t pid);
    int addtask(Task * task);
    int stop();
    int get_task_num();
private:
    std::vector<Task> task_list;
    bool shutdown;
    int thread_num;
    pthread_t * pthread_id;
    pthread_mutex_t pthread_mutex;
    pthread_cond_t pthread_cond;
    
};

#endif /* MEEP_HPP_ */
