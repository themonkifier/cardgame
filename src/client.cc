#include "../include/client.hh"

Client::Client() {
    fd = socket(DOMAIN, SOCK_STREAM, IPPROTO_TCP);

    if (fd < 0) {
        perror("client socket couldn't be opened");
        exit(1);
    }

    address = {.sin_family = DOMAIN, .sin_port = htons(PORT)};

    inet_pton(DOMAIN, GetIPAddress(), &address.sin_addr);
    connect(fd, (sockaddr*) &address, sizeof(address));
}

Client::~Client() {
    close(fd);
}

int Client::sendMessage(std::string message) {
    const char* data = message.c_str();
    return send(fd, &data, sizeof(data), 0);
}