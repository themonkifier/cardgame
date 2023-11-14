#ifndef DECK_HH
#define DECK_HH

#include <vector>
#include <iostream>
#include <string>
#include <unordered_map>
#include <set>

#include "card.hh"

template <typename T>
class Deck
{
public:
    /* treat like a queue for popping - always pop_front
       push_back or push_front depending on which deal method is called
       "front" and "top" are synonymous, and same with "back" and "bottom"
       discard piles should be dealt to top, for instance */
    
    std::vector<T> elements;
    std::vector<std::string> suits;
    std::unordered_map<std::string, int> cardRanksAndValues;
    std::size_t size;

    /**
      * @brief creates a new deck with `suits` suits and cardRanksAndValues ranks (and their associated values)
      */
    Deck(std::vector<std::string> suits, std::unordered_map<std::string, int> cardRanksAndValues) //, std::vector<std::shared_ptr<Card> > extraCard = nullptr);
       : suits(suits), cardRanksAndValues(cardRanksAndValues)
    {
        for (std::string suit : suits)
        {
            for (auto cardRankAndValue : cardRanksAndValues)
            {
                std::shared_ptr<T> gameobject = std::make_shared<T>(GameObject::ObjectInfo(suit, cardRankAndValue), false);
                elements.push_back(*gameobject);
            }
        }
        size = elements.size();
    }

    Deck<T>(std::vector<std::string> suits, std::unordered_map<std::string, int> cardRanksAndValues, Texture& back) //, std::vector<std::shared_ptr<Card> > extraCard = nullptr);
            : suits(suits), cardRanksAndValues(cardRanksAndValues)
    {
        for (std::string suit : suits)
        {
            for (auto cardRankAndValue : cardRanksAndValues)
            {
                std::shared_ptr<Card> gameobject = std::make_shared<T>(GameObject::ObjectInfo(suit, cardRankAndValue), false, back);
                elements.push_back(*gameobject);
            }
        }
        size = elements.size();
    }

    std::size_t operator()(const Deck<T>& d) const noexcept
    {
        return 48564466567 * std::hash<std::string>{}(d.suits[0]) % 68281292231 + 16100840257;
    }
};

template <typename T> struct std::hash<Deck<T>>
{
    std::size_t operator()(const Deck<T>& d) const noexcept
    {
        return 48564466567 * std::hash<std::string>{}(d.suits[0]) % 68281292231 + 16100840257;
    }
};

#endif