//
//  ip.hpp
//  thread
//
//  Created by imkcy on 15/12/6.
//  Copyright © 2015年 imkcy. All rights reserved.
//

#ifndef ip_hpp
#define ip_hpp

#include <stdio.h>
#include <string>
#include "fd.hpp"
namespace kcy
{
    fd_t open_socket(int domain_, int type_, int protocol_);
    
    //Sets the socket into non_blocking mode.
    void unblock_socket(fd_t s_);
    
    //Returns string representation of peer's address. socket sockfd_ must be connected . Returns true if successful.
    bool get_peer_ip_address(fd_t sockfd_, std::string& ip_addr_);
}

#endif /* ip_hpp */
