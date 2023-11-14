#ifndef GAME_HH
#define GAME_HH

#include "resource_manager.hh"
#include "renderer.hh"
#include "text_renderer.hh"
#include "deck.hh"
#include "info.hh"

#include <string>
#include <array>
#include <unordered_map>

class Game
{
public:
    enum class GameState
    {
        MENU,
        JOIN,
        ACTIVE,
        WIN
    } State;
    
    std::array<bool, 1024> Keys;
    std::array<std::pair<bool, std::pair<double, double> >, 2> MouseInput; // isPressed, (xpos, ypos)
    /* this couldn't be a shared ptr because of make_shared somehow
        calling the default constructor, which incremented zlevel,
        so the zlevel comparison in Card::operator==() would fail
        (i don't have internet access right now so i can't check
         if there's a better solution that uses shared_ptrs, sry!)*/
    Card* selectedCard;

    unsigned int Width, Height;
    // constructor/destructor
    Game(unsigned int width, unsigned int height);
    ~Game();
    // initialize game state (load all shaders/textures/levels)

    void InitMenu();
    void InitJoin();
    void InitWin();
    void InitActive();

    // game loop
    // calls the appropriate private helper method depending on this->State
    void ProcessInput(float dt);
    void Update(float dt);
    void Render();
private:
    void UpdateMenu(float dt); //?
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

    bool onTop(Texture& texture, Pile<Card>& pile, std::pair<double, double>& clickPosition);
};

#endif