#include "../include/deck.hh"
#include <memory>

Deck::Deck(std::vector<char> suits, std::map<char, int> values, Texture back)
    : suits(suits), values(values)
{
    for (char suit : suits)
    {
        for (std::pair<char, int> value : values)
        {
            std::unique_ptr<Card> card = std::make_unique<Card>(static_cast<Card::Suit>(suit), value, false, back);
            elements.push_back(*card);
        }
    }
    size = elements.size();
}

std::optional<Card> Deck::operator[](std::string message)
{
    Card value = Card::fromSocketRepresentation(message);

    for (Card& card : elements) {
        if (card == value) {
            return card;
        }
    }

    return {};
}

std::size_t Deck::operator()(const Deck& d) const noexcept
{
    return 48564466567 * std::hash<char>{}(d.suits[0]) % 68281292231 + 16100840257;
}

std::vector<Card>::iterator Deck::begin()
{
    return elements.begin();
}

std::vector<Card>::iterator Deck::end()
{
    return elements.end();
}
