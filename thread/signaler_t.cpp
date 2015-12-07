//
//  signaler_t.cpp
//  thread
//
//  Created by imkcy on 15/12/6.
//  Copyright © 2015年 imkcy. All rights reserved.
//

#include "signaler_t.hpp"
#include <sys/poll.h>
#include "fd.hpp"
#include "ip.hpp"
#include <unistd.h>
#include <sys/socket.h>
#include <sys/errno.h>
#include <assert.h>
static int sleep_ms(unsigned int ms_)
{
    if (ms_ == 0) {
        return 0;
    }
    
    return usleep(ms_ * 1000);
}

static int close_wait_ms(int fd_, unsigned int max_ms_ = 2000)
{
    unsigned int ms_so_far = 0;
    unsigned int step_ms = max_ms_ / 10;
    if (step_ms < 1) {
        step_ms = 1;
    }
    if (step_ms > 100) {
        step_ms = 100;
    }
    int rc = 0;
    
    do
    {
        if (rc == -1 && errno == EAGAIN) {
            sleep_ms(step_ms);
            ms_so_far += step_ms;
        }
        rc = close(fd_);
    } while (ms_so_far < max_ms_ && rc == -1 && errno == EAGAIN);
    return rc;
}

kcy::signaler_t::signaler_t()
{
    if (make_fdpair(&r,&w) == 0) {
        unblock_socket(w);
        unblock_socket(r);
    }
    pid = getpid();
}

kcy::signaler_t::~signaler_t()
{
    int rc = close_wait_ms(w);
    assert(rc == 0);
    rc = close_wait_ms(r);
    assert(rc == 0);
}

kcy::fd_t kcy::signaler_t::get_fd()
{
    
    return r;
}

void kcy::signaler_t::send()
{
    if (pid != getpid()) {
        return ;
    }
    unsigned char dummy = 0;
    while (true) {
        ssize_t nbytes = ::send(w,&dummy,sizeof(dummy),0);
        if (nbytes == -1 && errno == EINTR) {
            continue;
        }
        if (pid!=getpid()) {
            errno = EINTR;
            break;
        }
        assert(nbytes == sizeof(dummy));
        break;
    }
}

int kcy::signaler_t::wait(int timeout)
{
    if (pid != getpid()) {
        errno = EINTR;
        return -1;
    }
    
    struct pollfd pfd;
    pfd.fd = r;
    pfd.events = POLLIN;
    int rc = poll(&pfd, 1, timeout);
    if (rc < 0) {
        assert(errno == EINTR);
        return -1;
    }
    else
    if (rc == 0) {
        errno = EAGAIN;
        return -1;
    }
    if (pid != getpid()) {
        errno = EINTR;
        return -1;
    }
    assert(rc ==1);
    assert(pfd.revents & POLLIN);
    
    return 0;
}

void kcy::signaler_t::recv()
{
    unsigned char dummy;
    ssize_t nbytes = ::recv(r,&dummy,sizeof(dummy),0);
    assert(nbytes >= 0);
    assert(nbytes == sizeof(dummy));
    assert(dummy == 0 );
    
}

void kcy::signaler_t::forked()
{
    close(r);
    close(w);
    make_fdpair(&r, &w);
}

int kcy::signaler_t::make_fdpair(fd_t *r_, fd_t *w_)
{
    int sv[2];
    int rc = socketpair(AF_UNIX, SOCK_STREAM, 0, sv);
    if (rc == -1) {
        assert(errno == ENFILE || errno == EMFILE);
        *w_ = *r_ = -1;
        return -1;
    } else {
        *w_ = sv[0];
        *r_ = sv[1];
        return 0;
    }
}






















