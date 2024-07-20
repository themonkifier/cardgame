#ifndef CLIENT_HH
#define CLIENT_HH

#include "multiplayer.hh"

class Game;
#include "game.hh"
#include <deque>

class Client {
public:
    int server_fd;
    // std::binary_semaphore semaphore;
    std::string ip;
    std::deque<std::string> received_messages;
    std::thread* recv_thread;
    Game* game;

    Client(std::string ip, Game* game);
    virtual ~Client();

    virtual void SetupActive();
    virtual void sendMsg(std::string message);
    virtual std::unique_ptr<std::string> getLastMessage();
    virtual void quit();

protected:
    Client(Game* game);

    static void getData(Client* self, int server_fd);
};

#endif
