//
//  Meep.cpp
//  Meep
//
//  Created by leviathan on 16/1/31.
//  Copyright © 2016年 leviathan. All rights reserved.
//



#include "Meep.hpp"


void Task::set_data(void *data)
{
    task_data=data;
}


Meep::Meep(int num)
{
    thread_num=num;
    create_pool();
}


void * Meep::thread_call(void * data)
{
    pthread_t self_tid=pthread_self();
    while (1) {
        pthread_mutex_lock(&pthread_mutex);
        while (task_list.size()==0&&!shutdown) {
            
        }
    }
    
}


void Meep::create_pool()
{
    pthread_id=new pthread_t[thread_num];
    for (int i=0; i<thread_num; ++i) {
        pthread_create(&pthread_id[i], NULL,thread_calL,NULL);
    }
    return ;
}


int Meep::get_task_num()
{
    return thread_num;
}
