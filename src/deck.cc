#include "../include/deck.hh"

Deck::Deck(std::vector<std::string> suits, std::unordered_map<std::string, int> cardRanksAndValues)
    : suits(suits), cardRanksAndValues(cardRanksAndValues)
{
    for (std::string suit : suits)
    {
        for (auto cardRankAndValue : cardRanksAndValues)
        {
            std::shared_ptr<GameObject> gameobject = std::make_shared<GameObject>(GameObject::ObjectInfo(suit, cardRankAndValue), false);
            elements.push_back(*gameobject);
        }
    }
    size = elements.size();
}

std::size_t Deck::operator()(const Deck& d) const noexcept
{
    return 48564466567 * std::hash<std::string>{}(d.suits[0]) % 68281292231 + 16100840257;
}