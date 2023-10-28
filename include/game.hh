#ifndef GAME_HH
#define GAME_HH

#include "resource_manager.hh"
#include "renderer.hh"
#include "deck.hh"

#include <string>
#include <unordered_map>

extern const unsigned int SCREEN_WIDTH, SCREEN_HEIGHT;

class Game
{
public:
    enum class GameState
    {
        GAME_ACTIVE,
        GAME_MENU,
        GAME_WIN
    } State;
    std::vector<bool> Keys;

    // isPressed, xpos, ypos
    std::vector<std::pair<bool, std::pair<double, double> > > MouseInput;

    unsigned int Width, Height;
    // constructor/destructor
    Game(unsigned int width, unsigned int height);
    ~Game();
    // initialize game state (load all shaders/textures/levels)
    void Init();
    // game loop
    void ProcessInput(float dt);
    void Update(float dt);
    void Render();
    bool isClicked(std::shared_ptr<Card> card, glm::vec2* position);
};

#endif