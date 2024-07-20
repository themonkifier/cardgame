#ifndef SERVER_HH
#define SERVER_HH

#include "multiplayer.hh"
#include "client.hh"

class Server : public Client {
public:
    const int num_clients;
    int server_fd;
    int* client_fds;
    std::thread** recv_threads;

    Server(const int num_clients, Game* game);
    ~Server();

    void SetupActive() override;
    void sendMsg(std::string message) override;

protected:
    static void getData(Server* self, int client_fd);
    void setup_socket();
    void bind_socket(sockaddr* address);
    void listen_socket(sockaddr* address);
    void forward(int from, const char* message);
    int setup(sockaddr_in* address);
};

#endif
