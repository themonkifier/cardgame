#ifndef GAME_HH
#define GAME_HH

#include "text_renderer.hh"
#include "deck.hh"

class TextBox;
#include "text_box.hh"

class Client;
class Server;
#include "pile.hh"

#include <GLFW/glfw3.h>

#include <string>
#include <array>
#include <unordered_map>

class Game
{
public:
    enum class GameState
    {
        MENU = 'M',
        JOIN = 'J',
        ACTIVE = 'A',
        WIN = 'W'
    } State;
    void SetState(GameState newState);
    
    GLFWwindow* window;
    TextRenderer textRenderer;
    std::array<std::pair<bool, std::pair<double, double>>, 2> MouseInput; // isPressed, (xpos, ypos)
    std::vector<std::unique_ptr<Pile>> piles;
    std::vector<std::unique_ptr<Deck>> decks;
    std::vector<std::unique_ptr<Pile>> hands;

    std::unordered_map<std::string, std::unique_ptr<Texture>> ui_elements;
    std::unordered_map<std::string, std::unique_ptr<TextBox>> text_boxes;

    int id;

    /* this couldn't be a shared ptr because of make_shared somehow
        calling the default constructor, which incremented zlevel,
        so the zlevel comparison in Card::operator==() would fail
        (i don't have internet access right now so i can't check
         if there's a better solution that uses shared_ptrs, sry!)*/
    Card* selectedCard;

    // constructor/destructor
    Game(TextRenderer& textRenderer, GLFWwindow* window);
    ~Game();

    std::unique_ptr<Client> client; // OR SERVER...

    // functions to be run when specific keys are pressed
    void OnEscape();
    std::string Notify(std::string message);

    // initialize game state (load all shaders/textures/ui elements)
    void InitMenu();
    void InitJoin();
    void InitWin();
    void InitActive();

    // game loop
    // calls the appropriate private helper method depending on this->State
    void Init();
    void ProcessInput(float dt);
    void Update(float dt);
    void Render();
private:
    void UpdateMenu(float dt); //?
    void UpdateJoin(float dt);
    void UpdateWin(float dt); //?
    void UpdateActive(float dt);

    void ProcessMenu(float dt);
    void ProcessJoin(float dt);
    void ProcessWin(float dt);
    void ProcessActive(float dt);

    void RenderMenu();
    void RenderJoin();
    void RenderWin();
    void RenderActive();

    std::string GetStateString();
    bool onTop(Texture& texture, Pile& pile, std::pair<double, double>& clickPosition);
};

#endif
