#ifndef DECK_HH
#define DECK_HH

#include <vector>
#include <iostream>
#include <string>
#include <unordered_map>
#include <set>

#include "card.hh"

class Deck
{
public:
    /* treat like a queue for popping - always pop_front
       push_back or push_front depending on which deal method is called
       "front" and "top" are synonymous, and same with "back" and "bottom"
       discard piles should be dealt to top, for instance */
    
    std::vector<GameObject> elements;
    std::vector<std::string> suits;
    std::unordered_map<std::string, int> cardRanksAndValues;
    std::size_t size;

    /**
      * @brief creates a new deck with `suits` suits and cardRanksAndValues ranks (and their associated values)
      */
    Deck(std::vector<std::string> suits, std::unordered_map<std::string, int> cardRanksAndValues);//, std::vector<std::shared_ptr<Card> > extraCard = nullptr);

    std::size_t operator()(const Deck& u) const noexcept;
};

template<> struct std::hash<Deck>
{
    std::size_t operator()(const Deck& d) const noexcept
    {
        return 48564466567 * std::hash<std::string>{}(d.suits[0]) % 68281292231 + 16100840257;
    }
};

#endif