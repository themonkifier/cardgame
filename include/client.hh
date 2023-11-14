#ifndef CLIENT_HH
#define CLIENT_HH

#include "multiplayer.hh"

class Client {
public:
    int fd;
    sockaddr_in address;

    Client();
    ~Client();

    int sendMessage(std::string message);
};

#endif