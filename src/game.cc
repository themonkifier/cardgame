#include "../include/game.hh"
#include <iostream>

// Game-related State data
Renderer* spriteRenderer;
TextRenderer* textRenderer;
// std::unordered_map<std::string, std::shared_ptr<Pile<GameObject> > > objectPiles;
std::unordered_map<std::string, std::shared_ptr<Pile<Card> > > piles;
std::unordered_map<std::string, std::shared_ptr<Deck<Card> > > decks;
std::unordered_map<std::string, std::shared_ptr<Texture> > uiElements;

Game::Game(unsigned int width, unsigned int height)
    : State(GameState::MENU), Keys(), MouseInput(), Width(width), Height(height)
{}

Game::~Game() {
    delete spriteRenderer;
    delete textRenderer;
}

void Game::InitMenu() {
    /* REQUIRED FOR ALL GAMES */
    MouseInput[0] = {false, {0, 0}};
    MouseInput[1] = {false, {0, 0}};

    // load shaders
    ResourceManager::LoadShader("../src/sprite_vertex.vert", "../src/sprite_fragment.frag", nullptr, "sprite");
    ResourceManager::LoadShader("../src/text_vertex.vert", "../src/text_fragment.frag", nullptr, "text");

    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
                                      static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);

    ResourceManager::GetShader("text").SetMatrix4("projection", projection);

    // set render-specific controls
    spriteRenderer = new Renderer(ResourceManager::GetShader("sprite"));
    textRenderer = new TextRenderer(ResourceManager::GetShader("text"), Font("../fonts/arial.ttf", 100));


    ResourceManager::LoadTexture("../textures/new_game.jfif", false, "new");
    Texture& buttonNew = ResourceManager::GetTexture("new");
    buttonNew.move({1 * Width / 3, Height / 2});
    uiElements.emplace("new", std::make_shared<Texture>(buttonNew));

    ResourceManager::LoadTexture("../textures/join_game.jfif", false, "join");
    Texture& buttonJoin = ResourceManager::GetTexture("join");
    buttonJoin.move({2 * Width / 3, Height / 2});
    uiElements.emplace("join", std::make_shared<Texture>(buttonJoin));
}

void Game::InitJoin() {
    ;
}

void Game::InitWin() {
    ;
}

void Game::InitActive() {
    /* SPECIFIC TO CRAZY 8's */
    // initialize card deck
    std::vector<std::string> variations = {"Spades", "Hearts", "Clubs", "Diamonds"};
    std::unordered_map<std::string, int> values =
    {{"A", 1}, {"2", 2}, {"3", 3}, {"4", 4}, {"5", 5},
     {"6", 6}, {"7", 7}, {"8", 8}, {"9", 9},
     {"10", 10}, {"J", 11}, {"Q", 12}, {"K", 13}};

    ResourceManager::LoadTexture("../cards/Back.jpg", false, "back");
    Texture& back = ResourceManager::GetTexture("back");

    decks.emplace("cards", std::make_shared<Deck<Card> >(variations, values, back));

    // initialize chips deck
    /*
    variations = {"chip"};
    values =
    {{"White", 1}, {"Red", 5}, {"Blue", 10}, {"Yellow", 25}, 
     {"Black", 100}, {"Orange", 250}, {"Purple", 500},
     {"Silver", 1000}, {"Gold", 5000}};
     */

    std::shared_ptr<Pile<Card> > draw = std::make_shared<Pile<Card> >(decks["cards"]);
    draw->fillFromDeck();
    draw->fillFromDeck();
    draw->shuffle();
    piles.emplace("draw", draw);
    draw->move({SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 - 50});
        
    // create a hand pile for each person playing
    std::vector<std::shared_ptr<Pile<Card> > > hands;
    hands.reserve(NUM_PLAYERS);
    for (unsigned int i = 0; i < NUM_PLAYERS; i++) {
        std::shared_ptr<Pile<Card> > hand = std::make_shared<Pile<Card> >(decks["cards"]);
        piles.emplace("player " + std::to_string(i) + "'s hand",  hand);
        hands.push_back(hand);
    }

    // deal 7 cards to each person’s pile
    for (std::size_t i = 0; i < hands.size(); i++) {
        for (std::size_t j = 0; j < 7; j++) {
            if (draw->size <= 0) {
                draw->fillFromDeck();
                draw->fillFromDeck();
            }
            draw->deal(hands[i]);
            hands[i]->move({i * 100, i * 100});
        }
    }

    // create discard pile
    std::shared_ptr<Pile<Card> > discard = std::make_shared<Pile<Card> >(decks["cards"]);
    piles.emplace("discard", discard);

    // deal top card from deck to discard pile and repeat until it's not an 8
    do {
        if (draw->size <= 0) {
            draw->fillFromDeck();
            draw->fillFromDeck();
        }
        draw->deal(discard, discard->size - 1);
    } while ((*discard)[discard->size - 1].objectInfo.value.second == 8);
    discard->move({SCREEN_WIDTH / 2 + 50, SCREEN_HEIGHT / 2 + 50});

    selectedCard = nullptr;
}

void Game::Update(float dt) {
    switch (State) {
        case GameState::MENU:
        UpdateMenu(dt);
        break;

        case GameState::WIN:
        UpdateWin(dt);
        break;

        case GameState::ACTIVE:
        UpdateActive(dt);
        break;
    }
}

void Game::UpdateMenu(float dt) {
    ;
}

void Game::UpdateWin(float dt) {
    ;
}

void Game::UpdateActive(float dt) {
    double now = glfwGetTime();
    /* if left mouse isn't pressed, make sure every texture knows this*/
    if (!MouseInput[0].first) {
        for (unsigned int i = 0; i < NUM_PLAYERS; i++) {
            for (Card& card : *piles["player " + std::to_string(i) + "'s hand"]) {
                card.texture.isClicked = false;
            }
        }
        selectedCard = nullptr;
    }

    for (unsigned int i = 0; i < NUM_PLAYERS; i++) {
        for (Card& card : *piles["player " + std::to_string(i) + "'s hand"]) {
            if (now - card.texture.lastClickTime > 0.5f) {
                card.texture.clicks = 0;
            }
        }
    }
}

void Game::ProcessInput(float dt) {
    switch (State) {
        case GameState::MENU:
        ProcessMenu(dt);
        break;

        case GameState::JOIN:
        ProcessJoin(dt);
        break;
        
        case GameState::WIN:
        ProcessWin(dt);
        break;
        
        case GameState::ACTIVE:
        ProcessActive(dt);
        break;
    }
}

void Game::ProcessMenu(float dt) {
    if (MouseInput[0].first) {
        if (uiElements.at("new")->contains(MouseInput[0].second, false)) {
            InitActive();
            State = GameState::ACTIVE;
        }
        else if (uiElements.at("join")->contains(MouseInput[0].second, false)) {
            InitJoin();
            State = GameState::JOIN;
        }
    }
}

void Game::ProcessJoin(float dt) {
    ;
}

void Game::ProcessWin(float dt) {
    // if (MouseInput[0].first && uiElements["new"]->isClicked) {
    //     State = GameState::MENU;
    // }
}
void Game::ProcessActive(float dt) {/* needs to happen before the second loop :( */
    if (MouseInput[0].first && selectedCard == nullptr)
    {
        for (unsigned int i = 0; i < NUM_PLAYERS; i++)
        {
            for (Card& card : *piles["player " + std::to_string(i) + "'s hand"])
            {
                if (card.texture.contains(MouseInput[0].second, card.texture.isClicked)
                &&  onTop(card.texture, *piles["player " + std::to_string(i) + "'s hand"], MouseInput[0].second))
                {
                    selectedCard = &card;
                }
            }
        }
    }

    if (MouseInput[0].first && selectedCard != nullptr)
    {
        for (unsigned int i = 0; i < NUM_PLAYERS; i++)
        {
            for (Card& card : *piles["player " + std::to_string(i) + "'s hand"])
            {
                if (card == *selectedCard && card.texture.contains(MouseInput[0].second, card.texture.isClicked)
                &&  onTop(card.texture, *piles["player " + std::to_string(i) + "'s hand"], MouseInput[0].second))
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
                        if (card.texture.onClick(position))
                        {
                            /* on click */
                            card.flipOver();
                        }
                        piles["player " + std::to_string(i) + "'s hand"]->sort();
                    }
                    else
                    {
                        /* on click */
                    }
                }
            }
        }
    }
}

void Game::Render() {
    switch (State) {
        case GameState::MENU:
        RenderMenu();
        break;

        case GameState::JOIN:
        RenderJoin();
        break;
        
        case GameState::WIN:
        RenderWin();
        break;
        
        case GameState::ACTIVE:
        RenderActive();
        break;
    }
}

void Game::RenderMenu() {
    textRenderer->DrawText("This is sample text", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
    textRenderer->DrawText("(C) LearnOpenGL.com", 540.0f, 570.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));
    spriteRenderer->DrawTexture(*uiElements.at("new"));
    spriteRenderer->DrawTexture(*uiElements.at("join"));
}

void Game::RenderJoin() {
    ;
}

void Game::RenderWin() {
    ;
}

void Game::RenderActive() {
    for (unsigned int i = 0; i < NUM_PLAYERS; i++) {
        spriteRenderer->DrawPile(*piles["player " + std::to_string(i) + "'s hand"]);
    }
    spriteRenderer->DrawGameObject(piles["discard"]->back());
    spriteRenderer->DrawGameObject(piles["draw"]->back());
}

bool Game::onTop(Texture& texture, Pile<Card>& pile, std::pair<double, double>& clickPosition) {
    // i don't really like this method - how can we not look at everything? i don't want to store too much but this seems worse
    for (Card& gameobject : pile) {
        if (texture.contains(clickPosition) && gameobject.texture.contains(clickPosition, true) && gameobject.texture.zlevel > texture.zlevel) return false;
    }
    return true;
}