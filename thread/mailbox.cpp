//
//  mailbox.cpp
//  thread
//
//  Created by imkcy on 15/12/6.
//  Copyright © 2015年 imkcy. All rights reserved.
//

#include "mailbox.hpp"
#include <errno.h>
kcy::mailbox_t::mailbox_t()
{
    bool ok = cpipe.read(NULL);
    assert(!ok);
    active = false;
}

kcy::mailbox_t::~mailbox_t()
{
    sync.lock();
    sync.unlock();
}

kcy::fd_t kcy::mailbox_t::get_fd()
{
    return signaler.get_fd();
}

void kcy::mailbox_t::send(const kcy::command_t &cmd)
{
    sync.lock();
    cpipe.write(cmd, false);
    bool ok = cpipe.flush();
    sync.unlock();
    if (!ok) {
        signaler.send();
    }
}

int kcy::mailbox_t::recv(kcy::command_t *cmd_, int timeout_)
{
    if (active) {
        bool ok = cpipe.read(cmd_);
        if (ok) {
            return 0;
        }
        //no more commands available
        active = false;
        signaler.recv();
    }
    
    int rc = signaler.wait(timeout_);
    if (rc != 0 && (errno == EAGAIN || errno == EINTR)) {
        return -1;
    }
    active = true;
    assert(rc == 0);
    bool ok = cpipe.read(cmd_);
    assert(ok);
    return 0;
}































