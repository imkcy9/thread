//
//  ip.cpp
//  thread
//
//  Created by imkcy on 15/12/6.
//  Copyright © 2015年 imkcy. All rights reserved.
//

#include "ip.hpp"
#include <sys/socket.h>
#include <netdb.h>
#include <fcntl.h>
#include <assert.h>
kcy::fd_t kcy::open_socket(int domain_, int type_, int protocol_)
{
    fd_t s = socket(domain_, type_, protocol_);
    if (s == -1) {
        return -1;
    }
    int rc = fcntl(s, F_SETFD,FD_CLOEXEC);
    assert(rc != -1);
    return s;
}

void kcy::unblock_socket(fd_t s_)
{
    int flags = fcntl(s_, F_GETFL,0);
    if (flags == -1) {
        flags = 0;
    }
    int rc = fcntl(s_, F_SETFL,flags|O_NONBLOCK);
    assert(rc != -1);
}

bool kcy::get_peer_ip_address(fd_t sockfd_, std::string &ip_addr_)
{
    int rc;
    struct sockaddr_storage ss;
    socklen_t addrlen = sizeof(ss);
    rc = getpeername(sockfd_, (struct sockaddr*)&ss, &addrlen);
    if (rc == -1) {
        return false;
    }
    
    char host[NI_MAXHOST];
    rc = getnameinfo((struct sockaddr*)&ss, addrlen, host, sizeof(host), NULL, 0, NI_NUMERICHOST);
    if (rc != 0) {
        return false;
    }
    ip_addr_ = host;
    return true;
}