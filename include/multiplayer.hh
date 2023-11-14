#ifndef MULTIPLAYER_I_HH
#define MULTIPLAYER_I_HH

#include <iostream>
#include <string>
#include <thread>
#include <vector>

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

#define DOMAIN AF_INET

/* picked by lindsay wang on nov 10th 2023 */
#define PORT 7723

const char* GetIPAddress();

#endif