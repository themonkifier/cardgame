#include "../include/deck.hh"

Deck::Deck(std::vector<std::string> suits, std::unordered_map<std::string, int> cardRanksAndValues, bool empty)
    : suits(suits), cardRanksAndValues(cardRanksAndValues)
{
    if (!empty)
    {
        for (std::string suit : suits)
        {
            for (auto cardRankAndValue : cardRanksAndValues)
            {
                cards.emplace_back(new Card(Card::CardInfo(suit, cardRankAndValue.first, cardRankAndValue.second), false));
            }
        }
    }
    size = cards.size();
}

void Deck::shuffle()
{
    srandom(time(nullptr));

    for (int k = size - 1; k > 0; k--)
    {
        int randPos = random() % k;
        std::shared_ptr<Card> temp = std::move(cards[k]);
        cards[k] = std::move(cards[randPos]);
        cards[randPos] = std::move(temp);
    }
}

std::shared_ptr<Card> Deck::dealToFront(std::unique_ptr<Deck> d)
{
    std::shared_ptr<Card> c = cards.front();
    this->cards.pop_front();
    d->cards.push_front(c);

    size--;
    d->size++;
    return c;
}

std::shared_ptr<Card> Deck::dealToBack(std::unique_ptr<Deck> d)
{
    std::shared_ptr<Card> c = cards.front();
    this->cards.pop_front();
    d->cards.push_back(c);

    size--;
    d->size++;
    return c;
}