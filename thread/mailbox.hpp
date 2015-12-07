//
//  mailbox.hpp
//  thread
//
//  Created by imkcy on 15/12/6.
//  Copyright © 2015年 imkcy. All rights reserved.
//

#ifndef mailbox_hpp
#define mailbox_hpp

#include <stdio.h>
#include "command.hpp"
#include "fd.hpp"
#include "KcyPipe.hpp"
#include "KcyLock.hpp"
#include "signaler_t.hpp"
namespace kcy
{
    class mailbox_t
    {
    public:
        mailbox_t();
        ~mailbox_t();
        
        fd_t get_fd();
        void send(const command_t&cmd);
        int recv(command_t*cmd_,int timeout_);
        
        void forked() {signaler.forked();}
        
        
    private:
        typedef KcyPipe<command_t,16> cpipe_t;
        cpipe_t cpipe;
        
        signaler_t signaler;
        KcyLock sync;
        bool active;
        
        mailbox_t(const mailbox_t&);
        const mailbox_t& operator=(const mailbox_t&);
        
    };
}
#endif /* mailbox_hpp */
