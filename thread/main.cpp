//
//  main.cpp
//  thread
//
//  Created by imkcy on 15/12/5.
//  Copyright © 2015年 imkcy. All rights reserved.
//

#include <iostream>
#include <stdlib.h>
#include <pthread.h>
using namespace std;
#include "KcyPipe.hpp"
#include "KcyThread.hpp"
#include "signaler_t.hpp"

void a(void*);
int main(int argc, const char * argv[]) {
    kcy::KcyThread thread_t;
    thread_t.start(a, NULL);
    thread_t.stop();
    kcy::signaler_t sig;
    //sig.send();
    //sig.wait(10000);
    sig.recv();
    return 0;
}

void a(void*)
{

    std::cout << "hello world" << std::endl;
    return ;
}