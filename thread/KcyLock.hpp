//
//  KcyLock.hpp
//  thread
//
//  Created by imkcy on 15/12/5.
//  Copyright © 2015年 imkcy. All rights reserved.
//

#ifndef KcyLock_hpp
#define KcyLock_hpp

#include <pthread.h>
#include <assert.h>
namespace kcy
{
    class KcyLock
    {
    public:
        inline KcyLock()
        {
            int rc = pthread_mutex_init(&mutex, NULL);
            assert(0 == rc);
        }
        inline ~KcyLock()
        {
            int rc = pthread_mutex_destroy(&mutex);
            assert(0 == rc);
        }
        
        inline void lock()
        {
            int rc = pthread_mutex_lock(&mutex);
            assert(0 == rc);
        }
        
        inline void try_lock()
        {
            int rc = pthread_mutex_trylock(&mutex);
            assert(0 == rc);
        }
        
        inline void unlock()
        {
            int rc = pthread_mutex_unlock(&mutex);
            assert(0 == rc);
        }
        
    private:
        pthread_mutex_t mutex;
    
        KcyLock(const KcyLock&);
        const KcyLock& operator=(const KcyLock&);
    };
    
}

namespace kcy
{
    class KcyAutoLock
    {
    public:
        KcyAutoLock(KcyLock& mutex_):
        mutex(mutex_)
        {
            mutex.lock();
        }
        ~KcyAutoLock()
        {
            mutex.unlock();
        }
    private:
        KcyLock& mutex;
        
        KcyAutoLock(const KcyAutoLock&);
        const KcyAutoLock& operator=(const KcyAutoLock&);
    };
}


#endif /* KcyLock_hpp */
