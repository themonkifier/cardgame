#include "../include/game.hh"
#include <iostream>

// Game-related State data
Renderer *renderer;

Game::Game(unsigned int width, unsigned int height)
    : State(GameState::GAME_ACTIVE), Keys(), Width(width), Height(height)
{
}

Game::~Game()
{
    delete renderer;
}

void Game::Init()
{
    // load shaders
    ResourceManager::LoadShader("../src/vertex_shader.vert", "../src/fragment_shader.frag", nullptr, "sprite");
    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
                                      static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    // set render-specific controls
    renderer = new Renderer(ResourceManager::GetShader("sprite"));
    // load textures
    ResourceManager::LoadTexture("../cards/Back.jpg", false, "face");
}

void Game::Update(float dt)
{
}

void Game::ProcessInput(float dt)
{
}

void Game::Render()
{
    renderer->DrawSprite( ResourceManager::GetTexture("face"), glm::vec2(136.0f, 190.0f),
                          glm::vec2(136.0f, 190.0f), 45.0f, glm::vec3(1.0f, 1.0f, 1.0f) );
}