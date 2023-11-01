#include "../include/game.hh"
#include <iostream>

// Game-related State data
Renderer* renderer;
std::unordered_map<std::string, std::shared_ptr<Pile> > piles;
std::unordered_map<std::string, std::shared_ptr<Deck> > decks;

Game::Game(unsigned int width, unsigned int height)
    : State(GameState::GAME_ACTIVE), Keys(), MouseInput(), Width(width), Height(height)
{}

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

    decks.emplace("cards", std::make_shared<Deck>(suits, cardRanksAndValues));

    std::shared_ptr<Pile> draw = std::make_shared<Pile>(decks["cards"]);
    draw->fillFromDeck();
    std::shared_ptr<Pile> discard = std::make_shared<Pile>(decks["cards"]);
    std::shared_ptr<Pile> visible = std::make_shared<Pile>(decks["cards"]);
    std::shared_ptr<Pile> hand = std::make_shared<Pile>(decks["hand"]);
    piles.emplace("draw", draw);
    piles.emplace("discard", discard);
    piles.emplace("visible", visible);
    piles.emplace("hand", hand);

    // pick random card
    piles["draw"]->shuffle();
    piles["draw"]->shuffle();
    for (int i = 0; i < 5; i++)
    {
        GameObject& testCard = piles["draw"]->move(piles["visible"]);
        testCard.texture.move(glm::vec2(15 * i, 10 * i));
    }
}

void Game::Update(float dt)
{
    /* if left mouse isn't pressed, make sure every texture knows this*/
    if (!MouseInput[0].first)
    {
        for (GameObject& card : *piles["visible"])
        {
            card.texture.isClicked = false;
        }
    }
}

void Game::ProcessInput(float dt)
{
    if (MouseInput[0].first)
    {
        for (GameObject& card : *piles["visible"])
        {
            if (card.texture.contains(MouseInput[0].second, card.texture.isClicked)
            &&  onTop(card.texture, *piles["visible"], MouseInput[0].second))
            {
                if (card.texture.isClicked)
                {
                    card.texture.onHold(glm::vec2(MouseInput[0].second.first,
                        MouseInput[0].second.second) - card.texture.position - card.texture.lastClickPos);
                }
                else if (card.texture.isDraggable)
                {
                    glm::vec2 position(MouseInput[0].second.first - card.texture.position.x,
                        MouseInput[0].second.second - card.texture.position.y);
                    card.texture.onClick(position);
                    card.texture.lastClickPos = position;
                    piles["visible"]->sort();
                }
            }
        }
    }
}

void Game::Render()
{
    renderer->DrawPile(*piles["visible"]);
}

bool Game::onTop(Texture& texture, Pile& pile, std::pair<double, double>& clickPosition)
{
    // i don't really like this method - how can we not look at everything? i don't want to store too much but this seems worse
    for (GameObject& gameobject : pile)
    {
        if (texture.contains(clickPosition) && gameobject.texture.contains(clickPosition) && gameobject.texture.zlevel > texture.zlevel) return false;
    }
    return true;
}