#ifndef TEXT_BOX_HH
#define TEXT_BOX_HH

#include <string>

class Client;
#include "client.hh"
#include "game.hh"

class TextBox {
public:
    bool selected;
    std::size_t cursor_pos;
    std::string contained_text;
    Client* client;
    Game* game;

    TextBox(Game* game);
    TextBox(Client* client, Game* game);

    void (*enterCallback)(Game*, TextBox*);

    /* glfw callbacks */
    void keyCallback(int key, int action);
    void characterCallback(unsigned int codepoint);
};

#endif