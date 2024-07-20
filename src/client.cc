#include "../include/client.hh"
#include "../include/info.hh"

Client::Client(std::string ip, Game* game)
    : /*semaphore{0},*/ ip(ip), game(game)
{
    if ((server_fd = socket(DOMAIN, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        perror("socket couldn't be opened");
        exit(1);
    }

    sockaddr_in address = {.sin_family = DOMAIN, .sin_port = htons(PORT)};

    inet_pton(DOMAIN, ip.c_str(), &address.sin_addr);
    if (connect(server_fd, (sockaddr*) &address, sizeof(address)) < 0) {
        perror("server failed to connect");
        exit(EXIT_FAILURE);
    }

    recv_thread = new std::thread(getData, this, server_fd);
}

Client::Client(Game* game)
    : /*semaphore{0},*/ game(game)
{}

Client::~Client() {
    // recv_thread.join();

    delete recv_thread;
    close(server_fd);
}

void Client::getData(Client* self, int server_fd) {
    char buffer[256];

    do {
        if (recv(server_fd, buffer, 1UL, 0) != 1) {
            perror("client couldn't read action type");
            exit(EXIT_FAILURE);
        }
        switch (buffer[0]) {
            case 'd':
            self->received_messages.push_back("d");
            break;

            case 'p':
            {
                if (recv(server_fd, buffer + 1, 2UL, 0) != 2) {
                    perror("client couldn't read card played");
                    exit(EXIT_FAILURE);
                }
                self->received_messages.emplace_back(buffer, 3);
            }
            break;

            case 'r':
            {
                if (recv(server_fd, buffer + 1, 2UL, 0) != 2) {
                    perror("client couldn't read refill info");
                    exit(EXIT_FAILURE);
                }
                unsigned char length = std::min<unsigned char>(127, buffer[2]) * 2 + 1; // not 255 because each card is 2 bytes

                std::string message = {'r', buffer[0], buffer[1]};
                message.reserve(3 + length);

                if (recv(server_fd, buffer, length, 0) < length) {
                    perror("client couldn't read refill data");
                    exit(EXIT_FAILURE);
                }
                message += std::string(buffer, length);

                self->received_messages.emplace_back(message);
            }
            break;

            case 'g':
            {
                if (recv(server_fd, buffer, 2UL, 0) != 2) {
                    perror("client couldn't read generation info");
                    exit(EXIT_FAILURE);
                }
                unsigned char length = std::min<unsigned char>(127, buffer[1]) * 2;

                std::string message = {'g', buffer[0], buffer[1]};
                message.reserve(3 + length);

                if (recv(server_fd, buffer, length, 0) < length) {
                    perror("client couldn't read generation data");
                    exit(EXIT_FAILURE);
                }
                message += std::string(buffer, length);

                self->received_messages.emplace_back(message);
            }
            break;

            case 'h':
            {
                if (recv(server_fd, buffer, 2UL, 0) != 2) {
                    perror("client couldn't read hand info");
                    exit(EXIT_FAILURE);
                }
                unsigned char length = std::min<unsigned char>(127, buffer[1]) * 2 + 1;

                std::string message = {'h', buffer[0], buffer[1], buffer[2]};
                message.reserve(4 + length);

                if (recv(server_fd, buffer, length, 0) < length) {
                    perror("client couldn't read hand data");
                    exit(EXIT_FAILURE);
                }
                message += std::string(buffer, length);
                
                self->received_messages.emplace_back(message);
            }
            break;
        }
    } while (buffer[0] != 'q');

    printf("disconnected from server\n");
}

std::unique_ptr<std::string> Client::getLastMessage() {
    std::unique_ptr<std::string> last_message = std::make_unique<std::string>(received_messages.front());
    received_messages.pop_front();
    return last_message;
}

void Client::sendMsg(std::string message) {
    send(server_fd, message.c_str(), message.size(), 0);
}

void Client::SetupActive() {
    while (received_messages.size() == 0);
    for (unsigned int i = 0; i < NUM_PLAYERS; i++) {
        std::unique_ptr<Pile> hand = std::make_unique<Pile>(*game->decks[0], getLastMessage()->substr(5));
        hand->move({i * 100, i * 100});
        game->hands.push_back(std::move(hand));
    }

    std::unique_ptr<Pile> draw = std::make_unique<Pile>(*game->decks[0], getLastMessage()->substr(3));
    draw->move({SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 - 50});
    game->piles.push_back(std::move(draw));

    std::unique_ptr<Pile> discard = std::make_unique<Pile>(*game->decks[0], getLastMessage()->substr(3));
    discard->move({SCREEN_WIDTH / 2 + 50, SCREEN_HEIGHT / 2 + 50});
    game->piles.push_back(std::move(discard));
}

void Client::quit() {
    sendMsg("q");
    delete this;
}
