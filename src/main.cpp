//
//  main.cpp
//  Meep
//
//  Created by leviathan on 16/1/31.
//  Copyright © 2016年 leviathan. All rights reserved.
//

#include <iostream>
#include "Meep.hpp"

using namespace Meep;
using std::cout;
using std::endl;

int main()
{
    Task task;
    string tmp="The thread is running task";
    task.set_data(tmp);
    Thread_pool t_pool(2);
    for (int i=0; i<5 ; i++)
        t_pool.add_task(&task);
    while (1)
    {
        sleep(0);
        pthread_mutex_lock(&Thread_pool::output_mutex);
        cout<<"There are still "<<t_pool.get_task_num()<<" tasks need to handle"<<endl;
        pthread_mutex_unlock(&Thread_pool::output_mutex);
        if (t_pool.get_task_num()==0&&Thread_pool::busy.size()==0)
        {
            t_pool.stop();
            if (t_pool.stop()==-1) {
                pthread_mutex_lock(&Thread_pool::output_mutex);
                cout<<"The main function will exit"<<endl;
                pthread_mutex_unlock(&Thread_pool::output_mutex);
                return 0;
            }
        }
    }
    return 0;
}


