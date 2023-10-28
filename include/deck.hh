#ifndef DECK_HH
#define DECK_HH

#include <vector>
#include <iostream>
#include <string>
#include <unordered_map>
#include <deque>

#include "card.hh"

class Deck
{
public:
    /* treat like a queue for popping - always pop_front
       push_back or push_front depending on which deal method is called
       "front" and "top" are synonymous, and same with "back" and "bottom"
       discard piles should be dealt to top, for instance */
    std::deque<std::shared_ptr<Card>> cards;
    std::vector<std::string> suits;
    std::unordered_map<std::string, int> cardRanksAndValues;
    std::size_t size;

    /**
      * @brief creates a new deck with `suits` suits and cardRanksAndValues ranks (and their associated values)
      */
    Deck(std::vector<std::string> suits, std::unordered_map<std::string, int> cardRanksAndValues, bool empty = false);
    void shuffle();
    std::shared_ptr<Card> dealToFront(std::unique_ptr<Deck> d);
    std::shared_ptr<Card> dealToBack(std::unique_ptr<Deck> d);
};

#endif