#include "../include/server.hh"
#include "../include/info.hh"

Server::Server(const int num_clients, Game* game)
    : Client(game), num_clients(num_clients)
{
    sockaddr_in address = {.sin_family = DOMAIN, .sin_port = htons(PORT)};
    address.sin_addr.s_addr = INADDR_ANY;

    socklen_t addrlen;

    /* creates socket for server */
    // printf("!%d?\n", server_fd = socket(DOMAIN, SOCK_STREAM, 0));
    // printf("!%d?\n", bind(server_fd, (sockaddr*) &address, sizeof(address)));
    // printf("!%d?\n", listen(server_fd, 32));
    setup_socket();
    bind_socket((sockaddr*) &address);
    listen_socket((sockaddr*) &address);

    client_fds = new int[num_clients];
    recv_threads = new std::thread*[num_clients];

    for (int i = 0; i < num_clients; i++) {
        client_fds[i] = accept(server_fd, (sockaddr*) &address, &addrlen);
    }

    for (int i = 0; i < num_clients; i++) {
        recv_threads[i] = new std::thread(getData, this, client_fds[i]);
    }
}

void Server::setup_socket() {
    if ((server_fd = socket(DOMAIN, SOCK_STREAM, 0)) < 0) {
        double wait_time = glfwGetTime() + 60 * 2; // wait for 2 minutes;

        do {
            printf("retrying call to socket()...\n");
            sleep(2);
            printf("socket: %d\n", server_fd = socket(DOMAIN, SOCK_STREAM, 0));
        }
        while (server_fd < 0 && glfwGetTime() < wait_time);

        if (server_fd < 0) {
            perror("call to socket() failed");
            game->SetState(Game::GameState::MENU);
            return;
        }
    }

    printf("call to socket() succeeded\n");
}

void Server::bind_socket(sockaddr* address) {
    if (bind(server_fd, address, sizeof(*address)) < 0) {
        sleep(5);
        double wait_time = glfwGetTime() + 60 * 2;
        int error_code;

        do {
            printf("retrying bind()...\n");
            close(server_fd);
            setup_socket();
            error_code = bind(server_fd, (sockaddr*) &address, sizeof(address));
            printf("server_fd: %d, error_code: %d\n", server_fd, error_code);
        }
        while (error_code < 0 && glfwGetTime() < wait_time);

        if (error_code < 0) {
            perror("bind() failed");
            game->SetState(Game::GameState::MENU);
            return;
        }
    }

    printf("bind() succeeded\n");
}

void Server::listen_socket(sockaddr* address) {
    if (listen(server_fd, 32) < 0) {
        double wait_time = glfwGetTime() + 60 * 2;
        int error_code;

        do {
            printf("retrying listen()...\n");
            close(server_fd);
            setup_socket();
            bind_socket(address);
            printf("%d", error_code = listen(server_fd, 32));
        } while (error_code < 0 && glfwGetTime() < wait_time);

        if (error_code < 0) {
            perror("listen() failed");
            game->SetState(Game::GameState::MENU);
            return;
        }
    }

    printf("listen() succeeded\n");
}

Server::~Server() {
    // for (int i = 0; i < num_clients; i++) {
        // recv_threads[i].join();
    // }

    close(server_fd);
    std::cout.flush();

    for (int i = 0; i < num_clients; i++) {
        close(client_fds[i]);
    }

    delete[] client_fds;
    delete[] recv_threads;
}

/* Data Format:
    ACTION: d(raw) - 1 (more) byte, p(lay) - 2 (more) bytes , g(enerate deck) - 1 (more) byte
    d: (none)
    p: suit, rank
        suit: D(iamonds), S(pades), C(lubs), H(earts)
        rank: A(ce), 2, 3, 4, 5, 6, 7, 8, 9, T(en), J(ack), Q(ueen), K(ing)
    g: length, card info[length]
        length: 0 - 255
        card info: same format as p, but repeated `length` times
*/
void Server::getData(Server* self, int client_fd) {
    char buffer[256];

    do {
        //self->semaphore.acquire();
        if (recv(client_fd, buffer, 1UL, 0) != 1) {
            perror("server couldn't read action type");
            exit(EXIT_FAILURE);
        }
        switch (buffer[0]) {
            case 'd':
            self->received_messages.push_back("d");
            break;

            case 'p':
            {
                if (recv(client_fd, buffer + 1, 2UL, 0) != 2) {
                    perror("server couldn't read card played");
                    exit(EXIT_FAILURE);
                }
                self->received_messages.emplace_back(buffer, 3);
            }
            break;
        }
    } while (buffer[0] != 'q');

    printf("client %d disconnected, closing...\n", client_fd);
    self->quit();
}

void Server::sendMsg(std::string message) {
    for (int i = 0; i < num_clients; i++) {
        send(client_fds[i], message.c_str(), message.size(), 0);
    }
}

void Server::SetupActive() {
    //server.semaphore.acquire();

    std::unique_ptr<Pile> draw = std::make_unique<Pile>(*game->decks[0]);
    draw->fillFromDeck();
    draw->fillFromDeck();
    draw->shuffle();
    draw->move({SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 - 50});

    // create a hand pile for each person playing
    for (std::size_t i = 0; i < NUM_PLAYERS; i++) {
        game->hands.push_back(std::make_unique<Pile>(*game->decks[0], i == 0));
    }

    // deal 7 cards to each person’s pile
    for (std::size_t i = 0; i < game->hands.size(); i++) {
        std::string message = "h";
        message.reserve(4 + DEFAULT_HAND_SIZE);

        message += static_cast<unsigned char>(game->hands.size());
        message += static_cast<unsigned char>(DEFAULT_HAND_SIZE);

        message += std::to_string(i);

        for (std::size_t j = 0; j < 7; j++) {
            if (draw->size <= 0) {
                draw->fillFromDeck();
                draw->fillFromDeck();
            }
            /* ignoring possible empty optional becase we just checked that the deck isn't empty! */
            auto& c = *(draw->deal(*game->hands[i]).value());
            auto s = to_string(c);
            message += s;
        }
        game->hands[i]->move({i * 100, i * 100});
        sendMsg(message);
    }

    // create discard pile
    std::unique_ptr<Pile> discard = std::make_unique<Pile>(*game->decks[0]);

    // deal top card from deck to discard pile and repeat until it's not an 8
    do {
        if (draw->size <= 0) {
            draw->fillFromDeck();
            draw->fillFromDeck();
        }
        draw->deal(*discard, discard->size - 1);
    } while ((*discard)[discard->size - 1].value.second == 8);
    discard->move({SCREEN_WIDTH / 2 + 50, SCREEN_HEIGHT / 2 + 50});

    sendMsg("g0" + to_string(*draw));
    sendMsg("g1" + to_string(*discard));

    game->piles.push_back(std::move(draw));
    game->piles.push_back(std::move(discard));
    //server.semaphore.release();
}

void Server::forward(int client_fd, const char* message) {
    for (int i = 0; i < num_clients; i++) {
        if (client_fds[i] != client_fd) {
            send(client_fds[i], message, 31UL, 0);
        }
    }
}
