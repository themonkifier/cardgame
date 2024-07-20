#include "../include/pile.hh"

Pile::Pile(Deck& d, bool isDraggable)
    : deck(d), isDraggable(isDraggable)
{
    size = 0;
}

Pile::Pile(Deck& d, std::string message, bool isDraggable)
    : deck(d), isDraggable(isDraggable)
{
    std::size_t size_change = 0;
    for (auto it = message.begin(); it != message.end(); it += 2, size_change++) {
        Card&& c = deck[std::string({*it, *(it + 1)})].value();
        if (!isDraggable) {
            c.flipOver();
        }

        elements.push_back(c);
    }
    size += size_change;
}

void Pile::fillFromDeck()
{
    for (Card& c : deck)
    {
        c.texture.isDraggable = isDraggable;
        if (!isDraggable) {
            c.flipOver();
        }
        elements.push_back(c);
    }
    size += deck.size;
}

Card& Pile::operator[](std::size_t idx)
{
    auto it = elements.begin();
    
    while (idx > 0)
    {
        it++;
        idx--;
    }

    return *it;
}

const Card& Pile::operator[](std::size_t idx) const
{
    auto it = elements.begin();

    while (idx > 0)
    {
        it++;
        idx--;
    }

    return *it;
}

std::size_t Pile::operator()(const Pile& p) const noexcept {
    return 84279839621 * p.size % 11963004479 + 82710400009;
}

Card& Pile::front() {
    return elements.front();
}

Card& Pile::back() {
    return elements.back();
}

std::optional<Card*> Pile::deal(Pile& p, std::size_t pidx, std::size_t tidx)
{
    if (size <= 0) return {};

    auto it = elements.begin();
    for (; it != elements.end() && tidx > 0; it++, tidx--);
    Card* gameobject = &*it;
    elements.erase(it);

    it = p.elements.begin();
    for (; it != p.elements.end() && pidx > 0; it++, pidx--);
    p.elements.insert(it, *gameobject);

    size--;
    p.size++;
    return gameobject;
}

/**
 * @brief copy one card from `this`s deck to `this`
 * @param pidx the index in this to move to
 * @param tidx the index in this's deck to move from
 */
std::optional<std::shared_ptr<Card>> Pile::copy(std::size_t pidx, std::size_t didx)
{
    if (deck.size <= 0) return {};

    auto dit = deck.begin();
    for (; dit != deck.end() && didx > 0; dit++, didx--);
    std::shared_ptr<Card> gameobject = std::make_shared<Card>(*dit);

    auto pit = elements.begin();
    for (; pit != elements.end() && pidx > 0; pit++, pidx--);
    elements.insert(pit, *gameobject);

    size++;
    
    return gameobject;
}

/**
 * @brief sorts the pile by z-index
 */
void Pile::sort() {
    elements.sort();
}

/**
 * @brief shuffles the pile
 */
void Pile::shuffle()
{
    srandom(time(nullptr));

    for (int k = size - 1; k > 0; k--)
    {
        int randPos = random() % k;
        Card temp = std::move((*this)[k]);
        (*this)[k] = std::move((*this)[randPos]);
        (*this)[randPos] = std::move(temp);
    }
}

void Pile::move(glm::vec2 newPosition) {
    position = newPosition;
    for (Card& t : elements) {
        t.texture.move(newPosition);
    }
}