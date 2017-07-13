//
//  UDPTransceiver.h
//  RobotController
//
//  Created by Berend Weel on 10/04/15.
//  Copyright (c) 2015 Berend Weel. All rights reserved.
//

#ifndef __RobotController__UDPTransceiver__
#define __RobotController__UDPTransceiver__

#include "GenomeTransceiver.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

class UDPTransceiver : public GenomeTransceiver {
public:
    UDPTransceiver(int port);
    virtual ~UDPTransceiver();
    UDPTransceiver(std::string genome, int port);
    
    virtual void broadcastGenome();
    
    virtual void send(struct sockaddr_in &address);
    virtual void receive();
    
    virtual void step();
    
private:
    int socketfd;
    std::string genome;
    int port;
    std::string localIp;
    
    void error(const char *msg);
    
    void bindLocalSocket();
    void retrieveLocalIp();
};

#endif /* defined(__RobotController__UDPTransceiver__) */
