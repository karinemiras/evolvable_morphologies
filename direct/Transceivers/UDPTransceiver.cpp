//
//  UDPTransceiver.cpp
//  RobotController
//
//  Created by Berend Weel on 10/04/15.
//  Copyright (c) 2015 Berend Weel. All rights reserved.
//

#include <iostream>
#include <arpa/inet.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h>

#include "UDPTransceiver.h"

UDPTransceiver::UDPTransceiver (int port) :
    port(port)
{
    bindLocalSocket();
    retrieveLocalIp();
}

void UDPTransceiver::bindLocalSocket() {
    socketfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketfd < 0) {
        error("ERROR opening socket");
    }
    
    int broadcastEnable=1;
    int ret = setsockopt(socketfd, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));
    if (ret < 0) {
        error("ERROR setting socket options");
    }
    
    struct sockaddr_in serv_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);
    
    if (bind(socketfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");
}

void UDPTransceiver::retrieveLocalIp() {
    struct ifaddrs * ifAddrStruct=NULL;
    struct ifaddrs * ifa=NULL;
    void * tmpAddrPtr=NULL;
    
    getifaddrs(&ifAddrStruct);
    
    for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
        if (!ifa->ifa_addr) {
            continue;
        }
        if (ifa->ifa_addr->sa_family == AF_INET) { // check it is IP4
            // is a valid IP4 Address
            tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
//            printf("%s IP Address %s\n", ifa->ifa_name, addressBuffer);
            
            if(strncmp(ifa->ifa_name,"en0",3) == 0) {
                localIp = addressBuffer;
            }
        }
    }
    
    std::cout << "Found local ip: " << localIp << std::endl;
}

UDPTransceiver::~UDPTransceiver() {
}

UDPTransceiver::UDPTransceiver (std::string genome, int port) :
    UDPTransceiver (port) {
    this->genome = genome;
}

void UDPTransceiver::broadcastGenome() {
    struct sockaddr_in s;
    
    memset(&s, '\0', sizeof(struct sockaddr_in));
    s.sin_family = AF_INET;
    s.sin_port = (in_port_t)htons(13337);
    s.sin_addr.s_addr = htonl(INADDR_BROADCAST);
    
    send(s);
}

void UDPTransceiver::send(struct sockaddr_in &address) {
    if(sendto(socketfd, genome.c_str(), genome.length()+1, 0, (struct sockaddr *) &address, sizeof(struct sockaddr_in)) < 0)
        perror("ERROR sending genome");
}

void UDPTransceiver::receive() {
    struct sockaddr_in sender;
    char buffer[2048];
    long recvlength;
    socklen_t addrlength;
    
    addrlength = sizeof(struct sockaddr_in);

    recvlength = recvfrom(socketfd, (void*)buffer, 2048, 0, (struct sockaddr *) &sender, &addrlength);
    char tmpAddr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &sender.sin_addr, tmpAddr, INET_ADDRSTRLEN);
    
    std::string ipAddress = tmpAddr;
    
    if(recvlength < 0) {
        perror("ERROR receiving");
    } else if (recvlength > 0) {
        std::cout << "Received message of length: " << recvlength << " from: " << ipAddress << ":" << sender.sin_port << " it says: " << buffer << std::endl;
    }
}

void UDPTransceiver::step () {
    broadcastGenome();
    
    receive();
}

void UDPTransceiver::error(const char *msg)
{
    perror(msg);
    exit(0);
}