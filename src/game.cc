#include "../include/game.hh"
#include "../include/server.hh"
#include "../include/renderer.hh"
#include "../include/info.hh"
#include "../include/resource_manager.hh"

unsigned int DEFAULT_HAND_SIZE = 7;
// Game-related State data
Renderer* spriteRenderer;

Game::Game(TextRenderer& textRenderer, GLFWwindow* window)
    : State(GameState::MENU), window(window),
      textRenderer(textRenderer), MouseInput()
{
    piles.reserve(2);
    decks.reserve(1);
    hands.reserve(NUM_PLAYERS);
}

Game::~Game() {
    delete spriteRenderer;
}

void Game::SetState(GameState newState) {
    State = newState;

    /* clean up previous state */
    if (client != nullptr && State != GameState::ACTIVE) {
        client->quit();
    }
    ui_elements.clear();
    hands.clear();
    decks.clear();
    piles.clear();

    selectedCard = nullptr;

    Init();
}

void Game::OnEscape() {
    switch (State) {
        using enum GameState;
        case MENU:
        glfwSetWindowShouldClose(window, true);
        break;

        case WIN:
        case ACTIVE:
        case JOIN:
        client.reset(nullptr);
        SetState(MENU);
        break;
    }
}

std::string Game::Notify(std::string message) {
    /* note: it's safe to dereference here because Notify(std::string)
     * should never be called manually, only by the client */
    return message;
}

void Game::Init() {
    switch (State) {
        using enum GameState;

        case MENU:
        InitMenu();
        break;

        case JOIN:
        InitJoin();
        break;

        case ACTIVE:
        InitActive();
        break;

        case WIN:
        InitWin();
        break;
    }
}

void Game::InitMenu() {
    /* REQUIRED FOR ALL GAMES */
    MouseInput[0] = {false, {0, 0}};
    MouseInput[1] = {false, {0, 0}};

    // load shaders
    ResourceManager::LoadShader("../src/sprite_vertex.vert", "../src/sprite_fragment.frag", nullptr, "sprite");

    // configure shaders
    glm::mat4 sprite = glm::ortho(0.0f, static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", sprite);

    // set render-specific controls
    spriteRenderer = new Renderer(ResourceManager::GetShader("sprite"));


    ResourceManager::LoadTexture("../textures/new_game.jfif", false, "new");
    Texture& buttonNew = ResourceManager::GetTexture("new");
    buttonNew.move({1 * SCREEN_WIDTH / 3, SCREEN_HEIGHT / 2});
    ui_elements.emplace("new", std::make_unique<Texture>(buttonNew));

    ResourceManager::LoadTexture("../textures/join_game.jfif", false, "join");
    Texture& buttonJoin = ResourceManager::GetTexture("join");
    buttonJoin.move({2 * SCREEN_WIDTH / 3, SCREEN_HEIGHT / 2});
    ui_elements.emplace("join", std::make_unique<Texture>(buttonJoin));
}

void enterCallbackJoin(Game* game, TextBox* tb) {
    game->client = std::make_unique<Client>(tb->contained_text, game);
    std::cout << "joining server on address " << tb->contained_text << std::endl;
    game->SetState(Game::GameState::ACTIVE);
}

void Game::InitJoin() {
    std::unique_ptr<TextBox> join_box = std::make_unique<TextBox>(this);
    join_box->selected = true;
    join_box->enterCallback = enterCallbackJoin;

    text_boxes["join"] = std::move(join_box);
}

void Game::InitWin() {}

void Game::InitActive() {
    /* SPECIFIC TO CRAZY 8's */
    // initialize card deck
    std::vector<char> variations = {'S', 'H', 'C', 'D'};
    std::map<char, int> values =
    {{'A', 1}, {'2', 2}, {'3', 3}, {'4', 4}, {'5', 5},
     {'6', 6}, {'7', 7}, {'8', 8}, {'9', 9},
     {'T', 10}, {'J', 11}, {'Q', 12}, {'K', 13}};

    ResourceManager::LoadTexture("../cards/Back.jpg", false, "back");
    Texture& back = ResourceManager::GetTexture("back");

    decks.emplace_back(std::make_unique<Deck>(variations, values, back));

    // initialize chips deck
    /*
    variations = {"chip"};
    values =
    {{"White", 1}, {"Red", 5}, {"Blue", 10}, {"Yellow", 25},
     {"Black", 100}, {"Orange", 250}, {"Purple", 500},
     {"Silver", 1000}, {"Gold", 5000}};
     */

    selectedCard = nullptr;

    client->SetupActive();
}

/* here we're shadowing 'client', but i think that's ok? */

void Game::Update(float dt) {
    switch (State) {
        using enum GameState;

        case MENU:
        UpdateMenu(dt);
        break;

        case JOIN:
        UpdateJoin(dt);
        break;

        case WIN:
        UpdateWin(dt);
        break;

        case ACTIVE:
        UpdateActive(dt);
        break;
    }
}

void Game::UpdateMenu(float dt) {
    (void) dt;
}

void Game::UpdateJoin(float dt) {
    (void) dt;
}

void Game::UpdateWin(float dt) {
    (void) dt;
}

void Game::UpdateActive(float dt) {
    (void) dt;

    double now = glfwGetTime();
    /* if left mouse isn't pressed, make sure every texture knows this*/
    if (!MouseInput[0].first) {
        for (const auto& pile : piles) {
            for (Card& card : *pile) {
                card.texture.isClicked = false;
            }
        }
        for (const auto& hand : hands) {
            for (Card& card : *hand) {
                card.texture.isClicked = false;
            }
        }
        selectedCard = nullptr;

        for (const auto& pile : piles) {
            for (Card& card : *pile) {
                if (now - card.texture.lastClickTime > 0.5f) {
                    card.texture.clicks = 0;
                }
            }
        }
        for (const auto& hand : hands) {
            for (Card& card : *hand) {
                if (now - card.texture.lastClickTime > 0.5f) {
                    card.texture.clicks = 0;
                }
            }
        }
    }

}

void Game::ProcessInput(float dt) {
    switch (State) {
        using enum GameState;

        case MENU:
        ProcessMenu(dt);
        break;

        case JOIN:
        ProcessJoin(dt);
        break;

        case WIN:
        ProcessWin(dt);
        break;

        case ACTIVE:
        ProcessActive(dt);
        break;
    }
}

void Game::ProcessMenu(float dt) {
    (void) dt;

    if (MouseInput[0].first) {
        if (ui_elements.at("new")->contains(MouseInput[0].second, false)) {
            client = std::make_unique<Server>(NUM_PLAYERS - 1, this); // don't include yourself!
            client->sendMsg(GetStateString());
            SetState(GameState::ACTIVE);
        }
        else if (ui_elements.at("join")->contains(MouseInput[0].second, false)) {
            SetState(GameState::JOIN);
        }
    }
}

void Game::ProcessJoin(float dt) {
    (void) dt;
}

void Game::ProcessWin(float dt) {
    (void) dt;

    if (MouseInput[0].first && ui_elements.at("new")->isClicked) {
        SetState(GameState::MENU);
    }
}

void Game::ProcessActive(float dt) {
    (void) dt;

    printf("%d", selectedCard == nullptr);
    /* needs to happen before the second loop :( */
    if (MouseInput[0].first && selectedCard == nullptr)
    {
        for (unsigned int i = 0; i < NUM_PLAYERS; i++)
        {
            for (Card& card : *piles[i])
            {
                if (card.texture.contains(MouseInput[0].second, card.texture.isClicked)
                &&  onTop(card.texture, *piles[i], MouseInput[0].second))
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
            for (Card& card : *piles[i])
            {
                if (card == *selectedCard && card.texture.contains(MouseInput[0].second, card.texture.isClicked)
                &&  onTop(card.texture, *piles[i], MouseInput[0].second))
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
                        piles[i]->sort();
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
        using enum GameState;

        case MENU:
        RenderMenu();
        break;

        case JOIN:
        RenderJoin();
        break;

        case WIN:
        RenderWin();
        break;

        case ACTIVE:
        RenderActive();
        break;
    }
}

void Game::RenderMenu() {
    spriteRenderer->DrawTexture(*ui_elements.at("new"));
    spriteRenderer->DrawTexture(*ui_elements.at("join"));
}

void Game::RenderJoin() {
    textRenderer.DrawText("Enter IP Address:", 230.0f, 400.0f);
    textRenderer.DrawText(text_boxes.at("join")->contained_text, 230.0f, 200.0f, 0.75f);
}

void Game::RenderWin() {
    ;
}

void Game::RenderActive() {
    for (unsigned int i = 0; i < NUM_PLAYERS; i++) {
        spriteRenderer->DrawPile(*hands[i]);
    }
    spriteRenderer->DrawCard(piles[0]->back());
    spriteRenderer->DrawCard(piles[1]->back());
}

std::string Game::GetStateString() {
    std::string state_string;
    for (std::size_t i = 0; i < piles.size(); i++) {
        state_string += 'g' + std::to_string(i) + to_string(*piles[i]); // generate new deck
    }
    return state_string;
}

bool Game::onTop(Texture& texture, Pile& pile, std::pair<double, double>& clickPosition) {
    // i don't really like this method - how can we not look at everything? i don't want to store too much but this seems worse
    for (Card& gameobject : pile) {
        if (texture.contains(clickPosition) && gameobject.texture.contains(clickPosition, true) && gameobject.texture.zlevel > texture.zlevel) return false;
    }
    return true;
}
