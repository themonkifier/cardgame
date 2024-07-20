#include "../include/text_box.hh"

TextBox::TextBox(Client* client, Game* game)
    : selected(false), cursor_pos(0),
      contained_text(), client(client),
      game(game)
{}

TextBox::TextBox(Game* game)
    : selected(false), cursor_pos(0),
      contained_text(), client(nullptr),
      game(game)
{}

void TextBox::keyCallback(int key, int action) {
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_ESCAPE:
            selected = false;
            break;

            case GLFW_KEY_BACKSPACE:
            if (cursor_pos > 0) {
                contained_text = contained_text.substr(0, cursor_pos - 1) + contained_text.substr(cursor_pos, contained_text.size());
                cursor_pos--;
            }
            break;

            case GLFW_KEY_DELETE:
            if (cursor_pos < contained_text.size()) {
                contained_text = contained_text.substr(0, cursor_pos) + contained_text.substr(cursor_pos + 1, contained_text.size());
            }
            break;

            case GLFW_KEY_LEFT:
            if (cursor_pos > 1) {
                cursor_pos--;
            }
            break;

            case GLFW_KEY_RIGHT:
            if (cursor_pos < contained_text.size()) {
                cursor_pos++;
            }
            break;

            case GLFW_KEY_ENTER:
            enterCallback(game, this);
            if (client != nullptr) {
                client->sendMsg(contained_text);
            }

            contained_text.clear();
            cursor_pos = 0;
            break;
        }
    }
}

void TextBox::characterCallback(unsigned int codepoint) {
    contained_text = contained_text.substr(0, cursor_pos) + static_cast<char>(codepoint) + contained_text.substr(cursor_pos);
    cursor_pos++;
}