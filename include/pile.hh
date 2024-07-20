#ifndef PILE_HH
#define PILE_HH

#include <memory>
#include <string>
#include <list>
#include <optional>

#include <glad/glad.h>

#include "deck.hh"

class Pile
{
public:
    /** poker chips
     * variations: {"chip"}
     * values: {{"White", 1}, {"Red", 5}, {"Blue", 10},
     * {"Yellow", 25}, {"Black", 100}, {"Orange", 250}, {"Purple", 500},
     * {"Silver", 1000}, {"Gold", 5000}}
     * 
     * monopoly houses
     * variations: {"house"}
     * values: {{"red", NULL}, {"green", NULL}}
     */

    glm::vec2 position;
    std::size_t size;

    Pile(Deck& d, bool isDraggable = false);
    Pile(Deck& d, std::string message, bool isDraggable = false);

    void fillFromDeck();

    Card& operator[](std::size_t idx);
    const Card& operator[](std::size_t idx) const;
    std::size_t operator()(const Pile& p) const noexcept;

    Card& front();
    Card& back();

    inline auto begin() { return elements.begin(); }

    inline auto end() { return elements.end(); }

    /**
     * @brief move one card from `this` to `p`
     * @param p the pile to move to
     * @param pidx the index in pile p to move to
     * @param tidx the index in this pile to move from
     */
    std::optional<Card*> deal(Pile& p, std::size_t pidx = 0, std::size_t tidx = 0);

    /**
     * @brief copy one card from `this`s deck to `this`
     * @param pidx the index in this to move to
     * @param tidx the index in this's deck to move from
     */
    std::optional<std::shared_ptr<Card>> copy(std::size_t pidx = 0, std::size_t didx = 0);

    /**
     * @brief sorts the pile by z-index
     */
    void sort();

    /**
     * @brief shuffles the pile
     */
    void shuffle();

    void move(glm::vec2 newPosition);
private:
    std::list<Card> elements;
    std::string pathToTextures, fileExtension;
    Deck& deck; // the cards allowable within this pile
    bool isDraggable;
};

template <> struct std::hash<Pile>
{
    std::size_t operator()(const Pile& p) const noexcept
    {
        return 84279839621 * p.size % 11963004479 + 82710400009;
    }
};

inline std::string to_string(Pile pile) {
    std::string ret;
    ret.reserve(1 + 2 * pile.size);

    ret += (unsigned char) (pile.size);

    for (const Card& card : pile) {
        ret += to_string(card);
    }

    return ret;
}

#endif
