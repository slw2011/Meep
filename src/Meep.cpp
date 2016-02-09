//
//  Meep.cpp
//  Meep
//
//  Created by leviathan on 16/1/31.
//  Copyright © 2016年 leviathan. All rights reserved.
//

#include <iostream>
#include <vector>
#include "Meep.hpp"

using namespace Meep;
using std::vector;
using std::endl;
using std::cout;

//Task class static variable
int Meep::Task::sign=0;

//Meep class static variable
vector<Meep::Task *> Thread_pool::task_list;
vector<pthread_t > Thread_pool::busy;
vector<pthread_t > Thread_pool::idle;

bool Thread_pool::shutdown=false;
pthread_mutex_t Thread_pool::pthread_mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t Thread_pool::list_mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t Thread_pool::output_mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t Thread_pool::pthread_cond=PTHREAD_COND_INITIALIZER;

//task class

Task::Task()
{
    sign=0;
    task_data="";
}

Task::~Task()
{
    cout<<"Task complete!"<<endl;
}

void Task::run(pthread_t pid)
{
    pthread_mutex_lock(&Thread_pool::output_mutex);
    cout<<pid<<":"<<task_data<<" "<<sign++<<endl;
    pthread_mutex_unlock(&Thread_pool::output_mutex);
}

//thread pool class
Thread_pool::Thread_pool(int num)
{
    thread_num=num;
    create_pool();
}

Thread_pool::~Thread_pool()
{
    cout<<"The thread pool is empty!"<<endl;
}

void Thread_pool::move2busy(const pthread_t move_pid)
{
    vector<pthread_t >::iterator index;
    for (index=idle.begin();index!=idle.end()&&idle.size()!=0;index++)
    {
        if (*index==move_pid)
            break;
    }
    idle.erase(index);
    busy.push_back(move_pid);
}

void Thread_pool::move2idle(const pthread_t move_pid)
{
    vector<pthread_t >::iterator index;
    for (index=busy.begin();index!=busy.end()&&busy.size()!=0;index++)
    {
        if (*index==move_pid)
            break;
    }
    busy.erase(index);
    idle.push_back(move_pid);
}

void * Thread_pool::thread_call(void * data)
{
    pthread_t self_tid=pthread_self();
    while (1)
    {
        pthread_mutex_lock(&pthread_mutex);
        while (task_list.size()==0&&!shutdown)
            pthread_cond_wait(&pthread_cond, &pthread_mutex);
        if(shutdown)
        {
            pthread_mutex_unlock(&pthread_mutex);
            pthread_mutex_lock(&output_mutex);
            cout<<"The thread "<<self_tid<<" will exit"<<endl;
            pthread_mutex_unlock(&output_mutex);
            pthread_exit(NULL);
        }
        auto pointer=task_list.begin();
        Task *task=*pointer;
        if (pointer!=task_list.end()) {
            task=*pointer;
            task_list.erase(pointer);
        }
        pthread_mutex_unlock(&pthread_mutex);
        //run task
        pthread_mutex_lock(&list_mutex);
        move2busy(self_tid);
        pthread_mutex_unlock(&list_mutex);
        task->run(self_tid);
        pthread_mutex_lock(&list_mutex);
        move2idle(self_tid);
        pthread_mutex_unlock(&list_mutex);
        
    }
    
}


void Thread_pool::create_pool()
{
    pthread_id=new pthread_t[thread_num];
    for (int i=0; i<thread_num; ++i) {
        pthread_create(&pthread_id[i], NULL,thread_call,NULL);
        idle.push_back(pthread_id[i]);
    }
    cout<<"Already create "<<thread_num<<" threads"<<endl;
    return ;
}


size_t Thread_pool::get_task_num()
{
    return task_list.size();
}

void Thread_pool::add_task(Task *task)
{
    pthread_mutex_lock(&pthread_mutex);
    task_list.push_back(task);
    pthread_mutex_unlock(&pthread_mutex);
    pthread_cond_signal(&pthread_cond);
    
}

int Thread_pool::stop()
{
    if(shutdown)
        return -1; //thread already exit
    shutdown=true;
    pthread_cond_broadcast(&pthread_cond);
    for(int i=0;i<thread_num;++i)
        pthread_join(pthread_id[i],NULL);
    delete pthread_id;
    pthread_mutex_destroy(&pthread_mutex);
    pthread_cond_destroy(&pthread_cond);
    return 0;
}

