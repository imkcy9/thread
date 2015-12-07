//
//  KcyThread.cpp
//  thread
//
//  Created by imkcy on 15/12/6.
//  Copyright © 2015年 imkcy. All rights reserved.
//

#include "KcyThread.hpp"
#include <assert.h>

static void *thread_routine(void* arg_)
{
    kcy::KcyThread* self = (kcy::KcyThread*)arg_;
    self->tfn(self->arg);
    return NULL;
}

void kcy::KcyThread::start(thread_fn *tfn_, void *arg_)
{
    tfn = tfn_;
    arg = arg_;
    int rc = pthread_create(&descriptor, NULL, thread_routine, this);
    assert(0 == rc);
}

void kcy::KcyThread::stop()
{
    int rc = pthread_join(descriptor, NULL);
    assert(0 == rc);
}