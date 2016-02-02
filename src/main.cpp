//
//  main.cpp
//  Meep
//
//  Created by weizhiyun－70 on 16/1/31.
//  Copyright © 2016年 weizhiyun－70. All rights reserved.
//

#include <iostream>
#include "Meep.hpp"
using namespace std;

int main()
{
    Task task;
    string tmp="The thread is running task";
    task.set_data(tmp);
    Meep t_pool(2);
    for (int i=0; i<5 ; i++)
        t_pool.add_task(&task);
    while (1)
    {
        sleep(0);
        pthread_mutex_lock(&Meep::output_mutex);
        cout<<"There are still "<<t_pool.get_task_num()<<" tasks need to handle"<<endl;
        pthread_mutex_unlock(&Meep::output_mutex);
        if (t_pool.get_task_num()==0&&Meep::busy.size()==0)
        {
            t_pool.stop();
            if (t_pool.stop()==-1) {
                pthread_mutex_lock(&Meep::output_mutex);
                cout<<"The main function will exit"<<endl;
                pthread_mutex_unlock(&Meep::output_mutex);
                exit(0);
            }
        }
    }
    return 0;
}


