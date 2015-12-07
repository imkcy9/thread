//
//  signaler_t.hpp
//  thread
//
//  Created by imkcy on 15/12/6.
//  Copyright © 2015年 imkcy. All rights reserved.
//

#ifndef signaler_t_hpp
#define signaler_t_hpp

#include "fd.hpp"
#include <unistd.h>
namespace kcy
{
    class signaler_t
    {
    public:
        signaler_t();
        ~signaler_t();
        
        fd_t get_fd();
        void send();
        int wait(int timeout);
        void recv();
        
        void forked();
    private:
        //Create a pair of filedescriptors that will be used to pass the signals
        static int make_fdpair(fd_t *r_,fd_t *w);
        //write & read file descriptor will be -1 if we exceeded number of availabe handles
        fd_t w;
        fd_t r;
        
        signaler_t(const signaler_t&);
        const signaler_t& operator=(const signaler_t&);
        
        pid_t pid;
        void close_internal();
    };
}

#endif /* signaler_t_hpp */
