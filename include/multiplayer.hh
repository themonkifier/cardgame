#ifndef MULTIPLAYER_HH
#define MULTIPLAYER_HH

#include <iostream>
#include <string>
#include <thread>

#include <cstring>
#include <unistd.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/poll.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <netdb.h>

/* picked by lindsay wang on nov 10th 2023 */
#define PORT 7723
#define DOMAIN AF_INET
#define SERVER_IP "127.0.0.1"

namespace Multiplayer {
    const char* GetIPAddress();

    enum class DataType {
        DRAW = 'd',
        PLAY = 'p',
        GENERATE_DECK = 'g',
        QUIT = 'q'
    };
}

#endif