#ifndef SERVER_HH
#define SERVER_HH

#include <vector>

#include "info.hh"
#include "multiplayer.hh"


class Server {
public:
    std::vector<std::thread> threads;
    std::vector<int> clients;
    int fd;
    sockaddr_in address;

    Server();
    ~Server();

private:
    socklen_t addrlen;

    /* Data Format:
        ACTION: d(raw) - 1 (more) byte, p(lay) - 2 (more) bytes , g(enerate deck) - 1 (more) byte
        d: (none)
        p: suit, rank
            suit: d(iamonds), s(pades), c(lubs), h(earts)
            rank: a(ce), 2, 3, 4, 5, 6, 7, 8, 9, t(en), j(ack), q(ueen), k(ing)
        g: length, card info[length]
            length: 0 - 255
            card info: same format as p, but repeated `length` times
    */
    static void get_data(int client);

    enum class DataType {
        DRAW = 'd',
        PLAY = 'p',
        GENERATE_DECK = 'g',
        QUIT = 'q'
    };
};

#endif