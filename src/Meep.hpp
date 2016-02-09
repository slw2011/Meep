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
#include <pthread.h>
#include <queue>
#include <functional>

using std::string;

namespace Meep {
    //Task class
    class Task
    {
    public:
        friend class Meep;
        Task();
        Task(std::string data):
            task_data(data){};
        ~Task();
        inline void set_data(const std::string & data);
        void run(pthread_t pid);
    private:
        static int sign;
        std::string task_data;
    };
    
    //inline function
    void Task::set_data(const string & data)
    {
        task_data=data;
    }
    //Thead pool class
    class Thread_pool
    {
    public:
        Thread_pool(int num); //init thread_num
        ~Thread_pool();
        void create_pool();
        static void *thread_call(void * data);
        static void move2idle(const pthread_t pid);
        static void move2busy(const pthread_t pid);
        void add_task(Task * task);
        int stop();
        size_t get_task_num();
        static pthread_mutex_t output_mutex;
        static std::vector<pthread_t > busy;
        static std::vector<pthread_t > idle;
    private:
        static std::vector<Task *> task_list;
        static bool shutdown;
        int thread_num;
        pthread_t * pthread_id;
        static pthread_mutex_t pthread_mutex;
        static pthread_cond_t pthread_cond;
        static pthread_mutex_t list_mutex;
        
    };
}

#endif /* MEEP_HPP_ */
