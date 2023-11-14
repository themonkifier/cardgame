#include "../include/server.hh"

Server::Server() {
    fd = -1;

    /* creates socket for server */
    if ((fd = socket(DOMAIN, SOCK_STREAM, 0)) < 0) {
        perror("server call to socket() failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = DOMAIN;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(fd, (sockaddr*) &address, sizeof(address)) < 0) {
        perror("server bind() failed");
        close(fd);
        exit(EXIT_FAILURE);
    }

    if (listen(fd, 128) < 0) {
        perror("server listen() failed");
        close(fd);
        exit(EXIT_FAILURE);
    }


    for (int i = 0; i < NUM_PLAYERS; i++) {
        clients.push_back(accept(fd, (sockaddr*) &address, &addrlen));
        threads.push_back(std::thread(get_data, clients[i]));
    }

    for (int i = 0; i < NUM_PLAYERS; i++) {
        threads[i].join();
    }
}
Server::~Server() {
    close(fd);

    for (int i = 0; i < NUM_PLAYERS; i++) {
        close(clients[i]);
    }
}

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
void Server::get_data(int client) {
    int8_t length;
    int8_t buffer[2] = {0};

    do {
        int valread = recv(client, &length, 1, 0);

        if (DataType(length) == DataType::PLAY) {
            valread = recv(client, buffer, 2, 0); // &buffer?
        }
        else if (DataType(length) == DataType::GENERATE_DECK) {
            valread = recv(client, &length, 1, 0);
            for (int8_t i = 0; i < length; i++) {
                valread = recv(client, buffer, 2, 0);
            }
        }
        else if (DataType(length) == DataType::DRAW){
            ;
        }
    } while (DataType(buffer[0]) != DataType::QUIT);
}