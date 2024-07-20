#ifndef CARD_HH
#define CARD_HH

#include <string>
#include <optional>

#include "texture.hh"

class Card
{
public:
    Texture texture;
    std::optional<Texture> back;

    enum class Suit {
        Spades = 'S',
        Hearts = 'H',
        Clubs = 'C',
        Diamonds = 'D'
    } suit;
    std::pair<char, int> value;

    bool operator==(const Card& c) const noexcept;
    bool operator<(const Card& c) const noexcept;
    std::size_t operator()(const Card& c) const noexcept;

    void flipOver();

    Card(Suit suit, std::pair<char, int> value, bool alpha = false, Texture back = {});

    std::string toSocketRepresentation();
    static Card fromSocketRepresentation(std::string socket_representation);

private:
    bool alpha;
};

inline std::string to_string(Card c) {
    return c.toSocketRepresentation();
}

template<> struct std::hash<Card>
{
    std::size_t operator()(const Card& c) const noexcept
    {
        return 10665624317 * std::hash<char>{}(static_cast<char>(c.suit)) % 46995126991 + 86166321749;
    }
};

#endif
