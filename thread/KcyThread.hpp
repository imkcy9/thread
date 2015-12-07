//
//  KcyThread.hpp
//  thread
//
//  Created by imkcy on 15/12/6.
//  Copyright © 2015年 imkcy. All rights reserved.
//

#ifndef KcyThread_hpp
#define KcyThread_hpp

#include <pthread.h>
namespace kcy
{
    typedef void (thread_fn)(void*);
    
    class KcyThread
    {
    public:
        inline KcyThread()
        {
            
        }
        
        void start(thread_fn* tfn_, void *arg_);
        
        void stop();
        
        thread_fn* tfn;
        void* arg;
        
    private:
        
        pthread_t descriptor;
        
        KcyThread(const KcyThread&);
        const KcyThread& operator=(const KcyThread&);
    };
}
#endif /* KcyThread_hpp */
