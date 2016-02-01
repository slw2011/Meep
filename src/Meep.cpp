//
//  Meep.cpp
//  Meep
//
//  Created by leviathan on 16/1/31.
//  Copyright © 2016年 leviathan. All rights reserved.
//



#include <iostream>
#include "Meep.hpp"
using namespace std;

//Task class static variable
int Task::sign=0;

//Meep class static variable
vector<Task *> Meep::task_list;
bool Meep::shutdown=false;
pthread_mutex_t Meep::pthread_mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t Meep::output_mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t Meep::pthread_cond=PTHREAD_COND_INITIALIZER;

//task class
void Task::set_data(void *data)
{
    task_data=data;
}

void Task::run()
{
    pthread_mutex_lock(&Meep::output_mutex);
    cout<<(char *)task_data<<endl;
    cout<<sign++<<endl;
    pthread_mutex_unlock(&Meep::output_mutex);
}

//thread pool class
Meep::Meep(int num)
{
    thread_num=num;
    create_pool();
}


void * Meep::thread_call(void * data)
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
        pthread_mutex_lock(&output_mutex);
        cout<<"The thread "<<self_tid<<" is running "<<endl;
        pthread_mutex_unlock(&output_mutex);
        task->run();
    }
    
}


void Meep::create_pool()
{
    pthread_id=new pthread_t[thread_num];
    for (int i=0; i<thread_num; ++i) {
        pthread_create(&pthread_id[i], NULL,thread_call,NULL);
    }
    cout<<"Already create "<<thread_num<<" threads"<<endl;
    return ;
}


size_t Meep::get_task_num()
{
    return task_list.size();
}

void Meep::add_task(Task *task)
{
    pthread_mutex_lock(&pthread_mutex);
    task_list.push_back(task);
    pthread_cond_signal(&pthread_cond);
    pthread_mutex_unlock(&pthread_mutex);

}

int Meep::stop()
{
    if(shutdown)
        return -1;
    shutdown=true;
    pthread_cond_broadcast(&pthread_cond);
    for(int i=0;i<thread_num;++i)
        pthread_join(pthread_id[i],NULL);
    delete pthread_id;
    pthread_mutex_destroy(&pthread_mutex);
    pthread_cond_destroy(&pthread_cond);
    return 0;
}

