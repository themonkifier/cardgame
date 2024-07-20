#ifndef DECK_HH
#define DECK_HH

#include <optional>
#include <vector>
#include <string>
#include <map>

#include "card.hh"

/* treat like a queue for popping - always pop_front
 *  push_back or push_front depending on which deal method is called
 *  "front" and "top" are synonymous, and same with "back" and "bottom"
 *  discard piles should be dealt to top, for instance
 */
class Deck
{
public:
    std::size_t size;

    /**
     * @brief creates a new deck with `suits` suits and values ranks (and their associated values)
     */
    Deck(std::vector<char> suits, std::map<char, int> values, Texture back);

    std::optional<Card> operator[](std::string message);
    std::size_t operator()(const Deck& d) const noexcept;

    std::vector<Card>::iterator begin();
    std::vector<Card>::iterator end();

private:
    std::vector<Card> elements;
    std::vector<char> suits;
    std::map<char, int> values;

    friend struct std::hash<Deck>;
};

template <> struct std::hash<Deck>
{
    std::size_t operator()(const Deck& d) const noexcept
    {
        return 48564466567 * std::hash<char>{}(d.suits[0]) % 68281292231 + 16100840257;
    }
};

#endif
