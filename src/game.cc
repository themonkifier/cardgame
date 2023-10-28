#include "../include/game.hh"
#include <iostream>

// Game-related State data
Renderer* renderer;
std::vector<std::shared_ptr<Card>> cards;
std::unordered_map<std::string, std::unique_ptr<Deck>> decks;

Game::Game(unsigned int width, unsigned int height)
    : State(GameState::GAME_ACTIVE), Keys(), MouseInput(), Width(width), Height(height)
{
}

Game::~Game()
{
    delete renderer;
}

void Game::Init()
{
    Keys.reserve(1024);
    MouseInput.reserve(2);
    MouseInput[0] = {false, {0, 0}};
    MouseInput[1] = {false, {0, 0}};

    // load shaders
    ResourceManager::LoadShader("../src/vertex_shader.vert", "../src/fragment_shader.frag", nullptr, "sprite");
    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
                                      static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    // set render-specific controls
    renderer = new Renderer(ResourceManager::GetShader("sprite"));
    
    // initialize deck
    std::vector<std::string> suits = {"Spades", "Hearts", "Clubs", "Diamonds"};
    std::unordered_map<std::string, int> cardRanksAndValues =
    {{"A", 1}, {"2", 2}, {"3", 3}, {"4", 4}, {"5", 5},
    {"6", 6}, {"7", 7}, {"8", 8}, {"9", 9},
    {"10", 10}, {"J", 11}, {"Q", 12}, {"K", 13}};

    decks.emplace("draw", std::make_unique<Deck>(suits, cardRanksAndValues));
    decks.emplace("discard", std::make_unique<Deck>(suits, cardRanksAndValues, true));
    decks.emplace("hand", std::make_unique<Deck>(suits, cardRanksAndValues, true));

    // pick random card
    decks["draw"]->shuffle();
    decks["draw"]->shuffle();
    std::shared_ptr<Card> testCard = decks["draw"]->dealToFront(std::move(decks["discard"]));
    cards.push_back(testCard);
    testCard->move(glm::vec2(800, 800));
}

void Game::Update(float dt)
{
    if (!MouseInput[0].first)
    {
        for (std::shared_ptr<Card> card : cards)
        {
            card->isClicked = false;
        }
    }
}

void Game::ProcessInput(float dt)
{
    // if (MouseInput[0].first) std::cout << "left mouse\n";
    // if (MouseInput[1].second) std::cout << "right mouse\n";

    glm::vec2 position;
    if (MouseInput[0].first)
    {
        for (std::shared_ptr card : cards)
        {
            if (isClicked(card, &position))
            {
                std::cout << position.x << " " << position.y << "\n";
                if (card->isClicked) card->onHold(position - card->position);
                else if (card->isDraggable) card->onClick(position - card->position);
            }
        }
    }
}

void Game::Render()
{
    renderer->DrawCard(cards.at(0));
}

bool Game::isClicked(std::shared_ptr<Card> card, glm::vec2* position)
{
    position->x = MouseInput[0].second.first;
    position->y = MouseInput[0].second.second;
    
    if ((position->x + 50 >= card->position.x && position->x <= card->position.x + card->size.x)
     && (position->y + 50 >= card->position.y && position->y <= card->position.y + card->size.y)) return true;
    return false;
}